#pragma once

#include "../basic3d/texture.hpp"
#include "../basic3d/pixel.hpp"

namespace Violent3D
{
    struct Material
    {
        //! Surface color
        Basic3D::pixel_t Albedo;

        //! Surface texture (or null)
        Basic3D::Texture const * Texture;
    };
}
