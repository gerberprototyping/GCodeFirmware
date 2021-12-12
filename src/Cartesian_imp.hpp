template<typename T>
Cartesian<T>::Cartesian(const T &x, const T &y, const T &z)
    : x(x), y(y), z(z)
{}


template<typename T>
Cartesian<T>::Cartesian(const T &magnitude, const Cartesian<T> &direction) {
  x = magnitude*direction.x;
  y = magnitude*direction.y;
  z = magnitude*direction.z;
}




template<typename T>
Cartesian<T>::Cartesian(const Cartesian<T> &c) {
  x = c.x;
  y = c.y;
  z = c.z;
}

template<typename T>
Cartesian<T>::Cartesian(const volatile Cartesian<T> &c) {
  x = *((volatile T*)&c.x);
  y = *((volatile T*)&c.y);
  z = *((volatile T*)&c.z);
}


template<typename T>
void Cartesian<T>::operator=(const volatile Cartesian<T> &c) volatile {
  *((volatile T*)&x) = *((volatile T*)&c.x);
  *((volatile T*)&y) = *((volatile T*)&c.y);
  *((volatile T*)&z) = *((volatile T*)&c.z);
}


template<typename T>
T Cartesian<T>::getMagnitude() const {
  return std::sqrt( std::pow(x,2) + std::pow(y,2) + std::pow(z,2) );
}


template<typename T>
bool Cartesian<T>::isNormalized() const {
  return operator==(normalize());
}


template<typename T>
Cartesian<T> Cartesian<T>::normalize() const {
  return operator/(getMagnitude());
}


template<typename T>
Cartesian<T> Cartesian<T>::operator+(const Cartesian<T> &c) const {
  Cartesian(x+c.x, y+c.y, z+c.z);
}


template<typename T>
Cartesian<T> Cartesian<T>::operator-(const Cartesian<T> &c) const {
  Cartesian(x-c.x, y-c.y, z-c.z);
}


template<typename T>
Cartesian<T> Cartesian<T>::operator*(const double &scalar) const {
  Cartesian(x*scalar, y*scalar, z*scalar);
}


template<typename T>
Cartesian<T> Cartesian<T>::operator/(const double &scalar) const {
  return Cartesian(x/scalar, y/scalar, z/scalar);
}


template<typename T>
void Cartesian<T>::operator+=(const Cartesian<T> &c) {
  x += c.x;
  y += c.y;
  z += c.z;
}


template<typename T>
void Cartesian<T>::operator-=(const Cartesian<T> &c) {
  x -= c.x;
  y -= c.y;
  z -= c.z;
}


template<typename T>
void Cartesian<T>::operator*=(const double &scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
}


template<typename T>
void Cartesian<T>::operator/=(const double &scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;
}


template<typename T>
bool Cartesian<T>::operator==(const Cartesian<T> &c) const {
  return (x == c.x) && (y == c.y) && (z == c.z);
}


template<typename T>
Cartesian<T> operator*(const double &scalar, const Cartesian<T> &c) {
  return Cartesian<T>(c.x*scalar, c.y*scalar, c.z*scalar);
}

