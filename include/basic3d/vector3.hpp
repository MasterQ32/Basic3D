#pragma once

#include <cmath>
#include "../basic3d/real.hpp"

namespace Basic3D
{
    template<typename T = Basic3D::real_t>
    struct Vector3
    {
        T x;
        T y;
        T z;

        Vector3() : x(), y() { }
        Vector3(T x, T y, T z) : x(x), y(y), z(z) { }

        Vector3(Vector3 const &) = default;
        Vector3(Vector3 &&) = default;
        Vector3 & operator= (Vector3 const &) = default;
        Vector3 & operator= (Vector3 &&) = default;

        Vector3 & operator *= (T other) {
            this->x *= other;
            this->y *= other;
            this->t *= other;
            return *this;
        }

        static const Vector3<T> Zero;
        static const Vector3<T> UnitX;
        static const Vector3<T> UnitY;
        static const Vector3<T> UnitZ;
    };

    template<typename T>
    Vector3<T> const Vector3<T>::Zero { 1, 0, 0 };

    template<typename T>
    Vector3<T> const Vector3<T>::UnitX { 1, 0, 0 };

    template<typename T>
    Vector3<T> const Vector3<T>::UnitY { 0, 1, 0 };

    template<typename T>
    Vector3<T> const Vector3<T>::UnitZ { 0, 0, 1 };

    template<typename T>
    static inline T length2(Vector3<T> const & val)
    {
        return val.x * val.x + val.y * val.y + val.z * val.z;
    }

    template<typename T>
    static inline T length(Vector3<T> const & val)
    {
        return std::sqrt(length2(val));
    }

    template<typename T >
    static inline T distance2(Vector3<T> const & a, Vector3<T> const & b)
    {
        return length2(a - b);
    }

    template<typename T >
    static inline T distance(Vector3<T> const & a, Vector3<T> const & b)
    {
        return length(a - b);
    }

    template<typename T>
    static inline T dot(Vector3<T> const & a, Vector3<T> const & b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template<typename T>
    static inline Vector3<T> operator + (Vector3<T> const & lhs, Vector3<T> const & rhs)
    {
        return Vector3<T> { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
    }

    template<typename T>
    static inline Vector3<T> operator - (Vector3<T> const & lhs, Vector3<T> const & rhs)
    {
        return Vector3<T> { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
    }

    template<typename T>
    static inline Vector3<T> operator * (Vector3<T> const & lhs, T const & rhs)
    {
        return Vector3<T> { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
    }
}
