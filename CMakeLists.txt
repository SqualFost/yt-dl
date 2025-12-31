cmake_minimum_required(VERSION 3.15)
project(LoadTube C)

set(CMAKE_C_STANDARD 11)

# Find SDL3 and SDL3_ttf
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(LoadTube main.c)

file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(LoadTube PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)