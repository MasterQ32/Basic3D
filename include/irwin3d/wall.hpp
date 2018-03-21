#pragma once

#include "../basic3d/texture.hpp"
#include "../basic3d/vector2.hpp"

namespace Irwin3D
{
    template<typename T = real_t>
    struct Wall
    {
        Basic3D::Texture const * texture;

        Basic3D::Vector2<T> P0, P1;

        T U0, U1;
    };
}
