#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
  ray() {}
  ray(const point3& origin, const vec3& direction)
    : _orig(origin), _dir(direction)
  {}

  point3 orig() const { return _orig; }
  vec3 dir() const { return _dir; }

  point3 at(double t) const {
    return _orig + t * _dir;
  }

public:
  point3 _orig;
  vec3 _dir;
};

#endif