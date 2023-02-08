#include "util.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world) {
  hit_p rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 + rec.n*0.5;
  }
  vec3 dir = normalize(r.dir());
  auto t = 0.5 * (dir.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // Simple gradient bg
}

int main() {

  // Image
  const auto asp = 16.0 / 9.0;
  const int img_wdt = 400;
  const int img_hgt = static_cast<int>(img_wdt / asp);
  const int AA_samples = 100;

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  camera cam(asp);

  // Render to ppm file
  std::cout << "P3\n" << img_wdt << ' ' << img_hgt << "\n255\n";

  for (int j = img_hgt - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < img_wdt; ++i) {
      color pixel_color(0.0,0.0,0.0);

      for (int s = 0; s < AA_samples; ++s) {
        auto u = (i + random_double()) / (img_wdt - 1);
        auto v = (j + random_double()) / (img_hgt - 1);

        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world);
      }

      write_color(std::cout, pixel_color, AA_samples);
    }
  }

  std::cerr << "\nDone.\n";
}