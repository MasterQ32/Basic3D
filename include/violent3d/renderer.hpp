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

        real MinZ;
        real MaxZ;

    public:
        explicit Renderer(Basic3D::Image<width,height> & target) :
            Renderer(target.data())
        {
            this->clearz();
        }

        explicit Renderer(pixel_t * rt = nullptr) :
            zbuffer(),
            RenderTarget(rt),
            Material(nullptr),
            MinZ(0),
            MaxZ(1)
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

            Vector2 const v2d1(v1.pos.x + width/2, v1.pos.y + height/2);
            Vector2 const v2d2(v2.pos.x + width/2, v2.pos.y + height/2);
            Vector2 const v2d3(v3.pos.x + width/2, v3.pos.y + height/2);

            // calculate bounding rectangle with clipping applied (first "inverse" limiter)
            int const left = std::max(0, int(std::min(std::min(v2d1.x, v2d2.x), v2d3.x)));
            int const right = std::min(width - 1, int(std::max(std::max<real>(v2d1.x, v2d2.x), v2d3.x)));

            int const top = std::max(0, int(std::min(std::min(v2d1.y, v2d2.y), v2d3.y)));
            int const bottom = std::min(height - 1, int(std::max(std::max(v2d1.y, v2d2.y), v2d3.y)));

            real const total = areaOfTris(v2d1, v2d2, v2d3);
            real const reftotal = std::nexttoward(std::nexttoward(total, total + 1), total + 1); // adjust our threshold just a teeny tiny bit

            Texture const * const tex = Material->Texture;

            for(int y = top; y <= bottom; y++)
            {
                for(int x = left; x <= right; x++)
                {
                    Vector2 const pt { real(x), real(y) };

                    real const a12 = areaOfTris(v2d1, v2d2, pt);
                    real const a23 = areaOfTris(v2d2, v2d3, pt);
                    real const a31 = areaOfTris(v2d3, v2d1, pt);

                    if((a12 + a23 + a31) > 1.001f * reftotal)
                         continue;

                    real const f1 = a23 / total;
                    real const f3 = a12 / total;
                    real const f2 = a31 / total;

                    real fz = f1 * v1.pos.z + f2 * v2.pos.z + f3 * v3.pos.z;
                    if((fz < MinZ) || (fz >= MaxZ))
                        continue;

                    fz -= MinZ;
                    fz /= (MaxZ - MinZ);

                    depth_t z = depth_t(real(std::numeric_limits<depth_t>::max() * fz));
                    if(z > zat(x,y))
                        continue;

                    if(tex != nullptr)
                    {
                        Vector2 const uv(f1 * v1.uv + f2 * v2.uv + f3 * v3.uv);
                        auto color = tex->sample(
                            int(real(tex->width - 1) * Basic3D::fract(uv.x)),
                            int(real(tex->height - 1) * Basic3D::fract(uv.y)));
                        if((color.alpha & 0x80) == 0)
                            continue;
                        pixel(x,y) = color;
                    }
                    else
                    {
                        pixel(x,y) = Material->Albedo;
                        // pixel(x,y) = pixel_t(255 * f1, 255 * f2, 255 * (1 - f1 * f2));
                    }
                    zat(x,y) = z;
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

        real areaOfTris(Vector2 const & a, Vector2 const & b, Vector2 const & c)
        {
            // https://en.wikipedia.org/wiki/Triangle#Using_coordinates
            return real(0.5) * std::abs((a.x - c.x) * (b.y - a.y) - (a.x - b.x) * (c.y - a.y));
        }
    };
}
