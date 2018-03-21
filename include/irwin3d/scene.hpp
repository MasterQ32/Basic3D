#pragma once

#include "../basic3d/real.hpp"
#include "wall.hpp"

namespace Irwin3D
{
    template<typename T = Basic3D::real_t>
    struct RaycastResult
    {
        Wall<T> const * wall;

        T distance;

        T u;
    };

    template<typename T>
    class IScene
    {
    public:
        virtual RaycastResult<T> const * castRay(Basic3D::Vector2<T> const & pos, Basic3D::Vector2<T> const & dir) const = 0;
    };
}
