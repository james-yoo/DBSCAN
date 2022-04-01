cmake_minimum_required(VERSION 3.0.0)
project(DBSCAN LANGUAGES C CXX VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -lstdc++fs -O3 -ffast-math -Wall -march=native -mtune=native")


add_library(DBSCAN INTERFACE)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE DBSCAN)