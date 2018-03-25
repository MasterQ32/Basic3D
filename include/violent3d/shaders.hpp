#pragma once

#include "vertex.hpp"
#include "../basic3d/pixel32.hpp"
#include "../basic3d/texture.hpp"

#include <algorithm>

namespace Violent3D
{
    template<typename pixel_t = Basic3D::Pixel32, typename real = Basic3D::real_t>
    struct TextureShader
    {
        Basic3D::Texture<pixel_t> * texture;

        pixel_t shade(Vertex<real> const & vtx, bool & discard) const
        {
            auto color = texture->sample(
                int(real(texture->width - 1) * Basic3D::fract(vtx.uv.x)),
                int(real(texture->height - 1) * Basic3D::fract(vtx.uv.y)));
            if(!Basic3D::alphaTest(color))
                discard = true;
            return color;
        }
    };

    template<typename pixel_t = Basic3D::Pixel32, typename real = Basic3D::real_t>
    struct TextureCoordShader
    {
        pixel_t shade(Vertex<real> const & vtx, bool) const
        {
            return pixel_t(
                uint8_t(255 * std::max(real(0), std::min(real(1), Basic3D::fract(vtx.uv.x)))),
                uint8_t(255 * std::max(real(0), std::min(real(1), Basic3D::fract(vtx.uv.y)))),
                0x00);
        }
    };

    template<typename pixel_t = Basic3D::Pixel32, typename real = Basic3D::real_t>
    struct ColorShader
    {
        pixel_t color;

        pixel_t shade(Vertex<real> const &, bool) const
        {
            return color;
        }
    };
}
