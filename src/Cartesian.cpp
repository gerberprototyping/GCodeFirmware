#include "Cartesian.h"


/***********************************************************
 * Point
 **********************************************************/

Point Point::fromMM(CartesianDouble &C) {
    Point p;
    p._x = C.getX()*STEPS_PER_MM;
    p._y = C.getY()*STEPS_PER_MM;
    p._z = C.getZ()*STEPS_PER_MM;
    return p;
}

Point Point::fromMM(double x, double y, double z) {
    Point p;
    p._x = x * STEPS_PER_MM;
    p._y = y * STEPS_PER_MM;
    p._z = z * STEPS_PER_MM;
    return p;
}

Point Point::fromSteps(CartesianInt &C) {
    return Point(C.getX(), C.getY(), C.getZ());
}

Point Point::fromSteps(int64_t x, int64_t y, int64_t z) {
    return Point(x, y, z);
}


CartesianDouble Point::toMM() const {
    // do algebra using doubles
    CartesianDouble mm = CartesianDouble(_x, _y, _z);
    mm /= STEPS_PER_MM;
    return mm;
}

CartesianInt Point::toSteps() const {
    return CartesianInt(_x, _y, _z);
}

CartesianInt Point::getRaw() const {
    return CartesianInt(_x, _y, _z);
}


double Point::getXMM() const { return _x / STEPS_PER_MM; }
double Point::getYMM() const { return _y / STEPS_PER_MM; }
double Point::getZMM() const { return _z / STEPS_PER_MM; }
int64_t Point::getXSteps() const { return _x; }
int64_t Point::getYSteps() const { return _y; }
int64_t Point::getZSteps() const { return _z; }
void Point::setXMM(double x) { _x = x * STEPS_PER_MM; }
void Point::setYMM(double y) { _y = y * STEPS_PER_MM; }
void Point::setZMM(double z) { _z = z * STEPS_PER_MM; }
void Point::setXSteps(int64_t x) { _x = x; }
void Point::setYSteps(int64_t y) { _y = y; }
void Point::setZSteps(int64_t z) { _z = z; }
