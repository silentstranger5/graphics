cmake_minimum_required(VERSION 3.10.0)

project(render VERSION 0.1.0 LANGUAGES C)

find_package(SDL3 CONFIG REQUIRED)

add_compile_options("-fsanitize=address")

add_executable(render main.c graphics.c glm.c)

target_link_libraries(render PRIVATE SDL3::SDL3)
