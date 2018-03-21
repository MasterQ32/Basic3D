#pragma once

#include <cmath>
#include "../basic3d/real.hpp"

namespace Basic3D
{
    template<typename T = Basic3D::real_t>
    struct Vector2
    {
        T x;
        T y;

        Vector2() : x(), y() { }
        Vector2(T x, T y) : x(x), y(y) { }

        Vector2(Vector2 const &) = default;
        Vector2(Vector2 &&) = default;
        Vector2 & operator= (Vector2 const &) = default;
        Vector2 & operator= (Vector2 &&) = default;

        Vector2 & operator *= (T other) {
            this->x *= other;
            this->y *= other;
            return *this;
        }

        static const Vector2<T> Zero;
        static const Vector2<T> UnitX;
        static const Vector2<T> UnitY;
    };

    template<typename T>
    Vector2<T> const Vector2<T>::Zero { 1, 0 };

    template<typename T>
    Vector2<T> const Vector2<T>::UnitX { 1, 0 };

    template<typename T>
    Vector2<T> const Vector2<T>::UnitY { 0, 1 };

    template<typename T>
    static inline T length2(Vector2<T> const & val)
    {
        return val.x * val.x + val.y * val.y;
    }

    template<typename T>
    static inline T length(Vector2<T> const & val)
    {
        return std::sqrt(length2(val));
    }

    template<typename T >
    static inline T distance2(Vector2<T> const & a, Vector2<T> const & b)
    {
        return length2(a - b);
    }

    template<typename T >
    static inline T distance(Vector2<T> const & a, Vector2<T> const & b)
    {
        return length(a - b);
    }

    template<typename T>
    static inline Vector2<T> operator + (Vector2<T> const & lhs, Vector2<T> const & rhs)
    {
        return Vector2<T> { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    template<typename T>
    static inline Vector2<T> operator - (Vector2<T> const & lhs, Vector2<T> const & rhs)
    {
        return Vector2<T> { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    template<typename T>
    static inline Vector2<T> operator * (Vector2<T> const & lhs, T const & rhs)
    {
        return Vector2<T> { lhs.x * rhs, lhs.y * rhs };
    }
}
