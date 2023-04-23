//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

class material;

struct hit_record {
    point3 p;
    vec3 norm;
    shared_ptr<material> mat_ptr;
    float t;
    bool front_face;

    inline void set_face_norm(const ray& r, const vec3& outward_norm) {
        front_face = dot(r.direction(), outward_norm) < 0;
        norm = front_face ? outward_norm : -outward_norm;
    }
};

class hittable {
public:
    /**
     * If the ray in the acceptable range hits the object.
     * @param r Ray
     * @param t_min Acceptable range min.
     * @param t_max  Acceptable range max.
     * @param rec Reference to the hit record.
     * @return If the ray hits.
     */
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};


#endif //RAYTRACER_HITTABLE_H
