//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H
#include "rtweekend.h"

class material;

class sphere : public hittable {
public:
    sphere() {}

    sphere(point3 cen, float r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;

public:
    point3 center;
    float radius;
    shared_ptr<material> mat_ptr;
};


bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminator = half_b * half_b - a * c;
    if (discriminator < 0) {
        return false;
    }

    auto sqrtd = sqrt(discriminator);
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / 2;
        if (root < t_min || root > t_min) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_norm = (rec.p - center) / radius;
    rec.set_face_norm(r, outward_norm);
    rec.mat_ptr = mat_ptr;
    return true;
}

#endif //RAYTRACER_SPHERE_H
