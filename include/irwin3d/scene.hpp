#pragma once

#include "../basic3d/real.hpp"
#include "wall.hpp"

namespace Irwin3D
{
    template<typename Pixel = Basic3D::Pixel32, typename real = Basic3D::real_t>
    struct RaycastResult
    {
        Wall<Pixel,real> const * wall;

        real distance;

        real u;
    };

    template<typename Pixel = Basic3D::Pixel32, typename real = Basic3D::real_t>
    class IScene
    {
    public:
        virtual RaycastResult<Pixel,real> const * castRay(Basic3D::Vector2<real> const & pos, Basic3D::Vector2<real> const & dir) const = 0;
    };
}
