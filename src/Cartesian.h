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
  Cartesian(const Cartesian<T> &c);
  Cartesian(const volatile Cartesian<T> &c);
  void operator=(const volatile Cartesian<T> &c) volatile;

  template<typename T2>
  Cartesian(const Cartesian<T2> &c);

  T getMagnitude() const;
  bool isNormalized() const;

  Cartesian<T> abs() const;
  Cartesian<T> normalize() const;
  Cartesian<T> operator+(const T &x) const;
  Cartesian<T> operator-(const T &x) const;
  Cartesian<T> operator+(const Cartesian<T> &c) const;
  Cartesian<T> operator-(const Cartesian<T> &c) const;
  Cartesian<T> operator*(const double &scalar) const;
  Cartesian<T> operator/(const double &scalar) const;
  void operator+=(const Cartesian<T> &c);
  void operator-=(const Cartesian<T> &c);
  void operator*=(const double &scalar);
  void operator/=(const double &scalar);
  bool operator==(const Cartesian<T> &c) const;

};


template<typename T>
Cartesian<T> operator+(const T &a, const Cartesian<T> &c);

template<typename T>
Cartesian<T> operator-(const T &a, const Cartesian<T> &c);

template<typename T>
Cartesian<T> operator*(const double &scalar, const Cartesian<T> &c);

template<typename T>
Cartesian<T> operator/(const double &scalar, const Cartesian<T> &c);




// Contains positional data stored as steps from origin
struct Point: public Cartesian<int32_t> {

  public:

    Point();
    Point(const Point &p);
    Point(const volatile Point &p);
    void operator=(const volatile Point &p) volatile;
    static Point zero();
    static Point fromMM(Cartesian<double> c);
    static Point fromMM(double x, double y, double z);
    static Point fromSteps(Cartesian<int32_t> c);
    static Point fromSteps(int32_t x, int32_t y, int32_t z);

    Cartesian<double> toMM() const;
    Cartesian<int32_t> toSteps() const;

    Point operator+(const Point &p) const;
    Point operator-(const Point &p) const;
    Point operator*(const double &scalar) const;
    Point operator/(const double &scalar) const;

    static int32_t fromMM(double mm);
    static int32_t fromSteps(int32_t steps);

  private:

    Point(const int32_t &x, const int32_t &y, const int32_t &z);

};




// Contains velocity data stored in mm/sec
struct Velocity: Cartesian<double> {
  Velocity();
  Velocity(const Cartesian<double> &vec);
  Velocity(const Cartesian<int32_t> &vec);
  Velocity(const volatile Cartesian<double> &vec);
  Velocity(const volatile Cartesian<int32_t> &vec);
  void operator=(const volatile Cartesian<double> &vec) volatile;
  void operator=(const volatile Cartesian<int32_t> &vec) volatile;
  using Cartesian::Cartesian;
};




#include "Cartesian_imp.hpp"


#endif