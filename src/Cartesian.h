#ifndef __CARTESIAN_H
#define __CARTESIAN_H

#include <cmath>
#include <cstdint>

#include "config.h"
#include "Concurrent.h"


// Forward Declare Cartesian
template<typename T>
class Cartesian;


template<typename T>
class AtomicCartesian {

    public:

        AtomicCartesian();
        AtomicCartesian(const AtomicCartesian<T> &AC);
        void store(const Cartesian<T> &C) volatile;
        Cartesian<T> load() volatile;

    private:

        volatile Semaphore lock;
        volatile T x;
        volatile T y;
        volatile T z;

};

template<typename T>
class Cartesian {

    public:

        Cartesian();
        Cartesian(const T &val);
        Cartesian(const T &x, const T &y, const T &z);
        Cartesian(const Cartesian<T> &C);
        template<typename T2> Cartesian(const Cartesian<T2> &C);

        T getX() const;
        T getY() const;
        T getZ() const;
        void setX(T &x);
        void setY(T &y);
        void setZ(T &z);

        T getMagnitude() const;
        bool isNormalized() const;
        Cartesian<T> abs() const;
        Cartesian<T> normalize() const;
        bool operator==(const Cartesian<T> &C) const;

        Cartesian<T> operator+(const Cartesian<T> &C) const;
        Cartesian<T> operator-(const Cartesian<T> &C) const;
        Cartesian<T> operator+(const T &scalar) const;
        Cartesian<T> operator-(const T &scalar) const;
        Cartesian<T> operator*(const T &scalar) const;
        Cartesian<T> operator/(const T &scalar) const;

        void operator+=(const Cartesian<T> &C);
        void operator-=(const Cartesian<T> &C);
        void operator+=(const T &scalar);
        void operator-=(const T &scalar);
        void operator*=(const T &scalar);
        void operator/=(const T & scalar);

        friend class AtomicCartesian<T>;

    protected:
    
        T _x, _y, _z;

};

template<typename T> Cartesian<T> operator/(const int scalar, Cartesian<T> C);


using CartesianInt = Cartesian<int64_t>;
using CartesianUint = Cartesian<uint64_t>;
using CartesianDouble = Cartesian<double>;
using AtomicCartesianInt = AtomicCartesian<int64_t>;
using AtomicCartesianUint = AtomicCartesian<uint64_t>;
using AtomicCartesianDouble = AtomicCartesian<double>;

using Velocity = Cartesian<double>;
using AtomicVelocity = AtomicCartesian<double>;


// Forward declare AtomicPoint
struct AtomicPoint;

class Point: public CartesianInt {

    public:

        Point() : CartesianInt() {}

        static Point fromMM(CartesianDouble &C);
        static Point fromMM(double x, double y, double z);
        static Point fromSteps(CartesianInt &C);
        static Point fromSteps(int64_t x, int64_t y, int64_t z);

        CartesianDouble toMM() const;
        CartesianInt toSteps() const;
        CartesianInt getRaw() const;

        double getXMM() const;
        double getYMM() const;
        double getZMM() const;
        int64_t getXSteps() const;
        int64_t getYSteps() const;
        int64_t getZSteps() const;
        void setXMM(double x);
        void setYMM(double y);
        void setZMM(double Z);
        void setXSteps(int64_t x);
        void setYSteps(int64_t y);
        void setZSteps(int64_t z);

        friend struct AtomicPoint;

    protected:

        Point(const CartesianInt C) : CartesianInt(C) {}
        Point(int64_t x, int64_t y, int64_t z) : CartesianInt(x, y, z) {}

        using CartesianInt::getX;
        using CartesianInt::getY;
        using CartesianInt::getZ;
        using CartesianInt::setX;
        using CartesianInt::setY;
        using CartesianInt::setZ;

};

struct AtomicPoint: public AtomicCartesian<int64_t> {
    Point load() volatile { return Point(AtomicCartesian::load()); }
};


#include "Cartesian.hpp"


#endif // __CARTESIAN_H