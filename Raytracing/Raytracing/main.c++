#include "util.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world) {
  hit_p rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 * (rec.n + color(1, 1, 1));
  }
  vec3 unit_direction = normalize(r.dir());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

  // Image
  const auto asp = 16.0 / 9.0;
  const int img_wdt = 400;
  const int img_hgt = static_cast<int>(img_wdt / asp);

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Camera
  auto vp_h = 2.0;
  auto vp_w = asp * vp_h;
  auto focal_length = 1.0;

  auto origin = point3(0, 0, 0);
  auto hor = vec3(vp_w, 0, 0);
  auto ver = vec3(0, vp_h, 0);
  auto lower_left_corner = origin - hor / 2 - ver / 2 - vec3(0, 0, focal_length);

  // Render to ppm file
  std::cout << "P3\n" << img_wdt << ' ' << img_hgt << "\n255\n";

  for (int j = img_hgt - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < img_wdt; ++i) {
      auto u = double(i) / (img_wdt - 1);
      auto v = double(j) / (img_hgt - 1);
      ray r(origin, lower_left_corner + u * hor + v * ver);
      color pixel_color = ray_color(r, world);
      write_color(std::cout, pixel_color);
    }
  }

  std::cerr << "\nDone.\n";
}