//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
public:
    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color &a) : albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        auto scatter_direction = random_unit_vec() + rec.norm;

        // Cache degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.norm;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal : public material {
public:
    metal(const color &a, float f) : albedo(a), fuzz(f) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        vec3 reflected = reflect(r_in.direction(), unit_vec(rec.norm));
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.norm) > 0);
    }

public:
    color albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
        vec3 unit_dir = unit_vec(r_in.direction());

        float cos_theta = fmin(dot(-unit_dir, rec.norm), 1.0);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        vec3 direction;
        if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > rand_float()) {
            // Reflect
            direction = reflect(unit_dir, rec.norm);
        } else {
            // Refract
            direction = refract(unit_dir, unit_vec(rec.norm), refraction_ratio);
        }
        scattered = ray(rec.p, direction);
        return true;
    }

public:
    float ir; // Index of Refraction

private:
    static float reflectance(float cosine, float ref_idx) {
        // Schlick's approximation
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif //RAYTRACER_MATERIAL_H
