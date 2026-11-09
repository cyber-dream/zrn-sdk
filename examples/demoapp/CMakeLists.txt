cmake_minimum_required(VERSION 3.16)
project(demoapp C)

add_subdirectory(../../zrnui ${CMAKE_BINARY_DIR}/zrnui)

add_executable(demoapp demoapp.c
    pages/main.c
    pages/text.c
    pages/button.c
    pages/hub.c
    pages/pivot.c)
target_link_libraries(demoapp PRIVATE zrnui::zrnui)