#include <fstream>
#include <iostream>

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"

// #include "color.h"
// #include "ray.h"
// #include "vec3.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

// bool hit_ellipsoid(const point3 &center, double r1, double r2, double r3,
// const ray &r)
// {
//     vec3 oc = r.origin() - center;
//     auto a = ellipsoid(r.direction(), r1, r2, r3);
//     auto b = 2.0 * ellipsoid(oc, r1, r2, r3);
//     auto c = ellipsoid(oc, r1, r2, r3) - 1;
//     auto discriminant = b * b - 4 * a * c;
//     return (discriminant > 0);
// }

// color ray_color(const ray &r) {
//     // if (hit_ellipsoid(point3(0, 0, -1), 0.2, 0.5, 0.2, r))
//     //     return color(1, 0, 0);

//     auto t = hit_sphere(point3(0.5, -0.5, -1), 0.5, r);
//     if (t > 0.0) {
//         vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
//         return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
//     }
//     vec3 unit_direction = unit_vector(r.direction());
//     t = 0.5 * (unit_direction.y() + 1.0);
//     return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
// }
	
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image output
    std::ofstream ofs{"D:\\dev\\c++\\RayTracingInAWeekend\\test.ppm"};
    // auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(ofs.rdbuf());

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 900;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    // std::cout.rdbuf(cout_buff);
}