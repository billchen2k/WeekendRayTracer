#include <iostream>
#include <fstream>
#include <chrono>

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"


color ray_color(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    if (depth <= 0) {
        return color(0, 0, 0);
    }

    if (world.hit(r, 0.001, inf, rec)) {
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

hittable_list world_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto mat = rand_float();
            point3 center(a + 0.9 * rand_float(), 0.2, b + 0.9 * rand_float());

            // If the center allows space for large spheres.
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                if (mat < 0.7) {
                    // Lambertian
                    auto albedo = color::rand() * color::rand();
                    sphere_material = make_shared<lambertian>(albedo);
                } else if (mat < 0.90) {
                    // Metal
                    auto albedo = color::rand(0.5, 1);
                    auto fuzz = rand_float(0.5, 1);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                } else {
                    // Glass
                    sphere_material = make_shared<dielectric>(1.5);
                }
                world.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }

    auto mat1 = make_shared<dielectric>(1.5);
    auto mat2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto mat3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);

    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, mat1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, mat2));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, mat3));
    return world;
}

int main() {
    std::ofstream fout("out/image.ppm");

    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 120;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;    // Used for antialiasing
    const int max_depth = 50;

    // World
    hittable_list world = world_scene();

//    hittable_list world;
//
//    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.8));
//    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
//    auto material_left = make_shared<dielectric>(1.5);
//    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
//
//
//    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
//    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
//    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
//    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));
//    // Hollow sphere, making left sphere functions like a bubble.
//    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));

    // Camera
    point3 lookfrom(13, 2, 4);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);

    // ppm file header
    fout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    auto start_time = std::chrono::steady_clock::now();
    for (int j = image_height - 1; j >= 0; --j) {
        int pixelsDone = (image_height - j) * image_width;
        float pps = pixelsDone / (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() / 1000.0);
        float eta = (image_height * image_width - pixelsDone) / pps;
        std::cerr << "\rScanlines remaining: " << j << ", Pixels/s: " << pps << ", ETA: " << eta << "s" << std::flush;
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