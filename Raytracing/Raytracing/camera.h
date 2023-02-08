#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

class camera {
public:
  camera(double asp) {
    auto vp_h = 2.0;
    auto vp_w = asp * vp_h;
    auto focal_length = 1.0;

    origin = point3(0, 0, 0);
    hor = vec3(vp_w, 0, 0);
    ver = vec3(0, vp_h, 0);
    lower_left = origin - hor / 2 - ver / 2 - vec3(0, 0, focal_length);
  }

  ray get_ray(double u, double v) {
    return ray(origin, lower_left + u * hor + v * ver);
  }
private:
  point3 origin;
  point3 lower_left;
  vec3 hor;
  vec3 ver;
};

#endif