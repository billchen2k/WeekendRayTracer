//
// Created by Bill Chen on 2023/4/22.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "rtweekend.h"

class camera {

public:
    camera() {
        float aspect_ratio = 16.0 / 9.0;
        float viewport_height = 2.0;
        float viewport_width = aspect_ratio * viewport_height;
        float focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
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
