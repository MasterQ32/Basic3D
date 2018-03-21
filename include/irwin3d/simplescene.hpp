#pragma once

#include "scene.hpp"

#include <vector>

namespace Irwin3D
{
    template<typename T = real_t>
    class SimpleScene :
        public IScene<T>
    {
    private:
        std::vector<Wall<T>> walls;
    public:
        RaycastResult<T> const * castRay(Basic3D::Vector2<T> const & pos, Basic3D::Vector2<T> const & dir) const override
        {
            static RaycastResult<T> value;
            RaycastResult<T> * nearest = nullptr;

            for(auto const & wall : this->walls)
            {
                T t, u;
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

        std::vector<Wall<T>> & getWalls() { return this->walls; }
        std::vector<Wall<T>> const & getWalls() const { return this->walls; }
    };
}
