//
// Created by Bill Chen on 2023/4/21.
//

#ifndef RAYTRACER_VEC3_H
#define RAYTRACER_VEC3_H

#include <cmath>
#include <iostream>

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
    return vec3(u[0], u[1], u[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u[0], u[1], u[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline vec3 unit_vec(vec3 v) {
    return v / v.length();
}


#endif //RAYTRACER_VEC3_H
