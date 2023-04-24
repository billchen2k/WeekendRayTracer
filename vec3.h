//
// Created by Bill Chen on 2023/4/21.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <iostream>
#include "rtweekend.h"

using std::sqrt;

class vec3 {
public:
    vec3() : e{0, 0, 0} {}

    vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    float x() const { return e[0]; }

    float y() const { return e[1]; }

    float z() const { return e[2]; }

    float r() const { return e[0]; }

    float g() const { return e[1]; }

    float b() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    float operator[](int i) const { return e[i]; }

    // Used for assignment
    float &operator[](int i) { return e[i]; }

    vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const float t) {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    float length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    float length() const {
        return sqrt(length_squared());
    }

    inline static vec3 rand() {
        return vec3(rand_float(), rand_float(), rand_float());
    }

    inline static vec3 rand(float min, float max) {
        return vec3(rand_float(min, max), rand_float(min, max), rand_float(min, max));
    }

    bool near_zero() const {
        const auto s = 1e-5;
        return (fabs(e[0] < s) && fabs(e[1]) < s && (fabs(e[2])) < s);
    }

private:
    float e[3];
};

using point3 = vec3;    // 3D point
using color = vec3;     // RGB colors

// vec3 utilities

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator/(vec3 v, float t) {
    return (1 / t) * v;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

/**
 * Cross product returns a vector that is perpendicular to u and v.
 * @param u
 * @param v
 * @return vec3
 */
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline vec3 unit_vec(vec3 v) {
    return v / v.length();
}

/**
 * @return A random vec3 inside the unit sphere (squared length <= 1)
 */
vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::rand(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 random_unit_vec() {
    return unit_vec(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3 &norm) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, norm) > 0.0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(rand_float(-1, 1), rand_float(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

/**
 *
 * @param v Incoming ray
 * @param n Unit vector of surface norm
 * @return The reflected vector B.
 */
vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

// TODO
vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif //RAYTRACER_VEC3_H