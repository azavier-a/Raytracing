#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color, int samples) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples.
  auto scale = 1.0 / samples;
  r *= scale;
  g *= scale;
  b *= scale;
  
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(256 * sat(r)) << ' '
      << static_cast<int>(256 * sat(g)) << ' '
      << static_cast<int>(256 * sat(b)) << '\n';
}

#endif
