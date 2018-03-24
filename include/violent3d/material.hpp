#pragma once

#include "../basic3d/texture.hpp"
#include "../basic3d/pixel32.hpp"

namespace Violent3D
{
    template<typename Pixel = Basic3D::Pixel32>
    struct Material
    {
        //! Surface color when no texture is set.
        Pixel Albedo;

        //! Surface texture (or `nullptr` for flat color)
        Basic3D::Texture<Pixel> const * Texture;
    };
}
