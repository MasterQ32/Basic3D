#pragma once

// this file contains use examples and experiments.

#include "material.hpp"
#include "vertex.hpp"
#include "../basic3d/real.hpp"
#include "../basic3d/vector2.hpp"
#include "../basic3d/vector3.hpp"
#include "../basic3d/pixel.hpp"
#include "../basic3d/texture.hpp"
#include "../basic3d/image.hpp"
#include "../basic3d/fixedmath.hpp"

#include <limits>

namespace Violent3D
{
    template<int width, int height, typename depth_t = uint16_t, typename real = Basic3D::real_t>
    class Renderer
    {
        typedef Basic3D::pixel_t pixel_t;
        typedef Basic3D::Texture Texture;
        typedef Basic3D::Vector2<real> Vector2;
        typedef Basic3D::Vector3<real> Vector3;
    private:
        std::array<depth_t, width * height> zbuffer;

    public:
        //! current render target
        pixel_t * RenderTarget;

        //! current polygon material
        Material const * Material;

    public:
        explicit Renderer(Basic3D::Image<width,height> & target) :
            Renderer(target.data())
        {
            this->clearz();
        }

        explicit Renderer(pixel_t * rt = nullptr) :
            zbuffer(),
            RenderTarget(rt),
            Material(nullptr)
        {

        }

        Renderer(Renderer const &) = delete;
        Renderer(Renderer &&) = delete;

        ~Renderer()
        {
        }

    public:
        void clearz(depth_t value = std::numeric_limits<depth_t>::max())
        {
            for(int i = 0; i < (width * height); i++)
                this->zbuffer[i] = value;
        }

        void drawTriangle(Vertex<real> const & v1, Vertex<real> const & v2, Vertex<real> const & v3)
        {
            assert(Material != nullptr);

            Vector2 const v2d1(v1.pos.x, v1.pos.y);
            Vector2 const v2d2(v2.pos.x, v2.pos.y);
            Vector2 const v2d3(v3.pos.x, v3.pos.y);

            // calculate bounding rectangle with clipping applied (first "inverse" limiter)
            int const left = std::max(0, int(std::min(std::min(v2d1.x, v2d2.x), v2d3.x)));
            int const right = std::min(width - 1, int(std::max(std::max<real>(v2d1.x, v2d2.x), v2d3.x)));

            int const top = std::max(0, int(std::min(std::min(v2d1.y, v2d2.y), v2d3.y)));
            int const bottom = std::min(height - 1, int(std::max(std::max(v2d1.y, v2d2.y), v2d3.y)));

            real const total = areaOfTris(v2d1, v2d2, v2d3);

            Texture const * const tex = Material->Texture;

            for(int y = top; y <= bottom; y++)
            {
                for(int x = left; x <= right; x++)
                {
                    Vector2 const pt { real(x), real(y) };

                    real const a12 = areaOfTris(v2d1, v2d2, pt);
                    real const a23 = areaOfTris(v2d2, v2d3, pt);
                    real const a31 = areaOfTris(v2d3, v2d1, pt);

                    if((a12 + a23 + a31) > total)
                        continue;

                    real const f1 = a23 / total;
                    real const f3 = a12 / total;
                    real const f2 = a31 / total;

                    depth_t z = depth_t(real(std::numeric_limits<depth_t>::max() * (f1 * v1.pos.z + f2 * v2.pos.z + f3 * v3.pos.z)));
                    if(z > zat(x,y))
                        continue;
                    zat(x,y) = z;

                    if(tex != nullptr)
                    {
                        Vector2 const uv(f1 * v1.uv + f2 * v2.uv + f3 * v3.uv);
                        pixel(x,y) = tex->sample(
                            int(real(tex->width - 1) * fract(uv.x)),
                            int(real(tex->height - 1) * fract(uv.y)));

                    }
                    else
                    {
                        pixel(x,y) = Material->Albedo;
                    }
                }
            }
        }

    public: // public utilities
        depth_t & zat(int x, int y) {
            return zbuffer[y * width + x];
        }
        depth_t const & zat(int x, int y) const {
            return zbuffer[y * width + x];
        }
    private: // private utilities
        pixel_t & pixel(int x, int y) {
            return RenderTarget[y * width + x];
        }

        real sign (Vector2 const & p1, Vector2 const & p2, Vector2 const & p3)
        {
            return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
        }

        real areaOfTris(Vector2 const & a, Vector2 const & b, Vector2 const & c)
        {
            // https://en.wikipedia.org/wiki/Triangle#Using_coordinates
            return real(0.5) * std::abs((a.x - c.x) * (b.y - a.y) - (a.x - b.x) * (c.y - a.y));
        }
    };
}
