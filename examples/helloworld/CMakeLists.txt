cmake_minimum_required(VERSION 3.16)
project(helloworld C)

add_subdirectory(../../zrnapp ${CMAKE_BINARY_DIR}/zrnapp)

add_executable(helloworld hello.c)
target_link_libraries(helloworld PRIVATE zrnapp::zrnapp)