// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * The GPU batcher. Everything libzrnui draws is a rounded rectangle (filled,
 * outlined or as a soft shadow, lines being thin rotated ones) or a glyph
 * from one coverage atlas. Each is a quad whose corners carry where in the
 * shape they are; one fragment shader works out the edge from a signed
 * distance, so corners and lines are smooth at any scale and under any
 * transform, with no tessellation and no stencil. A frame is usually one
 * draw call. Nothing is rasterised on the CPU except glyphs, once each.
 */
#define _GNU_SOURCE
#include <GLES2/gl2.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zui-internal.h"

struct vtx {
	float x, y, z;		/* points, after the transform */
	float lx, ly;		/* where in the shape (points from its middle), or atlas uv */
	float hx, hy;		/* the shape's half size */
	float r, bw, blur, mode;
	uint8_t c[4];		/* premultiplied */
};
enum { M_FILL = 0, M_BORDER = 1, M_SHADOW = 2, M_GLYPH = 3, M_IMAGE = 4, M_DOT = 5, M_ROW = 6, M_WEDGE = 7 };

#define MAX_QUADS 8192
static struct {
	bool ready;
	unsigned generation;
	GLuint program, vbo, ibo, atlas;
	GLint u_size, u_eye, u_px, u_atlas, u_image;
	GLuint bound_image;
	GLint u_dots, u_dorg, u_main;
	float dots, dorg; uint32_t dmain;
	struct vtx v[MAX_QUADS * 4];
	int n;		/* quads */
	int fb_w, fb_h;
	float scale, w, h;
	struct zg_xf xf;
	bool has_xf;
	float alpha;
	bool clip_on;
	int clip[4];
	/* the atlas, filled in shelves */
	int shelf_x, shelf_y, shelf_h;
} G_main, G_row;
/* each thread that draws has its own batcher, program and atlas; textures are shared */
static __thread __typeof__(G_main) *Gp = &G_main;
#define G (*Gp)
void zg_use_row_batcher(void) { Gp = &G_row; }
unsigned zg_main_generation(void) { return G_main.generation; }
bool zg_ready_here(void) { return G.ready; }

static const char VS[] =
	"attribute vec3 a_pos; attribute vec2 a_local; attribute vec2 a_half;"
	"attribute vec4 a_p; attribute vec4 a_color;"
	"uniform vec2 u_size; uniform float u_eye;"
	"varying vec2 v_local; varying vec2 v_half; varying vec4 v_p; varying vec4 v_color;"
	"void main(){ v_local=a_local; v_half=a_half; v_p=a_p; v_color=a_color;"
	"gl_Position=vec4(2.0*a_pos.x/u_size.x-1.0, 1.0-2.0*a_pos.y/u_size.y, 0.0, 1.0-a_pos.z/u_eye); }";

static const char FS[] =
	"#ifdef GL_FRAGMENT_PRECISION_HIGH\nprecision highp float;\n#else\nprecision mediump float;\n#endif\n"
	"varying vec2 v_local; varying vec2 v_half; varying vec4 v_p; varying vec4 v_color;"
	"uniform float u_px; uniform sampler2D u_atlas; uniform sampler2D u_image;"
	"uniform float u_dots; uniform float u_dorg; uniform vec3 u_main;"
	"float box(vec2 p, vec2 b, float r){ vec2 q=abs(p)-b+r;"
	" return min(max(q.x,q.y),0.0)+length(max(q,0.0))-r; }"
	"void main(){ float mode=v_p.w;"
	" if(mode>6.5){ float e1=box(v_local,v_half,0.0); float e2=dot(v_local,v_p.xy)+v_p.z;"
	"  gl_FragColor=v_color*clamp(0.5-e1/u_px,0.0,1.0)*clamp(0.5-e2/u_px,0.0,1.0); return; }"
	" if(mode>5.5){ gl_FragColor=texture2D(u_image,vec2(0.5+0.5*v_local.x/v_half.x,0.5-0.5*v_local.y/v_half.y))*v_color.a; return; }"
	" if(mode>4.5){ float q=box(v_local,v_half,v_p.x); gl_FragColor=v_color*clamp(0.5-q/u_px,0.0,1.0); return; }"
	" vec2 off=vec2(0.0); float lit=1.0;"
	" if(u_dots>0.0&&mode<2.5||u_dots>0.0&&mode>3.5){ vec2 fc=vec2(gl_FragCoord.x,-gl_FragCoord.y)*u_px;"
	"  fc.y-=u_dorg; vec2 cell=(floor(fc/u_dots)+0.5)*u_dots; off=cell-fc;"
	"  lit=clamp(0.5+(u_dots*0.36-length(off))/u_px,0.0,1.0); }"
	" if(mode>3.5){ vec2 lc=v_local+off; float e=box(lc,v_half,v_p.x);"
	"  if(u_dots>0.0){ vec2 uu=clamp(0.5+0.5*lc/v_half*v_p.yz,0.0,1.0); vec4 tx=texture2D(u_image,uu);"
	"   float lum=0.12+0.88*dot(tx.rgb,vec3(0.3,0.59,0.11)); gl_FragColor=vec4(u_main*lum,1.0)*v_color.a*lit*step(e,0.0); return; }"
	"  e=box(v_local,v_half,v_p.x);"
	"  vec2 uv=0.5+0.5*v_local/v_half*v_p.yz; uv=clamp(uv,0.0,1.0);"
	"  gl_FragColor=texture2D(u_image,uv)*v_color.a*clamp(0.5-e/u_px,0.0,1.0); return; }"
	" if(mode>2.5){ gl_FragColor=v_color*texture2D(u_atlas,v_local).a; return; }"
	" float d=box(v_local+off,v_half,v_p.x);"
	" float a;"
	" if(u_dots>0.0&&mode<1.5){ a=step(d,0.0); if(mode>0.5) a*=step(-v_p.y-u_dots*0.5,d); gl_FragColor=v_color*a*lit; return; }"
	" if(mode<0.5){ a=clamp(0.5-d/u_px,0.0,1.0); }"
	" else if(mode<1.5){ a=clamp(0.5-d/u_px,0.0,1.0)*clamp(0.5+(d+v_p.y)/u_px,0.0,1.0); }"
	" else { float b=max(v_p.z,u_px); a=1.0-smoothstep(-b,b,d); a=a*a*(3.0-2.0*a); }"
	" gl_FragColor=v_color*a; }";

static GLuint shader(GLenum type, const char *src) {
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &src, NULL);
	glCompileShader(s);
	GLint ok = 0;
	glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		char log[1024];
		glGetShaderInfoLog(s, sizeof(log), NULL, log);
		fprintf(stderr, "zrnui: shader: %s\n", log);
		glDeleteShader(s);
		return 0;
	}
	return s;
}

unsigned zg_generation(void) { return G.generation; }
float zui_gfx_scale(void) { return G.scale > 0 ? G.scale : 1; }

bool zg_init(void) {
	if (G.ready) zg_fini();
	GLuint vs = shader(GL_VERTEX_SHADER, VS), fs = shader(GL_FRAGMENT_SHADER, FS);
	if (!vs || !fs) return false;
	G.program = glCreateProgram();
	glAttachShader(G.program, vs);
	glAttachShader(G.program, fs);
	glBindAttribLocation(G.program, 0, "a_pos");
	glBindAttribLocation(G.program, 1, "a_local");
	glBindAttribLocation(G.program, 2, "a_half");
	glBindAttribLocation(G.program, 3, "a_p");
	glBindAttribLocation(G.program, 4, "a_color");
	glLinkProgram(G.program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	GLint ok = 0;
	glGetProgramiv(G.program, GL_LINK_STATUS, &ok);
	if (!ok) {
		fprintf(stderr, "zrnui: the shader program did not link\n");
		return false;
	}
	G.u_size = glGetUniformLocation(G.program, "u_size");
	G.u_eye = glGetUniformLocation(G.program, "u_eye");
	G.u_px = glGetUniformLocation(G.program, "u_px");
	G.u_atlas = glGetUniformLocation(G.program, "u_atlas");
	G.u_image = glGetUniformLocation(G.program, "u_image");
	G.u_dots = glGetUniformLocation(G.program, "u_dots");
	G.u_dorg = glGetUniformLocation(G.program, "u_dorg");
	G.u_main = glGetUniformLocation(G.program, "u_main");
	glGenBuffers(1, &G.vbo);
	glGenBuffers(1, &G.ibo);
	uint16_t *idx = malloc(sizeof(uint16_t) * 6 * (65536 / 4));
	for (int q = 0; q < 65536 / 4; q++) {
		uint16_t b = (uint16_t)(q * 4);
		uint16_t *o = idx + q * 6;
		o[0] = b; o[1] = b + 1; o[2] = b + 2; o[3] = b + 2; o[4] = b + 1; o[5] = b + 3;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6 * (65536 / 4), idx, GL_STATIC_DRAW);
	free(idx);
	glBindBuffer(GL_ARRAY_BUFFER, G.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(G.v), NULL, GL_STREAM_DRAW);
	glGenTextures(1, &G.atlas);
	glBindTexture(GL_TEXTURE_2D, G.atlas);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	uint8_t *zero = calloc(ZG_ATLAS, ZG_ATLAS);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ZG_ATLAS, ZG_ATLAS, 0, GL_ALPHA, GL_UNSIGNED_BYTE, zero);
	free(zero);
	G.shelf_x = G.shelf_y = G.shelf_h = 0;
	G.ready = true;
	G.generation++;
	G.alpha = 1;
	return true;
}

void zg_fini(void) {
	if (!G.ready) return;
	glDeleteProgram(G.program);
	glDeleteBuffers(1, &G.vbo);
	glDeleteBuffers(1, &G.ibo);
	glDeleteTextures(1, &G.atlas);
	G.ready = false;
}

static void flush(void) {
	if (!G.n) return;
	glBindBuffer(GL_ARRAY_BUFFER, G.vbo);
	/* a new store each time: updating one the GPU still reads makes the driver
	 * copy it whole, a large part of a frame on the small cores */
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vtx) * 4 * G.n, G.v, GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G.ibo);
	const GLsizei st = sizeof(struct vtx);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, st, (void *)offsetof(struct vtx, x));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, st, (void *)offsetof(struct vtx, lx));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, st, (void *)offsetof(struct vtx, hx));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, st, (void *)offsetof(struct vtx, r));
	glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, st, (void *)offsetof(struct vtx, c));
	for (int i = 0; i < 5; i++) glEnableVertexAttribArray(i);
	/* 16-bit indices: at most 16384 quads a call */
	for (int q = 0; q < G.n; q += 16384) {
		int count = G.n - q < 16384 ? G.n - q : 16384;
		if (q) {
			const char *base = (const char *)0 + sizeof(struct vtx) * 4 * q;
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, st, base + offsetof(struct vtx, x));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, st, base + offsetof(struct vtx, lx));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, st, base + offsetof(struct vtx, hx));
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, st, base + offsetof(struct vtx, r));
			glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, st, base + offsetof(struct vtx, c));
		}
		glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, 0);
	}
	G.n = 0;
}

void zg_begin(int fb_w, int fb_h, float scale, uint32_t clear) {
	G.fb_w = fb_w;
	G.fb_h = fb_h;
	G.scale = scale;
	G.w = fb_w / scale;
	G.h = fb_h / scale;
	G.n = 0;
	G.has_xf = false;
	G.alpha = 1;
	G.clip_on = false;
	glViewport(0, 0, fb_w, fb_h);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(((clear >> 24) & 0xff) / 255.0f, ((clear >> 16) & 0xff) / 255.0f,
		((clear >> 8) & 0xff) / 255.0f, (clear & 0xff) / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(G.program);
	glUniform2f(G.u_size, G.w, G.h);
	/* the eye as far from the screen as the shell's 34 degree field of view puts it */
	glUniform1f(G.u_eye, (G.h / 2) / tanf(17.0f * (float)M_PI / 180.0f));
	glUniform1f(G.u_px, 1.0f / scale);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, G.atlas);
	glUniform1i(G.u_atlas, 0);
	glUniform1i(G.u_image, 1);
	G.bound_image = 0;
}

void zg_end(void) {
	flush();
	glDisable(GL_SCISSOR_TEST);
}

/* ---- transforms ---- */
void zg_xf_identity(struct zg_xf *xf) {
	memset(xf, 0, sizeof(*xf));
	xf->m[0] = xf->m[5] = xf->m[10] = xf->m[15] = 1;
}

bool zg_xf_is_identity(const struct zg_xf *xf) {
	struct zg_xf i;
	zg_xf_identity(&i);
	return memcmp(&i, xf, sizeof(i)) == 0;
}

/* xf = op * xf */
static void premul(struct zg_xf *xf, const float op[16]) {
	float r[16];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			float s = 0;
			for (int k = 0; k < 4; k++) s += op[i * 4 + k] * xf->m[k * 4 + j];
			r[i * 4 + j] = s;
		}
	memcpy(xf->m, r, sizeof(r));
}

void zg_xf_translate(struct zg_xf *xf, float x, float y, float z) {
	const float op[16] = { 1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1 };
	premul(xf, op);
}

void zg_xf_scale_about(struct zg_xf *xf, float s, float cx, float cy) {
	zg_xf_translate(xf, -cx, -cy, 0);
	const float op[16] = { s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0, 0, 0, 0, 1 };
	premul(xf, op);
	zg_xf_translate(xf, cx, cy, 0);
}

/* positive takes the right-hand side away from the viewer */
void zg_xf_rotate_y(struct zg_xf *xf, float deg, float px) {
	float a = deg * (float)M_PI / 180, c = cosf(a), s = sinf(a);
	zg_xf_translate(xf, -px, 0, 0);
	const float op[16] = { c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1 };
	premul(xf, op);
	zg_xf_translate(xf, px, 0, 0);
}

/* positive takes the bottom away from the viewer */
void zg_xf_rotate_x(struct zg_xf *xf, float deg, float py) {
	float a = deg * (float)M_PI / 180, c = cosf(a), s = sinf(a);
	zg_xf_translate(xf, 0, -py, 0);
	const float op[16] = { 1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1 };
	premul(xf, op);
	zg_xf_translate(xf, 0, py, 0);
}

void zg_xf_apply(const struct zg_xf *xf, float x, float y, float out[3]) {
	const float *m = xf->m;
	float w = m[12] * x + m[13] * y + m[15];
	if (w == 0) w = 1;
	out[0] = (m[0] * x + m[1] * y + m[3]) / w;
	out[1] = (m[4] * x + m[5] * y + m[7]) / w;
	out[2] = (m[8] * x + m[9] * y + m[11]) / w;
}

void zg_set_xf(const struct zg_xf *xf) {
	if (xf && !zg_xf_is_identity(xf)) {
		G.xf = *xf;
		G.has_xf = true;
	} else {
		G.has_xf = false;
	}
}

const struct zg_xf *zg_get_xf(void) {
	static struct zg_xf id;
	if (G.has_xf) return &G.xf;
	zg_xf_identity(&id);
	return &id;
}

void zg_set_alpha(float a) { G.alpha = a < 0 ? 0 : a > 1 ? 1 : a; }
float zg_get_alpha(void) { return G.alpha; }

void zg_clip(float x, float y, float w, float h) {
	int c[4] = { (int)floorf(x * G.scale), (int)floorf(G.fb_h - (y + h) * G.scale),
		(int)ceilf(w * G.scale), (int)ceilf(h * G.scale) };
	if (c[2] < 0) c[2] = 0;
	if (c[3] < 0) c[3] = 0;
	if (G.clip_on && memcmp(c, G.clip, sizeof(c)) == 0) return;
	flush();
	memcpy(G.clip, c, sizeof(c));
	G.clip_on = true;
	glEnable(GL_SCISSOR_TEST);
	glScissor(c[0], c[1], c[2], c[3]);
}

void zg_noclip(void) {
	if (!G.clip_on) return;
	flush();
	G.clip_on = false;
	glDisable(GL_SCISSOR_TEST);
}

/* ---- colours ---- */
uint32_t zc_mix(uint32_t a, uint32_t b, float t) {
	uint32_t out = 0;
	for (int s = 0; s < 32; s += 8) {
		float ca = (a >> s) & 0xff, cb = (b >> s) & 0xff;
		out |= (uint32_t)lroundf(ca + (cb - ca) * t) << s;
	}
	return out;
}

float zc_luma(uint32_t c) {
	return (0.2126f * ((c >> 24) & 0xff) + 0.7152f * ((c >> 16) & 0xff) + 0.0722f * ((c >> 8) & 0xff)) / 255.0f;
}

/* ---- quads ---- */
static void emit(const float pos[4][2], const float local[4][2], float hx, float hy,
		float r, float bw, float blur, float mode, uint32_t rgba) {
	if (G.n >= MAX_QUADS) flush();
	float a = ((rgba & 0xff) / 255.0f) * G.alpha;
	if (a <= 0.002f) return;
	uint8_t c[4] = {
		(uint8_t)lroundf(((rgba >> 24) & 0xff) * a), (uint8_t)lroundf(((rgba >> 16) & 0xff) * a),
		(uint8_t)lroundf(((rgba >> 8) & 0xff) * a), (uint8_t)lroundf(255 * a) };
	struct vtx *v = G.v + G.n * 4;
	for (int i = 0; i < 4; i++) {
		if (G.has_xf) {
			float o[3];
			zg_xf_apply(&G.xf, pos[i][0], pos[i][1], o);
			v[i].x = o[0]; v[i].y = o[1]; v[i].z = o[2];
		} else {
			v[i].x = pos[i][0]; v[i].y = pos[i][1]; v[i].z = 0;
		}
		v[i].lx = local[i][0]; v[i].ly = local[i][1];
		v[i].hx = hx; v[i].hy = hy;
		v[i].r = r; v[i].bw = bw; v[i].blur = blur; v[i].mode = mode;
		memcpy(v[i].c, c, 4);
	}
	G.n++;
}

/* an axis-aligned shape with `pad` points of room round it for its soft edge */
static void shape(float x, float y, float w, float h, float r, float bw, float blur, float mode,
		uint32_t rgba) {
	if (w <= 0 || h <= 0) return;
	float hx = w / 2, hy = h / 2;
	if (r > hx) r = hx;
	if (r > hy) r = hy;
	float pad = 1.5f / G.scale + blur * 1.2f + G.dots;
	float x0 = x - pad, y0 = y - pad, x1 = x + w + pad, y1 = y + h + pad;
	float cx = x + hx, cy = y + hy;
	const float pos[4][2] = { { x0, y0 }, { x1, y0 }, { x0, y1 }, { x1, y1 } };
	const float local[4][2] = { { x0 - cx, y0 - cy }, { x1 - cx, y0 - cy }, { x0 - cx, y1 - cy },
		{ x1 - cx, y1 - cy } };
	emit(pos, local, hx, hy, r, bw, blur, mode, rgba);
}

void zg_rect(float x, float y, float w, float h, float r, uint32_t rgba) {
	/* black is what is behind the dots: a bar over a list must hide it whole */
	if (G.dots && (rgba >> 8) == 0) {
		shape(x, y, w, h, r, 0, 0, M_DOT, rgba);
		return;
	}
	shape(x, y, w, h, r, 0, 0, M_FILL, rgba);
}

void zg_border(float x, float y, float w, float h, float r, float width, uint32_t rgba) {
	shape(x, y, w, h, r, width, 0, M_BORDER, rgba);
}

void zg_shadow(float x, float y, float w, float h, float r, float blur, uint32_t rgba) {
	shape(x, y, w, h, r, 0, blur, M_SHADOW, rgba);
}

void zg_circle(float cx, float cy, float radius, uint32_t rgba) {
	shape(cx - radius, cy - radius, radius * 2, radius * 2, radius, 0, 0, M_FILL, rgba);
}

void zg_ring(float cx, float cy, float radius, float width, uint32_t rgba) {
	shape(cx - radius, cy - radius, radius * 2, radius * 2, radius, width, 0, M_BORDER, rgba);
}

/* a stroke with round ends: a rounded rectangle turned along the line */
/* A right triangle on the box x, y, w, h: its vertical side at the left or right,
 * its flat side at the top or bottom, the slope smooth at any scale and turn. */
void zg_wedge(float x, float y, float w, float h, bool right, bool bottom, uint32_t rgba) {
	float hx = w / 2, hy = h / 2, len = sqrtf(w * w + h * h);
	float nx = (right ? -h : h) / len, ny = (right == bottom ? -w : w) / len;
	/* the slope joins the two corners away from the right angle */
	float px = right ? hx : -hx, py = bottom ? -hy : hy;
	float cut = -(nx * (right ? -hx : hx) + ny * py);
	(void)px;
	float pad = 1.5f / G.scale, x0 = x - pad, y0 = y - pad, x1 = x + w + pad, y1 = y + h + pad, cx = x + hx, cy = y + hy;
	const float pos[4][2] = { { x0, y0 }, { x1, y0 }, { x0, y1 }, { x1, y1 } };
	const float local[4][2] = { { x0 - cx, y0 - cy }, { x1 - cx, y0 - cy }, { x0 - cx, y1 - cy }, { x1 - cx, y1 - cy } };
	emit(pos, local, hx, hy, nx, ny, cut, M_WEDGE, rgba);
}

void zg_line(float x0, float y0, float x1, float y1, float width, uint32_t rgba) {
	float dx = x1 - x0, dy = y1 - y0, len = sqrtf(dx * dx + dy * dy);
	if (len < 1e-4f) {
		zg_circle(x0, y0, width / 2, rgba);
		return;
	}
	float ux = dx / len, uy = dy / len;	/* along */
	float nx = -uy, ny = ux;		/* across */
	float hx = len / 2 + width / 2, hy = width / 2;
	float pad = 1.5f / G.scale;
	float ex = hx + pad, ey = hy + pad;
	float cx = (x0 + x1) / 2, cy = (y0 + y1) / 2;
	const float s[4][2] = { { -ex, -ey }, { ex, -ey }, { -ex, ey }, { ex, ey } };
	float pos[4][2], local[4][2];
	for (int i = 0; i < 4; i++) {
		pos[i][0] = cx + ux * s[i][0] + nx * s[i][1];
		pos[i][1] = cy + uy * s[i][0] + ny * s[i][1];
		local[i][0] = s[i][0];
		local[i][1] = s[i][1];
	}
	emit(pos, local, hx, hy, hy, 0, 0, M_FILL, rgba);
}

void zg_arc(float cx, float cy, float radius, float a0, float a1, float width, uint32_t rgba) {
	int steps = (int)ceilf(fabsf(a1 - a0) / 12.0f);
	if (steps < 2) steps = 2;
	float px = 0, py = 0;
	for (int i = 0; i <= steps; i++) {
		float a = (a0 + (a1 - a0) * i / steps) * (float)M_PI / 180;
		float x = cx + cosf(a) * radius, y = cy + sinf(a) * radius;
		if (i) zg_line(px, py, x, y, width, rgba);
		px = x;
		py = y;
	}
}

void zg_set_dots(float pitch, uint32_t main_rgb) {
	flush();
	G.dots = pitch;
	G.dmain = main_rgb;
	glUniform1f(G.u_dots, pitch);
	glUniform1f(G.u_dorg, 0);
	glUniform3f(G.u_main, ((main_rgb >> 16) & 255) / 255.0f, ((main_rgb >> 8) & 255) / 255.0f, (main_rgb & 255) / 255.0f);
}

void zg_set_dot_origin(float y) {
	if (!G.dots || y == G.dorg) { G.dorg = y; return; }
	flush();
	G.dorg = y;
	glUniform1f(G.u_dorg, y);
}

void zg_dot(float cx, float cy, float radius, uint32_t rgba) {
	shape(cx - radius, cy - radius, radius * 2, radius * 2, radius, 0, 0, M_DOT, rgba);
}

/* ---- rows painted once: into a texture, then only moved ---- */
static __thread GLuint row_fbo;
void zg_row_begin(unsigned tex, int w, int h, float scale) {
	if (!row_fbo) glGenFramebuffers(1, &row_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, row_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	zg_begin(w, h, scale, 0x00000000);
}
void zg_row_end(void) {
	zg_end();
	glFinish();	/* the other context may sample it as soon as it is published */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned zg_row_tex(int w, int h) {
	GLuint t;
	glGenTextures(1, &t);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	G.bound_image = t;
	glActiveTexture(GL_TEXTURE0);
	return t;
}
void zg_row(unsigned tex, float x, float y, float w, float h) {
	if (G.bound_image != tex) {
		flush();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE0);
		G.bound_image = tex;
	}
	float hx = w / 2, hy = h / 2;
	const float pos[4][2] = { { x, y }, { x + w, y }, { x, y + h }, { x + w, y + h } };
	const float local[4][2] = { { -hx, -hy }, { hx, -hy }, { -hx, hy }, { hx, hy } };
	emit(pos, local, hx, hy, 0, 0, 0, M_ROW, 0xffffffff);
	flush();
}

/* ---- pictures: their own texture, so a draw call each ---- */
unsigned zg_tex_new(int w, int h, const uint8_t *rgba, bool nearest) {
	GLuint t;
	glGenTextures(1, &t);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t);
	GLint f = nearest ? GL_NEAREST : GL_LINEAR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	G.bound_image = t;
	glActiveTexture(GL_TEXTURE0);
	return t;
}

void zg_tex_free(unsigned tex) {
	GLuint t = tex;
	flush();
	if (G.bound_image == t) G.bound_image = 0;
	glDeleteTextures(1, &t);
}

void zg_image(unsigned tex, float x, float y, float w, float h, float r, float crop_x, float crop_y) {
	if (G.bound_image != tex) {
		flush();	/* what is queued used the picture bound until now */
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex);
		glActiveTexture(GL_TEXTURE0);
		G.bound_image = tex;
	}
	float hx = w / 2, hy = h / 2;
	if (r > hx) r = hx;
	if (r > hy) r = hy;
	float cx = x + hx, cy = y + hy;
	const float pos[4][2] = { { x, y }, { x + w, y }, { x, y + h }, { x + w, y + h } };
	const float local[4][2] = { { -hx, -hy }, { hx, -hy }, { -hx, hy }, { hx, hy } };
	(void)cx; (void)cy;
	emit(pos, local, hx, hy, r, crop_x, crop_y, M_IMAGE, 0xffffffff);
	flush();	/* the next picture rebinds; keep one draw for each */
}

/* ---- an app's own fragment shader ----
 * A brush can paint with one: the same vertex stage as everything else, and at each pixel the colour the
 * app's `vec4 zui_shade(vec2 uv)` gives (uv 0..1 over the area, y down; straight, not premultiplied
 * alpha). It may read u_time (seconds), u_res (the area's size in points) and any uniform of its own.
 * The area's rounded corners, the alpha and DotMatrix (the colour's brightness as dots) are done here. */
static const char USER_FS_HEAD[] =
	"#ifdef GL_FRAGMENT_PRECISION_HIGH\nprecision highp float;\n#else\nprecision mediump float;\n#endif\n"
	"varying vec2 v_local; varying vec2 v_half; varying vec4 v_p; varying vec4 v_color;"
	"uniform float u_px; uniform float u_dots; uniform float u_dorg; uniform vec3 u_main;"
	"uniform float u_time; uniform vec2 u_res;\n";
static const char USER_FS_TAIL[] =
	"\nfloat zui_box(vec2 p, vec2 b, float r){ vec2 q=abs(p)-b+r;"
	" return min(max(q.x,q.y),0.0)+length(max(q,0.0))-r; }"
	"void main(){"
	" if(u_dots>0.0){ vec2 fc=vec2(gl_FragCoord.x,-gl_FragCoord.y)*u_px; fc.y-=u_dorg;"
	"  vec2 cell=(floor(fc/u_dots)+0.5)*u_dots; vec2 off=cell-fc;"
	"  float lit=clamp(0.5+(u_dots*0.36-length(off))/u_px,0.0,1.0);"
	"  vec2 lc=v_local+off; float e=zui_box(lc,v_half,v_p.x);"
	"  vec4 c=zui_shade(0.5+0.5*lc/v_half); float lum=0.12+0.88*dot(c.rgb,vec3(0.3,0.59,0.11));"
	"  gl_FragColor=vec4(u_main*lum,1.0)*c.a*v_color.a*lit*step(e,0.0); return; }"
	" float e=zui_box(v_local,v_half,v_p.x);"
	" vec4 c=zui_shade(0.5+0.5*v_local/v_half);"
	" gl_FragColor=vec4(c.rgb*c.a,c.a)*v_color.a*clamp(0.5-e/u_px,0.0,1.0); }";

unsigned zg_shader_compile(const char *user, char *err, size_t errn) {
	if (err && errn) err[0] = 0;
	/* #line 1: an error says the line of the app's own source */
	const char *parts[4] = { USER_FS_HEAD, "#line 1\n", user, USER_FS_TAIL };
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 4, parts, NULL);
	glCompileShader(fs);
	GLint ok = 0;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		if (err && errn) glGetShaderInfoLog(fs, (GLsizei)errn, NULL, err);
		glDeleteShader(fs);
		return 0;
	}
	GLuint vs = shader(GL_VERTEX_SHADER, VS);
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glBindAttribLocation(prog, 0, "a_pos");
	glBindAttribLocation(prog, 1, "a_local");
	glBindAttribLocation(prog, 2, "a_half");
	glBindAttribLocation(prog, 3, "a_p");
	glBindAttribLocation(prog, 4, "a_color");
	glLinkProgram(prog);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glGetProgramiv(prog, GL_LINK_STATUS, &ok);
	if (!ok) {
		if (err && errn) glGetProgramInfoLog(prog, (GLsizei)errn, NULL, err);
		glDeleteProgram(prog);
		return 0;
	}
	return prog;
}

void zg_shader_free(unsigned prog) {
	if (prog) glDeleteProgram(prog);
}

/* the area, painted by the program; `uniforms` are the app's own, name and four floats each */
void zg_shader_rect(unsigned prog, float x, float y, float w, float h, float r, float time,
		int nuniforms, const char *const *names, const float (*values)[4]) {
	if (w <= 0 || h <= 0 || !prog) return;
	flush();	/* what is queued is for the main program */
	glUseProgram(prog);
	glUniform2f(glGetUniformLocation(prog, "u_size"), G.w, G.h);
	glUniform1f(glGetUniformLocation(prog, "u_eye"), (G.h / 2) / tanf(17.0f * (float)M_PI / 180.0f));
	glUniform1f(glGetUniformLocation(prog, "u_px"), 1.0f / G.scale);
	glUniform1f(glGetUniformLocation(prog, "u_dots"), G.dots);
	glUniform1f(glGetUniformLocation(prog, "u_dorg"), G.dorg);
	glUniform3f(glGetUniformLocation(prog, "u_main"), ((G.dmain >> 16) & 255) / 255.0f,
		((G.dmain >> 8) & 255) / 255.0f, (G.dmain & 255) / 255.0f);
	glUniform1f(glGetUniformLocation(prog, "u_time"), time);
	glUniform2f(glGetUniformLocation(prog, "u_res"), w, h);
	for (int i = 0; i < nuniforms; i++) {
		GLint at = glGetUniformLocation(prog, names[i]);
		if (at >= 0) glUniform4f(at, values[i][0], values[i][1], values[i][2], values[i][3]);
	}
	float hx = w / 2, hy = h / 2;
	if (r > hx) r = hx;
	if (r > hy) r = hy;
	const float pos[4][2] = { { x, y }, { x + w, y }, { x, y + h }, { x + w, y + h } };
	const float local[4][2] = { { -hx, -hy }, { hx, -hy }, { -hx, hy }, { hx, hy } };
	emit(pos, local, hx, hy, r, 0, 0, M_FILL, 0xffffffff);
	flush();	/* drawn by this program, now */
	glUseProgram(G.program);
}

/* ---- glyphs ---- */
bool zg_atlas_alloc(int w, int h, int *x, int *y) {
	if (w > ZG_ATLAS || h > ZG_ATLAS) return false;
	if (G.shelf_x + w > ZG_ATLAS) {
		G.shelf_y += G.shelf_h + 1;
		G.shelf_x = 0;
		G.shelf_h = 0;
	}
	if (G.shelf_y + h > ZG_ATLAS) return false;
	*x = G.shelf_x;
	*y = G.shelf_y;
	G.shelf_x += w + 1;
	if (h > G.shelf_h) G.shelf_h = h;
	return true;
}

void zg_atlas_reset(void) {
	flush();	/* what is queued still samples the old glyphs */
	G.shelf_x = G.shelf_y = G.shelf_h = 0;
}

void zg_atlas_upload(int x, int y, int w, int h, const uint8_t *pixels, int pitch) {
	if (w <= 0 || h <= 0) return;
	glBindTexture(GL_TEXTURE_2D, G.atlas);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (pitch == w) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
	} else {
		uint8_t *tight = malloc((size_t)w * h);
		for (int r = 0; r < h; r++) memcpy(tight + (size_t)r * w, pixels + (size_t)r * pitch, w);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, tight);
		free(tight);
	}
}

void zg_glyph(float x, float y, float w, float h, float u0, float v0, float u1, float v1, uint32_t rgba) {
	const float pos[4][2] = { { x, y }, { x + w, y }, { x, y + h }, { x + w, y + h } };
	const float uv[4][2] = { { u0, v0 }, { u1, v0 }, { u0, v1 }, { u1, v1 } };
	emit(pos, uv, 0, 0, 0, 0, 0, M_GLYPH, rgba);
}
