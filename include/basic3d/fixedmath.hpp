#pragma once

#include "fixed.hpp"
#include <cmath>

namespace std
{
    template<int I, int D>
    static inline Basic3D::Fixed<I, D> floor(Basic3D::Fixed<I, D> val) {
        return Basic3D::Fixed<I, D>(std::floor(double(val)));
    }

    template<int I, int D>
    static inline Basic3D::Fixed<I, D> atan(Basic3D::Fixed<I, D> val) {
        return Basic3D::Fixed<I, D>(std::atan(double(val)));
    }

    template<int I, int D>
    static inline Basic3D::Fixed<I, D> sin(Basic3D::Fixed<I, D> val) {
        return Basic3D::Fixed<I, D>(std::sin(double(val)));
    }

    template<int I, int D>
    static inline Basic3D::Fixed<I, D> cos(Basic3D::Fixed<I, D> val) {
        return Basic3D::Fixed<I, D>(std::cos(double(val)));
    }
}
