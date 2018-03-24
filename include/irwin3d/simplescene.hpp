#pragma once

#include "scene.hpp"
#include "../basic3d/utils.hpp"

#include <vector>

namespace Irwin3D
{
    template<typename Pixel = Basic3D::Pixel32, typename real = Basic3D::real_t>
    class SimpleScene :
        public IScene<Pixel,real>
    {
    private:
        std::vector<Wall<Pixel,real>> walls;
    public:
        RaycastResult<Pixel,real> const * castRay(Basic3D::Vector2<real> const & pos, Basic3D::Vector2<real> const & dir) const override
        {
            static RaycastResult<Pixel,real> value;
            RaycastResult<Pixel,real> * nearest = nullptr;

            for(auto const & wall : this->walls)
            {
                real t, u;
                if (intersect(pos, dir, wall.P0, wall.P1 - wall.P0, t, u) == false)
                    continue;

                if ((nearest == nullptr) || (nearest->distance > t))
                {
                    nearest = &value;
                    nearest->distance = t;
                    nearest->wall = &wall;
                    nearest->u = wall.U0 + u * (wall.U1 - wall.U0);
                }
            }

            return nearest;
        }

        std::vector<Wall<Pixel,real>> & getWalls() { return this->walls; }
        std::vector<Wall<Pixel,real>> const & getWalls() const { return this->walls; }
    };
}
