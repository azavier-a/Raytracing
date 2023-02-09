#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

/* A material's job is to:
  * Produce a scattered ray, or say it absorbed the incident ray.
  * If scattered, say how much the ray should be attenuated.
*/
class material {
public:
  virtual bool scatter(const ray& r_in, const hit_p& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_p& rec, color& attenuation, ray& scattered) const override {
    auto scatter_direction = rec.n + rand_unit();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.n;

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

public:
  color albedo;
};

class metal : public material {
public:
  metal(const color& a, double r) : albedo(a), rough(r < 1.0 ? r : 1.0) {}

  virtual bool scatter(const ray& r_in, const hit_p& rec, color& attenuation, ray& scattered) const override {
    vec3 reflected = reflect(normalize(r_in.dir()), rec.n);
    scattered = ray(rec.p, reflected + rough*rand_unit_sphere());
    attenuation = albedo;

    return (dot(scattered.dir(), rec.n) > 0);
  }

public:
  color albedo;
  double rough;
};

class dielectric : public material {
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const ray& r_in, const hit_p& rec, color& attenuation, ray& scattered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = normalize(r_in.dir());
    double cos_theta = fmin(dot(-unit_direction, rec.n), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
      direction = reflect(unit_direction, rec.n);
    else
      direction = refract(unit_direction, rec.n, refraction_ratio);

    scattered = ray(rec.p, direction);
    return true;
  }
private:
  static double reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
public:
    double ir; // Index of Refraction
};

#endif