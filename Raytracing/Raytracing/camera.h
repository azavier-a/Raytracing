#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

class camera {
public:
  camera(point3 from, point3 foc, vec3 vup, double vfov, double asp, double apt, double foc_d) {
    auto theta = radians(vfov);
    auto h = tan(theta/2.0);
    auto vp_h = 2.0 * h;
    auto vp_w = asp * vp_h;

    w = normalize(from - foc);
    u = normalize(cross(vup, w));
    v = cross(w, u);

    origin = from;
    hor = foc_d * vp_w * u;
    ver = foc_d * vp_h * v;
    lower_left = origin - hor / 2.0 - ver / 2.0 - foc_d * w;

    lens_rad = apt / 2.0;
  }

  ray get_ray(double s, double t) const {
    vec3 rd = lens_rad * rand_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();

    return ray(origin + offset, lower_left + s * hor + t * ver - origin - offset);
  }
private:
  point3 origin;
  point3 lower_left;
  vec3 hor;
  vec3 ver;
  vec3 u, v, w;
  double lens_rad;
};

#endif