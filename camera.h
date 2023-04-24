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
           float vfov,
           float aspect_ratio,
           float aperture,
           float focus_dist) {
        auto theta = deg_to_rad(vfov);
        auto h = tan(theta / 2.0);
        float viewport_height = 2.0 * h;
        float viewport_width = aspect_ratio * viewport_height;

        w = unit_vec(lookfrom - lookat);
        u = unit_vec(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2;
    }

    /**
     * Get the ray pointing to u [0, 1], v[0, 1] of the image.
     * @param u
     * @param v
     * @return The ray starting from the camera origin.
     */
    ray get_ray(float s, float t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = rd.x() * u + rd.y() * v;
        return ray(
                origin + offset,
                lower_left_corner + s * horizontal + t * vertical - (origin + offset)
        );
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif //RAYTRACER_CAMERA_H
