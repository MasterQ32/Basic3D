#pragma once

#include "real.hpp"

namespace Basic3D
{
    template<typename real = Basic3D::real_t>
    struct Matrix4
    {
        real v[4][4];
    };
}
