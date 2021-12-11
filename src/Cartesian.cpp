#include "Cartesian.h"




template<typename T>
Cartesian<T>::Cartesian(const T &x, const T &y, const T &z)
    : x(x), y(y), z(z)
{}


template<typename T>
Cartesian<T>::Cartesian(const Cartesian &c) {
  x = c.x;
  y = c.y;
  z = c.z;
}


template<typename T>
Cartesian<T>::Cartesian(const T &magnitude, const Cartesian<T> &direction) {
  x = magnitude*direction.x;
  y = magnitude*direction.y;
  z = magnitude*direction.z;
}


template<typename T>
const T Cartesian<T>::getMagnitude() {
  return std::sqrt((x^2) + (y^2) + (z^2));
}


template<typename T>
const bool Cartesian<T>::isNormalized() {
  return operator==(normalize());
}


template<typename T>
Cartesian<T> Cartesian<T>::normalize() {
  operator/(getMagnitude());
}


template<typename T>
Cartesian<T> Cartesian<T>::operator+(const Cartesian<T> &c) {
  Cartesian(x+c.x, y+c.y, z+c.z);
}


template<typename T>
Cartesian<T> Cartesian<T>::operator-(const Cartesian<T> &c) {
  Cartesian(x-c.x, y-c.y, z-c.z);
}


template<typename T>
Cartesian<T> Cartesian<T>::operator*(const double &scalar) {
  Cartesian(x*scalar, y*scalar, z*scalar);
}


template<typename T>
Cartesian<T> Cartesian<T>::operator/(const double &scalar) {
  Cartesian(x/scalar, y/scalar, z/scalar);
}


template<typename T>
const bool Cartesian<T>::operator==(const Cartesian<T> &c) {
  return (x == c.x) && (y == c.y) && (z == c.z);
}








Point::Point()
    : Cartesian(0,0,0)
{}


const Cartesian<double> Point::toMM() {
  double _x = ((double)x)/1000;
  double _y = ((double)y)/1000;
  double _z = ((double)z)/1000;
  return Cartesian<double>(_x, _y, _z);
}


const Cartesian<int32_t> Point::toSteps() {
  int32_t _x = (x * STEPS_PER_MM) / 1000;
  int32_t _y = (y * STEPS_PER_MM) / 1000;
  int32_t _z = (z * STEPS_PER_MM) / 1000;
  return Cartesian<int32_t>(_x, _y, _z);
}


Point Point::fromMM(const double &x, const double &y, const double &z) {
  return Point(x*1000, y*1000, z*1000);
}


Point Point::fromSteps(const int32_t &x, const int32_t &y, const int32_t &z) {
  return Point((1000*x)/STEPS_PER_MM, (1000*y)/STEPS_PER_MM, (1000*z)/STEPS_PER_MM);
}


Point::Point(const int32_t &x, const int32_t &y, const int32_t &z)
    : Cartesian(x,y,z)
{}


template<typename T>
Cartesian<T> operator*(const double &scalar, const Cartesian<T> &c) {
  return c * scalar;
}

template<typename T>
Cartesian<T> operator/(const double &scalar, const Cartesian<T> &c) {
  return c / scalar;
}








Velocity::Velocity()
    : Cartesian(0,0,0)
{}

