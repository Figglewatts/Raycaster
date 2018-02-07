cmake_minimum_required(VERSION 3.1)
project(Raycaster)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories(include)

file(GLOB SOURCES "src/*.c")

file(COPY "assets" DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Raycaster ${SOURCES})
target_link_libraries(Raycaster ${SDL2_LIBRARY})
target_link_libraries(Raycaster m)