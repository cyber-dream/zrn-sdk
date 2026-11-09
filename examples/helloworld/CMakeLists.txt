cmake_minimum_required(VERSION 3.16)
project(helloworld C)

add_subdirectory(../../zrnui ${CMAKE_BINARY_DIR}/zrnui)

add_executable(helloworld hello.c)
target_link_libraries(helloworld PRIVATE zrnui::zrnui)