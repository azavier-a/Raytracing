#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r) : center(cen), radius(r) {};

  virtual bool hit(const ray& r, double t_min, double t_max, hit_p& rec) const override;

public:
  point3 center;
  double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_p& rec) const {
  vec3 oc = r.orig() - center;
  auto a = r.dir().length_squared();
  auto b = dot(oc, r.dir());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = b * b - a * c;
  if (discriminant < 0) return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-b + sqrtd) / a;

    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);

  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);

  return true;
}

#endif