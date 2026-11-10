/**
 * zrnui - render/gles.c
 * The OpenGL ES 2 renderer: the painter (render/painter.c) on the GPU.
 *
 * Every shape is a quad with a distance function evaluated in the fragment
 * shader (rounded rectangles, ellipses, lines), so the edges are anti-aliased
 * like those of the software painter. Glyphs come from an atlas, images from
 * one texture each. A transform (u_xf, a 4x4 matrix applied to the device
 * position, with perspective) is the way to animate: the painter draws a page
 * under a rotation without the controls knowing.
 *
 * The context is an EGL window of a Wayland surface (zrn_gl_create_window), or
 * an off-screen one with a framebuffer object (zrn_gl_create_offscreen), which
 * Zroot_render uses for a screenshot and the tests use to compare with the
 * software painter. Without EGL and GLES2 at build time every function here
 * says it is not available, and the software painter is all there is.
 */

#include "../internal.h"

#ifdef ZRNUI_HAVE_GL

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#ifdef ZRNUI_HAVE_WAYLAND
#include <wayland-egl.h>
#endif

static constexpr int FLOATS = 18; /* x y  lx ly  hx hy  r0 r1 r2 r3  aux mode  r g b a  u v */
static constexpr int MAX_QUADS = 4096;
static constexpr int ATLAS_SIZE = 2048;

enum { MODE_SOLID = 0, MODE_RRECT, MODE_ELLIPSE, MODE_RING, MODE_GLYPH, MODE_IMAGE };

typedef struct atlas_slot {
	const uint8_t* key;
	int w, h, x, y;
} atlas_slot_t;

struct zrn_gl {
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	int offscreen;
#ifdef ZRNUI_HAVE_WAYLAND
	struct wl_egl_window* wl_window;
#endif
	GLuint program, atlas, fbo, fbo_texture;
	/** The samples of the off-screen framebuffer (GL_EXT_multisampled_render_to_texture), 0 when it has none. */
	GLsizei fbo_samples;
	PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC fbo_multisample;
	GLint loc_size, loc_xf, loc_tex;
	int width, height;
	float* verts;
	int quads;
	int batch_clip[4], batch_clip_set;
	GLuint batch_texture;
	/* the atlas of glyphs: shelf packing, keyed by the bitmap of the glyph cache */
	atlas_slot_t* slots;
	size_t slot_count, slot_cap;
	int shelf_x, shelf_y, shelf_h;
	float xf[16];
	int has_xf;
	zrn_image_t** images;
	int image_count, image_cap;
};

static const char* VERTEX_SHADER =
	"attribute vec2 a_pos; attribute vec2 a_local; attribute vec2 a_half; attribute vec4 a_radii;\n"
	"attribute vec2 a_misc; attribute vec4 a_color; attribute vec2 a_uv;\n"
	"uniform vec2 u_size; uniform mat4 u_xf;\n"
	"varying vec2 v_local; varying vec2 v_half; varying vec4 v_radii; varying float v_aux; varying float v_mode;\n"
	"varying vec4 v_color; varying vec2 v_uv;\n"
	"void main() {\n"
	"  vec4 p = u_xf * vec4(a_pos, 0.0, 1.0);\n"
	"  gl_Position = vec4(p.x / u_size.x * 2.0 - 1.0, 1.0 - p.y / u_size.y * 2.0, 0.0, p.w);\n"
	"  v_local = a_local; v_half = a_half; v_radii = a_radii; v_aux = a_misc.x; v_mode = a_misc.y;\n"
	"  v_color = a_color; v_uv = a_uv;\n"
	"}\n";

static const char* FRAGMENT_SHADER =
	"precision highp float;\n"
	"varying vec2 v_local; varying vec2 v_half; varying vec4 v_radii; varying float v_aux; varying float v_mode;\n"
	"varying vec4 v_color; varying vec2 v_uv;\n"
	"uniform sampler2D u_tex;\n"
	"float rrect(vec2 p, vec2 h, vec4 r) {\n"
	"  float rad = p.x < 0.0 ? (p.y < 0.0 ? r.x : r.w) : (p.y < 0.0 ? r.y : r.z);\n"
	"  vec2 q = abs(p) - h + rad;\n"
	"  return length(max(q, vec2(0.0))) + min(max(q.x, q.y), 0.0) - rad;\n"
	"}\n"
	"float ellipse(vec2 p, vec2 r) {\n"
	"  float len = length(p / r);\n"
	"  return (len - 1.0) * min(r.x, r.y);\n"
	"}\n"
	"void main() {\n"
	"  float cov = 1.0;\n"
	"  if (v_mode < 0.5) cov = 1.0;\n"
	"  else if (v_mode < 1.5) cov = clamp(0.5 - rrect(v_local, v_half, v_radii), 0.0, 1.0);\n"
	"  else if (v_mode < 2.5) cov = clamp(0.5 - ellipse(v_local, v_half), 0.0, 1.0);\n"
	"  else if (v_mode < 3.5) { float d = ellipse(v_local, v_half); cov = clamp(0.5 - (abs(d + v_aux * 0.5) - v_aux * 0.5), 0.0, 1.0); }\n"
	"  else if (v_mode < 4.5) cov = texture2D(u_tex, v_uv).a;\n"
	"  else { vec4 t = texture2D(u_tex, v_uv); gl_FragColor = vec4(t.rgb, t.a * v_color.a); return; }\n"
	"  gl_FragColor = vec4(v_color.rgb, v_color.a * cov);\n"
	"}\n";

static GLuint compile(const GLenum type, const char* const source) {
	const GLuint s = glCreateShader(type);
	GLint ok = 0;
	glShaderSource(s, 1, &source, nullptr);
	glCompileShader(s);
	glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		char log[512];
		glGetShaderInfoLog(s, sizeof log, nullptr, log);
		zrn_error(Z_ERROR_IO, "OpenGL shader: %s", log);
		glDeleteShader(s);
		return 0;
	}
	return s;
}

static int build_program(zrn_gl_t* const gl) {
	const GLuint vs = compile(GL_VERTEX_SHADER, VERTEX_SHADER), fs = compile(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
	GLint ok = 0;
	if (!vs || !fs) return 0;
	gl->program = glCreateProgram();
	glAttachShader(gl->program, vs);
	glAttachShader(gl->program, fs);
	glBindAttribLocation(gl->program, 0, "a_pos");
	glBindAttribLocation(gl->program, 1, "a_local");
	glBindAttribLocation(gl->program, 2, "a_half");
	glBindAttribLocation(gl->program, 3, "a_radii");
	glBindAttribLocation(gl->program, 4, "a_misc");
	glBindAttribLocation(gl->program, 5, "a_color");
	glBindAttribLocation(gl->program, 6, "a_uv");
	glLinkProgram(gl->program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glGetProgramiv(gl->program, GL_LINK_STATUS, &ok);
	if (!ok) {
		zrn_error(Z_ERROR_IO, "OpenGL program did not link");
		return 0;
	}
	gl->loc_size = glGetUniformLocation(gl->program, "u_size");
	gl->loc_xf = glGetUniformLocation(gl->program, "u_xf");
	gl->loc_tex = glGetUniformLocation(gl->program, "u_tex");
	return 1;
}


/* ============================================================================
 *  EGL
 * ============================================================================ */

static constexpr GLsizei MSAA_SAMPLES = 4;

/* A config with 4x MSAA; a driver without one gets a plain config, so that the GPU renderer still works (without anti-aliased edges). */
static int choose(zrn_gl_t* const gl, const int window) {
	const EGLint surface_type = window ? EGL_WINDOW_BIT : EGL_PBUFFER_BIT;
	const EGLint multisampled[] = {
		EGL_SURFACE_TYPE, surface_type,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 1, EGL_SAMPLES, MSAA_SAMPLES, EGL_NONE
	};
	const EGLint plain[] = {
		EGL_SURFACE_TYPE, surface_type,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE
	};
	EGLint n = 0;
	if (eglChooseConfig(gl->display, multisampled, &gl->config, 1, &n) && n > 0) return 1;
	Z_DEBUG_LOG("no 4x MSAA config: the edges are not anti-aliased\n");
	if (eglChooseConfig(gl->display, plain, &gl->config, 1, &n) && n > 0) return 1;
	zrn_error(Z_ERROR_IO, "no usable EGL config");
	return 0;
}

/* The off-screen framebuffer is a texture, which has no samples of its own: where the driver can multisample a texture
 * attachment (GL_EXT_multisampled_render_to_texture, resolved when the frame ends) the framebuffer gets the samples. */
static void find_framebuffer_multisample(zrn_gl_t* const gl) {
	const char* const extensions = (const char*)glGetString(GL_EXTENSIONS);
	gl->fbo_samples = 0;
	gl->fbo_multisample = nullptr;
	if (!extensions || !strstr(extensions, "GL_EXT_multisampled_render_to_texture")) return;
	gl->fbo_multisample = (PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC)eglGetProcAddress("glFramebufferTexture2DMultisampleEXT");
	if (!gl->fbo_multisample) return;
	GLint most = 0;
	glGetIntegerv(GL_MAX_SAMPLES_EXT, &most);
	gl->fbo_samples = most < MSAA_SAMPLES ? most : MSAA_SAMPLES;
}

static int init_gl(zrn_gl_t* const gl) {
	if (!build_program(gl)) return 0;
	gl->verts = (float*)malloc((size_t)MAX_QUADS * 6 * FLOATS * sizeof(float));
	if (!gl->verts) return 0;
	glGenTextures(1, &gl->atlas);
	glBindTexture(GL_TEXTURE_2D, gl->atlas);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ATLAS_SIZE, ATLAS_SIZE, 0, GL_ALPHA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return 1;
}

/* Roots share the EGLDisplay of a platform: it is terminated when the last of them lets it go, not by the first that is destroyed. */
typedef struct display_use {
	EGLDisplay display;
	int users;
} display_use_t;

static display_use_t g_displays[8];

static void display_acquire(const EGLDisplay display) {
	display_use_t* free_slot = nullptr;
	for (size_t i = 0; i < sizeof g_displays / sizeof g_displays[0]; i++) {
		if (g_displays[i].display == display && g_displays[i].users > 0) {
			g_displays[i].users++;
			return;
		}
		if (g_displays[i].users == 0 && !free_slot) free_slot = &g_displays[i];
	}
	if (!free_slot) return;
	free_slot->display = display;
	free_slot->users = 1;
}

/* Terminates the display when nobody else uses it. A display that was never counted (the table was full) is terminated as before. */
static void display_release(const EGLDisplay display) {
	for (size_t i = 0; i < sizeof g_displays / sizeof g_displays[0]; i++) {
		if (g_displays[i].display != display || g_displays[i].users == 0) continue;
		if (--g_displays[i].users > 0) return;
		break;
	}
	eglTerminate(display);
}

static int open_display(zrn_gl_t* const gl, const EGLenum platform, void* const native) {
	EGLint major, minor;
	const PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	gl->display = EGL_NO_DISPLAY;
	if (get_platform && platform) gl->display = get_platform(platform, native, nullptr);
	if (gl->display == EGL_NO_DISPLAY && !platform) gl->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (gl->display == EGL_NO_DISPLAY || !eglInitialize(gl->display, &major, &minor)) {
		zrn_error(Z_ERROR_IO, "no EGL display");
		return 0;
	}
	display_acquire(gl->display);
	if (!eglBindAPI(EGL_OPENGL_ES_API)) {
		zrn_error(Z_ERROR_IO, "no OpenGL ES");
		display_release(gl->display);
		gl->display = EGL_NO_DISPLAY;
		return 0;
	}
	return 1;
}

static int make_context(zrn_gl_t* const gl) {
	constexpr EGLint ctx_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	gl->context = eglCreateContext(gl->display, gl->config, EGL_NO_CONTEXT, ctx_attribs);
	if (gl->context == EGL_NO_CONTEXT) {
		zrn_error(Z_ERROR_IO, "cannot create an OpenGL ES 2 context");
		return 0;
	}
	return 1;
}

static void gl_free(zrn_gl_t* gl);

zrn_gl_t* zrn_gl_create_offscreen(const int width, const int height) {
	zrn_gl_t* gl = (zrn_gl_t*)calloc(1, sizeof *gl);
	const EGLint pb[] = { EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE };
	if (!gl) return nullptr;
	gl->offscreen = 1;
	if (!open_display(gl, EGL_PLATFORM_SURFACELESS_MESA, nullptr) && !open_display(gl, 0, nullptr)) goto fail;
	if (!choose(gl, 0) || !make_context(gl)) goto fail;
	gl->surface = eglCreatePbufferSurface(gl->display, gl->config, pb);
	if (gl->surface == EGL_NO_SURFACE || !eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context)) {
		zrn_error(Z_ERROR_IO, "cannot make the OpenGL context current");
		goto fail;
	}
	if (!init_gl(gl)) goto fail;
	find_framebuffer_multisample(gl);
	glGenTextures(1, &gl->fbo_texture);
	glGenFramebuffers(1, &gl->fbo);
	zrn_gl_resize(gl, width, height);
	return gl;
fail:
	gl_free(gl);
	return nullptr;
}

zrn_gl_t* zrn_gl_create_window(void* const wl_display, void* const wl_surface, const int width, const int height) {
#ifdef ZRNUI_HAVE_WAYLAND
	zrn_gl_t* gl = (zrn_gl_t*)calloc(1, sizeof *gl);
	if (!gl) return nullptr;
	if (!open_display(gl, EGL_PLATFORM_WAYLAND_KHR, wl_display)) goto fail;
	if (!choose(gl, 1) || !make_context(gl)) goto fail;
	gl->wl_window = wl_egl_window_create((struct wl_surface*)wl_surface, width, height);
	if (!gl->wl_window) goto fail;
	gl->surface = eglCreateWindowSurface(gl->display, gl->config, (EGLNativeWindowType)gl->wl_window, nullptr);
	if (gl->surface == EGL_NO_SURFACE || !eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context)) {
		zrn_error(Z_ERROR_IO, "cannot make the OpenGL context current");
		goto fail;
	}
	eglSwapInterval(gl->display, 0);
	if (!init_gl(gl)) goto fail;
	gl->width = width;
	gl->height = height;
	return gl;
fail:
	gl_free(gl);
	return nullptr;
#else
	(void)wl_display; (void)wl_surface; (void)width; (void)height;
	return nullptr;
#endif
}

void zrn_gl_resize(zrn_gl_t* const gl, const int width, const int height) {
	if (!gl) return;
	if (gl->offscreen) {
		eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context);
		glBindTexture(GL_TEXTURE_2D, gl->fbo_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, gl->fbo);
		if (gl->fbo_samples > 0) gl->fbo_multisample(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl->fbo_texture, 0, gl->fbo_samples);
		else glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl->fbo_texture, 0);
	}
#ifdef ZRNUI_HAVE_WAYLAND
	else if (gl->wl_window) wl_egl_window_resize(gl->wl_window, width, height, 0, 0);
#endif
	gl->width = width;
	gl->height = height;
}

static void gl_free(zrn_gl_t* const gl) {
	int i;
	if (!gl) return;
	if (gl->display != EGL_NO_DISPLAY && gl->context != EGL_NO_CONTEXT) {
		eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context);
		for (i = 0; i < gl->image_count; i++) {
			const GLuint t = gl->images[i]->gl_texture;
			glDeleteTextures(1, &t);
			gl->images[i]->gl_texture = 0;
			gl->images[i]->gl_owner = nullptr;
		}
		if (gl->program) glDeleteProgram(gl->program);
		if (gl->atlas) glDeleteTextures(1, &gl->atlas);
		if (gl->fbo_texture) glDeleteTextures(1, &gl->fbo_texture);
		if (gl->fbo) glDeleteFramebuffers(1, &gl->fbo);
		eglMakeCurrent(gl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	}
	if (gl->display != EGL_NO_DISPLAY) {
		if (gl->surface != EGL_NO_SURFACE && gl->surface) eglDestroySurface(gl->display, gl->surface);
		if (gl->context != EGL_NO_CONTEXT && gl->context) eglDestroyContext(gl->display, gl->context);
		display_release(gl->display);
	}
#ifdef ZRNUI_HAVE_WAYLAND
	if (gl->wl_window) wl_egl_window_destroy(gl->wl_window);
#endif
	free(gl->verts);
	free(gl->slots);
	free(gl->images);
	free(gl);
}

void zrn_gl_destroy(zrn_gl_t* gl) { gl_free(gl); }

void zrn_gl_forget_image(zrn_image_t* const image) {
	const auto gl = (zrn_gl_t*)image->gl_owner;
	if (!gl) return;
	eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context);
	{
		const GLuint t = image->gl_texture;
		glDeleteTextures(1, &t);
	}
	for (int i = 0; i < gl->image_count; i++) {
		if (gl->images[i] == image) {
			gl->images[i] = gl->images[--gl->image_count];
			break;
		}
	}
	image->gl_texture = 0;
	image->gl_owner = nullptr;
}


/* ============================================================================
 *  batching
 * ============================================================================ */

static void flush(zrn_gl_t* const gl) {
	static constexpr GLsizei stride = (GLsizei)(FLOATS * sizeof(float));
	const float* v = gl->verts;
	if (!gl->quads) return;
	glUseProgram(gl->program);
	glUniform2f(gl->loc_size, (float)gl->width, (float)gl->height);
	glUniformMatrix4fv(gl->loc_xf, 1, GL_FALSE, gl->xf);
	glUniform1i(gl->loc_tex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gl->batch_texture ? gl->batch_texture : gl->atlas);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, v + 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, v + 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, v + 4);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, v + 6);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, v + 10);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, v + 12);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, stride, v + 16);
	{ int i; for (i = 0; i < 7; i++) glEnableVertexAttribArray((GLuint)i); }
	glDrawArrays(GL_TRIANGLES, 0, gl->quads * 6);
	{ int i; for (i = 0; i < 7; i++) glDisableVertexAttribArray((GLuint)i); }
	gl->quads = 0;
}

/* Gets the batch ready for a draw with this clip and texture: a change of either draws what is waiting. */
static float* reserve(const zrn_painter_t* const p, const GLuint texture) {
	const auto gl = (zrn_gl_t*)p->backend_data;
	if (gl->quads >= MAX_QUADS || (gl->quads && gl->batch_texture != texture)) flush(gl);
	if (gl->quads && memcmp(gl->batch_clip, p->clip, sizeof p->clip) != 0) flush(gl);
	if (!gl->batch_clip_set || memcmp(gl->batch_clip, p->clip, sizeof p->clip) != 0) {
		memcpy(gl->batch_clip, p->clip, sizeof p->clip);
		gl->batch_clip_set = 1;
		glEnable(GL_SCISSOR_TEST);
		glScissor(p->clip[0], gl->height - p->clip[3], p->clip[2] - p->clip[0] > 0 ? p->clip[2] - p->clip[0] : 0, p->clip[3] - p->clip[1] > 0 ? p->clip[3] - p->clip[1] : 0);
	}
	gl->batch_texture = texture;
	return gl->verts + (size_t)gl->quads * 6 * FLOATS;
}

typedef struct corner {
	float x, y, u, v;
} corner_t;

static void put(float* const out, const float x, const float y, const float lx, const float ly, const float hx, const float hy, const float radii[4], const float aux, const float mode, const ZColor c, const float alpha, const float u, const float v) {
	out[0] = x; out[1] = y; out[2] = lx; out[3] = ly; out[4] = hx; out[5] = hy;
	out[6] = radii[0]; out[7] = radii[1]; out[8] = radii[2]; out[9] = radii[3];
	out[10] = aux; out[11] = mode;
	out[12] = c.r; out[13] = c.g; out[14] = c.b; out[15] = c.a * alpha;
	out[16] = u; out[17] = v;
}

/* A device rectangle [x0,x1] x [y0,y1] whose local coordinates are centred on (cx, cy). */
static void quad(zrn_painter_t* p, const GLuint texture, const float x0, const float y0, const float x1, const float y1, const float cx, const float cy, const float hx, const float hy,
		const float radii[4], const float aux, const float mode, const ZColor c, const float u0, const float v0, const float u1, const float v1) {
	float* const o = reserve(p, texture);
	const auto gl = (zrn_gl_t*)p->backend_data;
	const float a = p->alpha;
#define V(i, X, Y, U, V_) put(o + (i) * FLOATS, X, Y, (X) - cx, (Y) - cy, hx, hy, radii, aux, mode, c, a, U, V_)
	V(0, x0, y0, u0, v0); V(1, x1, y0, u1, v0); V(2, x1, y1, u1, v1);
	V(3, x0, y0, u0, v0); V(4, x1, y1, u1, v1); V(5, x0, y1, u0, v1);
#undef V
	gl->quads++;
}

static constexpr float NO_RADII[4] = { 0, 0, 0, 0 };

static inline float dx(const zrn_painter_t* p, const float x) { return p->tx + x * p->scale; }
static inline float dy(const zrn_painter_t* p, const float y) { return p->ty + y * p->scale; }


/* ============================================================================
 *  the backend
 * ============================================================================ */

static void gl_paint_clear(zrn_painter_t* const p, const ZColor c) {
	const auto gl = (zrn_gl_t*)p->backend_data;
	flush(gl);
	glDisable(GL_SCISSOR_TEST);
	gl->batch_clip_set = 0;
	glClearColor(c.r, c.g, c.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void gl_fill_rect(zrn_painter_t* const p, const ZRect r, const ZColor c) {
	/* edges on whole device pixels, as the software painter does */
	const float x0 = floorf(dx(p, r.x) + 0.5f), y0 = floorf(dy(p, r.y) + 0.5f);
	const float x1 = floorf(dx(p, r.x + r.width) + 0.5f), y1 = floorf(dy(p, r.y + r.height) + 0.5f);
	if (x1 <= x0 || y1 <= y0) return;
	quad(p, 0, x0, y0, x1, y1, 0, 0, 0, 0, NO_RADII, 0, MODE_SOLID, c, 0, 0, 0, 0);
}

static void gl_fill_rrect(zrn_painter_t* const p, const ZRect r, const ZCornerRadius radius, const ZColor c) {
	float x0, y0, x1, y1, rad[4];
	if (radius.top_left <= 0 && radius.top_right <= 0 && radius.bottom_right <= 0 && radius.bottom_left <= 0) {
		gl_fill_rect(p, r, c);
		return;
	}
	x0 = dx(p, r.x); y0 = dy(p, r.y); x1 = dx(p, r.x + r.width); y1 = dy(p, r.y + r.height);
	rad[0] = radius.top_left * p->scale; rad[1] = radius.top_right * p->scale;
	rad[2] = radius.bottom_right * p->scale; rad[3] = radius.bottom_left * p->scale;
	quad(p, 0, x0 - 1, y0 - 1, x1 + 1, y1 + 1, (x0 + x1) * 0.5f, (y0 + y1) * 0.5f, (x1 - x0) * 0.5f, (y1 - y0) * 0.5f, rad, 0, MODE_RRECT, c, 0, 0, 0, 0);
}

static void ellipse_quad(zrn_painter_t* const p, const ZRect r, const float stroke, const ZColor c) {
	const float x0 = dx(p, r.x), y0 = dy(p, r.y), x1 = dx(p, r.x + r.width), y1 = dy(p, r.y + r.height);
	const float rx = zrn_maxf((x1 - x0) * 0.5f, 0.01f), ry = zrn_maxf((y1 - y0) * 0.5f, 0.01f);
	quad(p, 0, x0 - 1, y0 - 1, x1 + 1, y1 + 1, (x0 + x1) * 0.5f, (y0 + y1) * 0.5f, rx, ry, NO_RADII, stroke, stroke > 0 ? MODE_RING : MODE_ELLIPSE, c, 0, 0, 0, 0);
}

static void gl_fill_ellipse(zrn_painter_t* p, const ZRect r, const ZColor c) { ellipse_quad(p, r, 0, c); }
static void gl_stroke_ellipse(zrn_painter_t* p, const ZRect r, const float thickness, const ZColor c) { ellipse_quad(p, r, thickness * p->scale, c); }

/* A polygon by ear clipping, in device pixels: the fill of a simple polygon, convex or not. */
static void gl_fill_polygon(zrn_painter_t* const p, const float* const xy, const int count, const ZColor c) {
	float pts[64][2];
	int idx[64], n = count, i, guard = 0;
	float area = 0;
	if (count < 3 || count > 64) return;
	for (i = 0; i < count; i++) {
		pts[i][0] = dx(p, xy[i * 2]);
		pts[i][1] = dy(p, xy[i * 2 + 1]);
		idx[i] = i;
	}
	for (i = 0; i < count; i++) {
		int j = (i + 1) % count;
		area += pts[i][0] * pts[j][1] - pts[j][0] * pts[i][1];
	}
	while (n > 3 && guard++ < 256) {
		int ear = -1;
		for (i = 0; i < n && ear < 0; i++) {
			int a = idx[(i + n - 1) % n], b = idx[i], cc = idx[(i + 1) % n], k, inside = 0;
			float cross = (pts[b][0] - pts[a][0]) * (pts[cc][1] - pts[a][1]) - (pts[b][1] - pts[a][1]) * (pts[cc][0] - pts[a][0]);
			if ((cross > 0) != (area > 0) || cross == 0) continue;
			for (k = 0; k < n && !inside; k++) {
				int t = idx[k];
				float d0, d1, d2;
				if (t == a || t == b || t == cc) continue;
				d0 = (pts[b][0] - pts[a][0]) * (pts[t][1] - pts[a][1]) - (pts[b][1] - pts[a][1]) * (pts[t][0] - pts[a][0]);
				d1 = (pts[cc][0] - pts[b][0]) * (pts[t][1] - pts[b][1]) - (pts[cc][1] - pts[b][1]) * (pts[t][0] - pts[b][0]);
				d2 = (pts[a][0] - pts[cc][0]) * (pts[t][1] - pts[cc][1]) - (pts[a][1] - pts[cc][1]) * (pts[t][0] - pts[cc][0]);
				if ((d0 >= 0 && d1 >= 0 && d2 >= 0) || (d0 <= 0 && d1 <= 0 && d2 <= 0)) inside = 1;
			}
			if (!inside) ear = i;
		}
		if (ear < 0) ear = 0;
		{
			int a = idx[(ear + n - 1) % n], b = idx[ear], cc = idx[(ear + 1) % n];
			float* const o = reserve(p, 0);
			const auto gl = (zrn_gl_t*)p->backend_data;
			int v;
			const int vs[3] = { a, b, cc };
			for (v = 0; v < 3; v++) put(o + v * FLOATS, pts[vs[v]][0], pts[vs[v]][1], 0, 0, 0, 0, NO_RADII, 0, MODE_SOLID, c, p->alpha, 0, 0);
			/* the quad of the batch has six vertices: a degenerate second triangle fills the rest */
			for (v = 3; v < 6; v++) put(o + v * FLOATS, pts[a][0], pts[a][1], 0, 0, 0, 0, NO_RADII, 0, MODE_SOLID, c, p->alpha, 0, 0);
			gl->quads++;
		}
		for (i = ear; i < n - 1; i++) idx[i] = idx[i + 1];
		n--;
	}
	if (n == 3) {
		float* const o = reserve(p, 0);
		const auto gl = (zrn_gl_t*)p->backend_data;
		for (int v = 0; v < 6; v++) put(o + v * FLOATS, pts[idx[v % 3]][0], pts[idx[v % 3]][1], 0, 0, 0, 0, NO_RADII, 0, MODE_SOLID, c, p->alpha, 0, 0);
		gl->quads++;
	}
}

static void gl_draw_line(zrn_painter_t* const p, const float x0, const float y0, const float x1, const float y1, const float thickness, const ZColor c) {
	float ax = dx(p, x0), ay = dy(p, y0), bx = dx(p, x1), by = dy(p, y1);
	float lx = bx - ax, ly = by - ay, len = sqrtf(lx * lx + ly * ly), h = thickness * p->scale * 0.5f, ux, uy, nx, ny, mx, my, e;
	const auto gl = (zrn_gl_t*)p->backend_data;
	float corners[4][2];
	const int order[6] = { 0, 1, 2, 0, 2, 3 };
	if (len <= 0.0001f) return;
	ux = lx / len; uy = ly / len; nx = -uy; ny = ux;
	mx = (ax + bx) * 0.5f; my = (ay + by) * 0.5f;
	e = 1.0f; /* room for the soft edge */
	corners[0][0] = ax - ux * e + nx * (h + e); corners[0][1] = ay - uy * e + ny * (h + e);
	corners[1][0] = bx + ux * e + nx * (h + e); corners[1][1] = by + uy * e + ny * (h + e);
	corners[2][0] = bx + ux * e - nx * (h + e); corners[2][1] = by + uy * e - ny * (h + e);
	corners[3][0] = ax - ux * e - nx * (h + e); corners[3][1] = ay - uy * e - ny * (h + e);
	float* o = reserve(p, 0);
	for (int i = 0; i < 6; i++) {
		float x = corners[order[i]][0], y = corners[order[i]][1];
		put(o + i * FLOATS, x, y, (x - mx) * ux + (y - my) * uy, (x - mx) * nx + (y - my) * ny, len * 0.5f, h, NO_RADII, 0, MODE_RRECT, c, p->alpha, 0, 0);
	}
	gl->quads++;
}

/* The atlas slot of a glyph bitmap, uploading it the first time. */
static atlas_slot_t* atlas_find(zrn_gl_t* const gl, const uint8_t* const alpha, const int w, const int h) {
	atlas_slot_t* s;
	for (size_t i = 0; i < gl->slot_count; i++) {
		s = &gl->slots[i];
		if (s->key == alpha && s->w == w && s->h == h) return s;
	}
	if (gl->shelf_x + w + 1 > ATLAS_SIZE) {
		gl->shelf_x = 0;
		gl->shelf_y += gl->shelf_h + 1;
		gl->shelf_h = 0;
	}
	if (gl->shelf_y + h + 1 > ATLAS_SIZE) return nullptr;
	if (gl->slot_count == gl->slot_cap) {
		const size_t cap = gl->slot_cap ? gl->slot_cap * 2 : 512;
		atlas_slot_t* slots = (atlas_slot_t*)realloc(gl->slots, cap * sizeof *slots);
		if (!slots) return nullptr;
		gl->slots = slots;
		gl->slot_cap = cap;
	}
	s = &gl->slots[gl->slot_count++];
	s->key = alpha; s->w = w; s->h = h; s->x = gl->shelf_x; s->y = gl->shelf_y;
	gl->shelf_x += w + 1;
	if (h > gl->shelf_h) gl->shelf_h = h;
	glBindTexture(GL_TEXTURE_2D, gl->atlas);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, s->x, s->y, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, alpha);
	return s;
}

static void gl_blit_alpha(zrn_painter_t* const p, const int x, const int y, const int w, const int h, const uint8_t* const alpha, const int stride, const ZColor c) {
	const auto gl = (zrn_gl_t*)p->backend_data;
	constexpr float inv = 1.0f / (float)ATLAS_SIZE;
	if (w <= 0 || h <= 0 || stride != w) return;
	atlas_slot_t* s = atlas_find(gl, alpha, w, h);
	if (!s) {
		/* the atlas is full: start it again */
		flush(gl);
		gl->slot_count = 0;
		gl->shelf_x = gl->shelf_y = gl->shelf_h = 0;
		s = atlas_find(gl, alpha, w, h);
		if (!s) return;
	}
	quad(p, 0, (float)x, (float)y, (float)(x + w), (float)(y + h), 0, 0, 0, 0, NO_RADII, 0, MODE_GLYPH, c,
		(float)s->x * inv, (float)s->y * inv, (float)(s->x + w) * inv, (float)(s->y + h) * inv);
}

static GLuint image_texture(zrn_gl_t* const gl, const zrn_image_t* const image) {
	const auto im = (zrn_image_t*)image;
	GLuint t;
	if (im->gl_owner == gl) return im->gl_texture;
	if (gl->image_count == gl->image_cap) {
		const int cap = gl->image_cap ? gl->image_cap * 2 : 16;
		zrn_image_t** images = (zrn_image_t**)realloc(gl->images, (size_t)cap * sizeof *images);
		if (!images) return 0;
		gl->images = images;
		gl->image_cap = cap;
	}
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, im->width, im->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, im->rgba);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	im->gl_texture = t;
	im->gl_owner = gl;
	gl->images[gl->image_count++] = im;
	return t;
}

static void gl_draw_image(zrn_painter_t* const p, const zrn_image_t* const image, const ZRect dest, const ZRect source) {
	const auto gl = (zrn_gl_t*)p->backend_data;
	const float x0 = dx(p, dest.x), y0 = dy(p, dest.y), x1 = dx(p, dest.x + dest.width), y1 = dy(p, dest.y + dest.height);
	const ZColor white = { 1, 1, 1, 1 };
	if (!image || x1 <= x0 || y1 <= y0) return;
	flush(gl);
	GLuint t = image_texture(gl, image);
	if (!t) return;
	quad(p, t, x0, y0, x1, y1, 0, 0, 0, 0, NO_RADII, 0, MODE_IMAGE, white,
		source.x / (float)image->width, source.y / (float)image->height,
		(source.x + source.width) / (float)image->width, (source.y + source.height) / (float)image->height);
	flush(gl);
}

/* An affine transform of device pixels (z toward the viewer), seen from an eye a 34 degree field of view away: the matrix of the shader has the perspective in it. */
static void gl_set_transform(zrn_painter_t* const p, const float* const m) {
	const auto gl = (zrn_gl_t*)p->backend_data;
	float persp[16], eye = ((float)gl->height * 0.5f) / tanf(17.0f * ZST_PI / 180.0f);
	flush(gl);
	if (!m) {
		zrn_xf_identity(gl->xf);
		return;
	}
	zrn_xf_identity(persp);
	persp[11] = -1.0f / eye;
	zrn_xf_mul(gl->xf, persp, m);
}

const zrn_backend_t zrn_backend_gles = {
	.name = "gles2",
	.paint_clear = gl_paint_clear,
	.fill_rect = gl_fill_rect,
	.fill_rrect = gl_fill_rrect,
	.fill_ellipse = gl_fill_ellipse,
	.stroke_ellipse = gl_stroke_ellipse,
	.fill_polygon = gl_fill_polygon,
	.draw_line = gl_draw_line,
	.blit_alpha = gl_blit_alpha,
	.draw_image = gl_draw_image,
	/* the clip is the painter's own state: only the scissor follows it */
	.push_clip = zrn_clip_push,
	.pop_clip = zrn_clip_pop,
	.clipped_out = zrn_clip_out,
	.set_transform = gl_set_transform,
};


/* ============================================================================
 *  a frame
 * ============================================================================ */

void zrn_gl_begin(zrn_gl_t* const gl, zrn_painter_t* const p, const int width, const int height, const float scale) {
	eglMakeCurrent(gl->display, gl->surface, gl->surface, gl->context);
	if (width != gl->width || height != gl->height) zrn_gl_resize(gl, width, height);
	if (gl->offscreen) glBindFramebuffer(GL_FRAMEBUFFER, gl->fbo);
	memset(p, 0, sizeof *p);
	p->be = &zrn_backend_gles;
	p->backend_data = gl;
	p->width = width;
	p->height = height;
	p->scale = scale;
	p->alpha = 1.0f;
	p->clip[2] = width;
	p->clip[3] = height;
	zrn_xf_identity(p->xf);
	zrn_xf_identity(gl->xf);
	gl->batch_clip_set = 0;
	gl->batch_texture = 0;
	gl->quads = 0;
	glViewport(0, 0, width, height);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(0x809D); /* GL_MULTISAMPLE */
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
}

void zrn_gl_end(zrn_gl_t* const gl, const int swap) {
	flush(gl);
	glDisable(GL_SCISSOR_TEST);
	if (swap && !gl->offscreen) eglSwapBuffers(gl->display, gl->surface);
}

void zrn_gl_swap(zrn_gl_t* const gl) {
	if (gl && !gl->offscreen) eglSwapBuffers(gl->display, gl->surface);
}

int zrn_gl_read(zrn_gl_t* const gl, uint8_t* const rgba, const int width, const int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, gl->offscreen ? gl->fbo : 0); /* the window swaps through the default framebuffer */
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	/* GL's rows run bottom to top */
	uint8_t* row = (uint8_t*)malloc((size_t)width * 4);
	if (!row) return 0;
	for (int y = 0; y < height / 2; y++) {
		uint8_t* const a = rgba + (size_t)y * (size_t)width * 4;
		uint8_t* const b = rgba + (size_t)(height - 1 - y) * (size_t)width * 4;
		memcpy(row, a, (size_t)width * 4);
		memcpy(a, b, (size_t)width * 4);
		memcpy(b, row, (size_t)width * 4);
	}
	free(row);
	return 1;
}

int zrn_gl_available(void) { return 1; }

#else /* no OpenGL ES at build time */

zrn_gl_t* zrn_gl_create_offscreen(int width, int height) { (void)width; (void)height; return nullptr; }
zrn_gl_t* zrn_gl_create_window(void* const wl_display, void* const wl_surface, const int width, const int height) {
	(void)wl_display; (void)wl_surface; (void)width; (void)height;
	return nullptr;
}
void zrn_gl_resize(zrn_gl_t* gl, int width, int height) { (void)gl; (void)width; (void)height; }
void zrn_gl_destroy(zrn_gl_t* gl) { (void)gl; }
void zrn_gl_begin(zrn_gl_t* gl, zrn_painter_t* p, int width, int height, float scale) { (void)gl; (void)p; (void)width; (void)height; (void)scale; }
void zrn_gl_end(zrn_gl_t* gl, int swap) { (void)gl; (void)swap; }
void zrn_gl_swap(zrn_gl_t* gl) { (void)gl; }
int zrn_gl_read(zrn_gl_t* gl, uint8_t* rgba, int width, int height) { (void)gl; (void)rgba; (void)width; (void)height; return 0; }
void zrn_gl_forget_image(zrn_image_t* image) { (void)image; }
int zrn_gl_available(void) { return 0; }

#endif
