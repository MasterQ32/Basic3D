#pragma once

#include "../basic3d/real.hpp"
#include "../basic3d/texture.hpp"
#include "../basic3d/vector2.hpp"

namespace Irwin3D
{
    template<typename Pixel = Basic3D::Pixel32, typename real = Basic3D::real_t>
    struct Wall
    {
        Basic3D::Texture<Pixel> const * texture;

        Basic3D::Vector2<real> P0, P1;

        real U0, U1;
    };
}
