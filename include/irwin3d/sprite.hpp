#pragma once

#include "../basic3d/real.hpp"
#include "../basic3d/texture.hpp"
#include "../basic3d/vec2.hpp"

namespace Irwin3D
{
    template<typename T = Basic3D::real_t>
    struct Sprite
    {
        Basic3D::Vector2<T> position;
        Basic3D::Texture const * texture;
    };
}
