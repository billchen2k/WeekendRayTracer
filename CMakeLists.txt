cmake_minimum_required(VERSION 3.25)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

# OpenMP support
find_package(OpenMP REQUIRED)

add_executable(RayTracer main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h rtweekend.h camera.h material.h)

if(OpenMP_CXX_FOUND)
    target_link_libraries(RayTracer PUBLIC OpenMP::OpenMP_CXX)
endif ()