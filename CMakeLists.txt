cmake_minimum_required(VERSION 3.25)
project(RayTracer)

set(CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-12)
set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracer main.cc vec3.h color.h)