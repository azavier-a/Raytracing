#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_p {
  point3 p;
  vec3 n;
  double t;
  bool front_face;

  inline void set_face_normal(const ray& r, const vec3& outward_normal) {
    front_face = dot(r.dir(), outward_normal) < 0.0;
    n = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual bool hit(const ray& r, double t_min, double t_max, hit_p& rec) const = 0;
};

#endif