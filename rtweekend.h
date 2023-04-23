//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_RTWEEKEND_H
#define RAYTRACER_RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const float inf = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932384626;

inline float deg_to_rad(float degrees) {
    return degrees * pi / 180;
}

/**
 * @return A random float number between 0.0 - 1.0
 */
inline float rand_float() {
    static std::uniform_real_distribution<float> dist(0.0, 1.0);
    static std::mt19937 generator;
    return dist(generator);
}

inline float rand_float(float min, float max) {
    return min + (max - min) * rand_float();
}

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#include "ray.h"
#include "vec3.h"

#endif //RAYTRACER_RTWEEKEND_H
