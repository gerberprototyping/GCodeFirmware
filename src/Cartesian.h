#ifndef __CARTESIAN_H
#define __CARTESIAN_H

#include <cmath>
#include <cstdint>
#include "Config.h"


template<typename T>
struct Cartesian {

  T x, y, z;

  Cartesian() {}
  Cartesian(const T &x, const T &y, const T &z);
  Cartesian(const T &magnitude, const Cartesian<T> &direction);
  Cartesian(const Cartesian &c);

  const T getMagnitude();
  const bool isNormalized();

  Cartesian<T> normalize();
  Cartesian<T> operator+(const Cartesian<T> &c);
  Cartesian<T> operator-(const Cartesian<T> &c);
  Cartesian<T> operator*(const double &scalar);
  Cartesian<T> operator/(const double &scalar);
  const bool operator==(const Cartesian<T> &c);

};


template<typename T>
Cartesian<T> operator*(const double &scalar, const Cartesian<T> &c);

template<typename T>
Cartesian<T> operator/(const double &scalar, const Cartesian<T> &c);


struct Point: public Cartesian<int32_t> {
  Point();
  const Cartesian<double> toMM();
  const Cartesian<int32_t> toSteps();
  static Point fromMM(const double &x, const double &y, const double &z);
  static Point fromSteps(const int32_t &x, const int32_t &y, const int32_t &z);
  private:
    Point(const int32_t &x, const int32_t &y, const int32_t &z);
};

struct Velocity: Cartesian<double> {
  Velocity();
  using Cartesian::Cartesian;
};


#endif