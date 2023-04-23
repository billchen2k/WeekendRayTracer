#include <iostream>
#include <fstream>
#include <chrono>

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

float hit_sphere(const point3 &center, float radius, const ray &r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminator = half_b * half_b - a * c;
    if (discriminator < 0) {
        return -1;
    } else {
        return (-half_b - sqrt(discriminator)) / a;
    }
}

color ray_color(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    if (depth <= 0) {
        return color(0, 0, 0);
    }

    if (world.hit(r, 0.001, inf, rec)) {
        //  Three diffuse methods:
        //  + random_in_unit_sphere()
        //  + random_unit_vec()
        //  + random_in_unit_sphere(rec.norm)
        point3 target = rec.p + rec.norm + random_in_hemisphere(rec.norm);
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }

    // Rendering the background gradinet.
    // y for unit direction: [-1, 1]
    vec3 unit_direction = unit_vec(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    std::ofstream fout("out/image.ppm");

    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;    // Used for antialiasing
    const int max_depth = 50;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.8));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);


    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));

    // Hollow sphere
//    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));

    // Camera
    camera cam;

    // ppm file header
    fout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    auto start_time = std::chrono::steady_clock::now();
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + rand_float()) / (image_width - 1);
                auto v = (j + rand_float()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(fout, pixel_color, samples_per_pixel);
        }
    }
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    fout.close();
    std::cerr << "\nDone in " << double(duration) / 1000 << "s.\n";
    return 0;
}