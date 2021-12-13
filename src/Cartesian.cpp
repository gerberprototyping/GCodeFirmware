#include "Cartesian.h"




Point::Point()
{
  x = 0;
  y = 0;
  z = 0;
}


Point::Point(const Point &p)
    : Cartesian(p)
{ }


Point::Point(const volatile Point &p) {
  x = *((volatile int32_t*)&p.x);
  y = *((volatile int32_t*)&p.y);
  z = *((volatile int32_t*)&p.z);
}


void Point::operator=(const volatile Point &p) volatile {
  *((volatile int32_t*)&x) = *((volatile int32_t*)&p.x);
  *((volatile int32_t*)&y) = *((volatile int32_t*)&p.y);
  *((volatile int32_t*)&z) = *((volatile int32_t*)&p.z);
}


Point Point::zero() {
  return Point(0,0,0);
}


Point Point::fromMM(Cartesian<double> c) {
  return Point(c.x*STEPS_PER_MM, c.y*STEPS_PER_MM, c.z*STEPS_PER_MM);
}


Point Point::fromMM(double x, double y, double z) {
  return Point(x*STEPS_PER_MM, y*STEPS_PER_MM, z*STEPS_PER_MM);
}


Point Point::fromSteps(Cartesian<int32_t> c) {
  return Point(c.x*STEPS_PER_MM, c.y*STEPS_PER_MM, c.z*STEPS_PER_MM);
}


Point Point::fromSteps(int32_t x, int32_t y, int32_t z) {
  return Point(x, y, z);
}


Cartesian<double> Point::toMM() const {
  double _x = ((double)x)/STEPS_PER_MM;
  double _y = ((double)y)/STEPS_PER_MM;
  double _z = ((double)z)/STEPS_PER_MM;
  return Cartesian<double>(_x, _y, _z);
}


Cartesian<int32_t> Point::toSteps() const {
  return Cartesian<int32_t>(x, y, z);
}


Point::Point(const int32_t &x, const int32_t &y, const int32_t &z)
    : Cartesian(x,y,z)
{}


Point Point::operator+(const Point &p) const {
  return Point(x+p.x, y+p.y, z+p.z);
}


Point Point::operator-(const Point &p) const {
  return Point(x-p.x, y-p.y, z-p.z);
}


Point Point::operator*(const double &scalar) const {
  return Point(x*scalar, y*scalar, z*scalar);
}

Point Point::operator/(const double &scalar) const {
  return Point(x/scalar, y/scalar, z/scalar);
}


int32_t Point::fromMM(double mm) {
  return mm*STEPS_PER_MM;
}


int32_t Point::fromSteps(int32_t steps) {
  return steps;
}








Velocity::Velocity()
    : Cartesian(0,0,0)
{}


Velocity::Velocity(const Cartesian<double> &vec)
    : Cartesian(vec)
{ }


Velocity::Velocity(const Cartesian<int32_t> &vec)
{
  x = vec.x;
  y = vec.y;
  z = vec.z;
}


Velocity::Velocity(const volatile Cartesian<double> &vec) {
  x = *((volatile double*)&vec.x);
  y = *((volatile double*)&vec.y);
  z = *((volatile double*)&vec.z);
}


Velocity::Velocity(const volatile Cartesian<int32_t> &vec) {
  x = *((volatile int32_t*)&vec.x);
  y = *((volatile int32_t*)&vec.y);
  z = *((volatile int32_t*)&vec.z);
}


void Velocity::operator=(const volatile Cartesian<double> &vec) volatile {
  *((volatile double*)&x) = *((volatile double*)&vec.x);
  *((volatile double*)&y) = *((volatile double*)&vec.y);
  *((volatile double*)&z) = *((volatile double*)&vec.z);
}


void Velocity::operator=(const volatile Cartesian<int32_t> &vec) volatile  {
  *((volatile int32_t*)&x) = *((volatile int32_t*)&vec.x);
  *((volatile int32_t*)&y) = *((volatile int32_t*)&vec.y);
  *((volatile int32_t*)&z) = *((volatile int32_t*)&vec.z);
}

