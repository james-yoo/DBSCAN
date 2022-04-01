cmake_minimum_required(VERSION 3.0.0)
project(DBSCAN LANGUAGES C CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -lstdc++fs -O3 -ffast-math -Wall -march=native -mtune=native")


add_library(DBSCAN DBSCAN.cpp)

add_executable(example main.cpp)
