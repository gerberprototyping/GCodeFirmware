#include "Cartesian.h"

// Template implementation file




/***********************************************************
 * Cartesian
 **********************************************************/

template<typename T>
Cartesian<T>::Cartesian()
    : _x(0), _y(0), _z(0)
{
    // Intentionally left blank
}

template<typename T>
Cartesian<T>::Cartesian(const T val)
    : _x(val), _y(val), _z(val)
{
    // Intentionally left blank
}

template<typename T>
Cartesian<T>::Cartesian(const T x, const T y, const T z)
    : _x(x), _y(y), _z(z)
{
    // Intentionally left blank
}

template<typename T>
Cartesian<T>::Cartesian(const Cartesian<T>& C)
    : _x(C._x), _y(C._y), _z(C._z)
{
    // Intentionally left blank
}

template<typename T>
Cartesian<T>::Cartesian(const volatile Cartesian& C) {
    _x = *static_cast<const volatile T*>(&C._x);
    _y = *static_cast<const volatile T*>(&C._y);
    _z = *static_cast<const volatile T*>(&C._z);
}

template<typename T>
template<typename T2>
Cartesian<T>::Cartesian(const Cartesian<T2>& C)
    : _x(C.getX()), _y(C.getY()), _z(C.getZ())
{
    // Intentionally left blank
}

template<typename T>
Cartesian<T>& Cartesian<T>::operator=(const Cartesian<T> &C) {
    _x = C._x;
    _y = C._y;
    _z = C._z;
    return *this;
}

template<typename T>
Cartesian<T>& Cartesian<T>::operator=(const volatile Cartesian<T>& C) {
    _x = *static_cast<const volatile T*>(&C._x);
    _y = *static_cast<const volatile T*>(&C._y);
    _z = *static_cast<const volatile T*>(&C._z);
    return *this;
}

template<typename T>
volatile Cartesian<T>& Cartesian<T>::operator=(const Cartesian<T>& C) volatile {
    *static_cast<volatile T*>(&_x) = C._x;
    *static_cast<volatile T*>(&_y) = C._y;
    *static_cast<volatile T*>(&_z) = C._z;
    return *this;
}


template<typename T>
inline T Cartesian<T>::getX() const {
    return _x;
}

template<typename T>
inline T Cartesian<T>::getY() const {
    return _y;
}

template<typename T>
inline T Cartesian<T>::getZ() const {
    return _z;
}

template<typename T>
inline void Cartesian<T>::setX(T x) {
    _x = x;
}

template<typename T>
inline void Cartesian<T>::setY(T y) {
    _y = y;
}

template<typename T>
inline void Cartesian<T>::setZ(T z) {
    _z = z;
}




template<typename T>
inline T Cartesian<T>::getMagnitude() const {
    return std::sqrt( std::pow(_x,2) + std::pow(_y,2) + std::pow(_z,2));
}

template<typename T>
inline bool Cartesian<T>::isNormalized() const {
    return 1 == getMagnitude();
}

template<typename T>
inline bool Cartesian<T>::isZero() const {
    return (!_x) && (!_y) && (!_z);
}

template<typename T>
inline bool Cartesian<T>::isNonZero() const {
    return _x || _y || _z;
}

template<typename T>
inline T Cartesian<T>::bitwiseAnd() const {
    return _x & _y & _z;
}

template<typename T>
inline T Cartesian<T>::bitwiseOr() const {
    return _x | _y | _z;
}

template<typename T>
inline Cartesian<T> Cartesian<T>::abs() const {
    return Cartesian<T>(std::abs(_x), std::abs(_y), std::abs(_z));
}

template<typename T>
inline Cartesian<T> Cartesian<T>::normalize() const {
    T mag = getMagnitude();
    return Cartesian<T>(_x/mag, _y/mag, _z/mag);
}

template<typename T>
inline bool Cartesian<T>::operator==(const Cartesian<T>& C) const {
    return (_x == C._x) && (_y == C._y) && (_z == C._z);
}




template<typename T>
inline Cartesian<T> Cartesian<T>::operator+(const Cartesian<T>& C) const {
    return Cartesian<T>(_x + C._x, _y + C._y, _z + C._z);
}

template<typename T>
inline Cartesian<T> Cartesian<T>::operator-(const Cartesian<T>& C) const {
    return Cartesian<T>(_x - C._x, _y - C._y, _z - C._z);
}

template<typename T>
inline Cartesian<T> Cartesian<T>::operator+(const T scalar) const {
    return Cartesian<T>(_x + scalar, _y + scalar, _z + scalar);
}

template<typename T>
inline Cartesian<T> Cartesian<T>::operator-(const T scalar) const {
    return Cartesian<T>(_x - scalar, _y - scalar, _z - scalar);
}

template<typename T>
inline Cartesian<T> Cartesian<T>::operator*(const T scalar) const {
    return Cartesian<T>(_x * scalar, _y * scalar, _z * scalar);
}

template<typename T>
inline Cartesian<T> Cartesian<T>::operator/(const T scalar) const {
    return Cartesian<T>(_x / scalar, _y / scalar, _z / scalar);
}


template<typename T>
inline void Cartesian<T>::operator+=(const Cartesian<T>& C) {
    _x += C._x;
    _y += C._y;
    _z += C._z;
}

template<typename T>
inline void Cartesian<T>::operator-=(const Cartesian<T>& C) {
    _x -= C._x;
    _y -= C._y;
    _z -= C._z;
}

template<typename T>
inline void Cartesian<T>::operator+=(const T scalar) {
    _x += scalar;
    _y += scalar;
    _z += scalar;
}

template<typename T>
inline void Cartesian<T>::operator-=(const T scalar) {
    _x -= scalar;
    _y -= scalar;
    _z -= scalar;
}

template<typename T>
inline void Cartesian<T>::operator*=(const T scalar) {
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;
}

template<typename T>
inline void Cartesian<T>::operator/=(const T scalar) {
    _x /= scalar;
    _y /= scalar;
    _z /= scalar;
}


template<typename T>
inline Cartesian<T> operator/(const int scalar, Cartesian<T> C) {
    return Cartesian<T>(scalar / C.getX(), scalar / C.getY(), scalar / C.getZ());
}
