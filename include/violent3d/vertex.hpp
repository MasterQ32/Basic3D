#pragma once

#include "../basic3d/real.hpp"
#include "../basic3d/pixel32.hpp"

#include "../basic3d/vector2.hpp"
#include "../basic3d/vector3.hpp"

namespace Violent3D
{
    template<typename real = Basic3D::real_t>
    struct Vertex
    {
        Basic3D::Vector3<real> pos;

        Basic3D::Vector2<real> uv;
    };
}
