//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtweekend.h"

class camera {

public:

    /**
     *
     * @param lookfrom Look from point
     * @param lookat Look at point
     * @param vup View up direction of camera
     * @param vfov  Vertical field-of view in degrees.
     * @param aspect_ratio Width / Height
     */
    camera(point3 lookfrom,
           point3 lookat,
           vec3 vup,
           float vfov, float aspect_ratio) {
        auto theta = deg_to_rad(vfov);
        auto h = tan(theta / 2.0);
        float viewport_height = 2.0 * h;
        float viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vec(lookfrom - lookat);
        auto u = unit_vec(cross(vup, w));
        auto v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
    }

    /**
     * Get the ray pointing to u [0, 1], v[0, 1] of the image.
     * @param u
     * @param v
     * @return The ray starting from the camera origin.
     */
    ray get_ray(float u, float v) const {
         return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif //RAYTRACER_CAMERA_H
