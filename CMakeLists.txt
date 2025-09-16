cmake_minimum_required(VERSION 3.16)
project(SDLTest C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(SDLTest main.c)
target_link_libraries(SDLTest ${SDL2_LIBRARIES})