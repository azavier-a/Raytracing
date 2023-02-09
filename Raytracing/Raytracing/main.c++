#include "util.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
  hit_p rec;

  if (depth <= 0)
    return color(0.0, 0.0, 0.0);

  if (world.hit(r, 0.001, infinity, rec)) {
    ray scattered;
    color atten;

    if (rec.mat_ptr->scatter(r, rec, atten, scattered)) {
      return atten * ray_color(scattered, world, depth-1);
    }

    return color(0.0, 0.0, 0.0);
  }

  vec3 dir = normalize(r.dir());
  auto t = 0.5 * (dir.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // Simple gradient bg
}

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
           world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {

  // Image
  const auto asp = 3.0 / 2.0;
  const int img_wdt = 400;
  const int img_hgt = static_cast<int>(img_wdt / asp);
  const int samples = 100;
  const int bounces = 50;

  // World
  hittable_list world = random_scene();

  // Camera
  point3 from(13.0, 2.0, 3.0);
  point3 focus(0.0, 0.0, 0.0);
  vec3 vup(0.0, 1.0, 0.0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;

  camera cam(from, focus, vup, 20.0, asp, aperture, dist_to_focus);

  // Render to ppm file
  std::cout << "P3\n" << img_wdt << ' ' << img_hgt << "\n255\n";

  for (int j = img_hgt - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < img_wdt; ++i) {
      color pixel_color(0.0,0.0,0.0);

      for (int s = 0; s < samples; ++s) {
        auto u = (i + random_double()) / (img_wdt - 1);
        auto v = (j + random_double()) / (img_hgt - 1);

        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, bounces);
      }

      write_color(std::cout, pixel_color, samples);
    }
  }

  std::cerr << "\nDone.\n";
}