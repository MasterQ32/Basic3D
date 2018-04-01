#pragma once

#include "vector2.hpp"
#include <cmath>

namespace Basic3D
{
    static const float Pi = float(3.14159265358979323846);
    static const float TwoPi = float(6.28318530717958647693);
    static const float PiOver2 = float(1.57079632679489661923);

    template<typename T>
    T radians(T degree)
    {
        return T(Pi) * degree / T(180);
    }

    template<typename T>
    T degree(T degree)
    {
        return T(180) * degree / T(Pi);
    }

    template<typename T>
    Vector2<T> rotate(Vector2<T> const & v, T angle)
    {
        T const cs = std::cos(angle);
        T const sn = std::sin(angle);
        return Vector2<T>
        {
            v.x * cs - v.y * sn,
            v.x * sn + v.y * cs
        };
    }

    template<typename T>
    T cross(Vector2<T> const & v, Vector2<T> const & w) {
        return (v.x * w.y - v.y * w.x);
    }

    template<typename T>
    bool intersect(Vector2<T> const & p, Vector2<T> const & r, Vector2<T> const & q, Vector2<T> const & s, T & t,T & u)
    {
        T d = cross(r, s);
        Vector2<T> v = q - p;

        if (d == 0) // parallel or colinear
            return false;

        t = cross(v, s) / d;
        u = cross(v, r) / d;

        return (t >= 0.0) && (u >= 0.0 && u <= 1.0);
    }

    template<typename T,typename TR>
    T lerp(T a, T b, TR f)
    {
        return a + f * (b - a);
    }

    template<typename T>
    T clamp(T v, T min, T max)
    {
        return std::min(std::max(v, min), max);
    }

    template<typename T>
    T fract(T a)
    {
        return a - std::floor(a);
    }

    template<typename T>
    T mod(T a, T b)
    {
        return a - std::floor(a / b) * b;
    }

    template<typename T>
    T ang(T a)
    {
        return mod((a + Pi), TwoPi) - Pi;
    }

    template<typename T>
    T angdiff(T lhs, T rhs)
    {
        return ang(lhs - rhs);
    }
}
