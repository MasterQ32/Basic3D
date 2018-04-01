#pragma once

// this file contains use examples and experiments.

#include "vertex.hpp"
#include "../basic3d/real.hpp"
#include "../basic3d/vector2.hpp"
#include "../basic3d/vector3.hpp"
#include "../basic3d/pixel32.hpp"
#include "../basic3d/texture.hpp"
#include "../basic3d/renderer.hpp"
#include "../basic3d/image.hpp"
#include "../basic3d/zbuffer.hpp"

#include <limits>

namespace Violent3D
{
    template<int WIDTH, int HEIGHT, typename RenderTarget = Basic3D::Image<WIDTH,HEIGHT>, typename ZBuffer = Basic3D::ZBuffer<WIDTH,HEIGHT,uint16_t>, typename real = Basic3D::real_t>
    class Renderer :
        public Basic3D::Renderer<WIDTH, HEIGHT, RenderTarget, ZBuffer>
    {
        typedef Basic3D::Texture<pixel_t> Texture;
        typedef Basic3D::Vector2<real> Vector2;
        typedef Basic3D::Vector3<real> Vector3;
        typedef Violent3D::Vertex<real> Vertex;

        struct Point { int x, y; };

        static constexpr int halfWidth = WIDTH / 2;
        static constexpr int halfHeight = HEIGHT / 2;
    private:
        real minZ;
        real maxZ;
        real invScaleZ;
    public:
        explicit Renderer(RenderTarget * target = nullptr, ZBuffer * zbuffer = nullptr) :
            Basic3D::Renderer<WIDTH, HEIGHT, RenderTarget, ZBuffer>(target, zbuffer),
            minZ(0),
            maxZ(1),
            invScaleZ(1)
        {

        }

        Renderer(Renderer const &) = delete;
        Renderer(Renderer &&) = delete;

        ~Renderer()
        {
        }

        void setZRange(real min, real max)
        {
            assert(max > min);
            this->minZ = min;
            this->maxZ = max;
            this->invScaleZ = real(1) / (max - min);
        }

    public:
        template<typename Shader>
        void drawTriangle(Shader const & shader, Vertex const & v1, Vertex const & v2, Vertex const & v3)
        {
            Point const p1 = vertexToPoint(v1);
            Point const p2 = vertexToPoint(v2);
            Point const p3 = vertexToPoint(v3);

            if((p1.x < 0) && (p2.x < 0) && (p3.x < 0))
                return;

            if((p1.x >= width) && (p2.x >= width) && (p3.x >= width))
                return;

            if((v1.pos.z < this->minZ) && (v2.pos.z < this->minZ) && (v3.pos.z < this->minZ))
                return;

            if((v1.pos.z >= this->maxZ) && (v2.pos.z >= this->maxZ) && (v3.pos.z >= this->maxZ))
                return;

            std::array<Point const *,3> plist = { &p1, &p2, &p3 };
            {
                // apply sorting network for 3 elements
                // http://www.angelfire.com/blog/ronz/Articles/999SortingNetworksReferen.html
                if(plist[0]->y > plist[1]->y)
                    std::swap(plist[0], plist[1]);
                if(plist[1]->y > plist[2]->y)
                    std::swap(plist[1], plist[2]);
                if(plist[0]->y > plist[1]->y)
                    std::swap(plist[0], plist[1]);
            }

            // this is horrible, but:
            // it sorts the vertices top(vt), to bottom(vb) with a middle point (vm)
            Point const & pt = *plist[0];
            Point const & pm = *plist[1];
            Point const & pb = *plist[2];

            if((pb.y < 0) || (pt.y >= height))
                return;

            assert(&pt != &pb);
            assert(&pb != &pm);
            assert(&pt != &pm);

            // Skip squished triangles
            if(pt.y == pb.y)
                return;

            Point const ps = {
                int(pt.x + real((pm.y - pt.y) * (pb.x - pt.x)) / real(pb.y - pt.y)),
                pm.y
            };

            real const invTotalArea = real(1) / areaOfTris(p1, p2, p3);

            if(invTotalArea < (real(1) / real(width * height)))
                return;

            if(pt.y != pm.y) // Draw upper half
            {
                assert(pt.y < pm.y);

                real const invfdy(real(1) / real(pm.y - pt.y));

                real const f0(real(std::min(pm.x, ps.x) - pt.x) * invfdy);
                real const f1(real(std::max(pm.x, ps.x) - pt.x) * invfdy);

                real fp0(pt.x);
                real fp1(pt.x);

                int const top = std::max(pt.y, 0);
                int const bottom = std::min(pm.y, HEIGHT - 1);

                fp0 += real(top - pt.y) * f0;
                fp1 += real(top - pt.y) * f1;

                Point p;
                for(p.y = top; p.y <= bottom; p.y++)
                {
                    int const left = std::max(int(fp0), 0);
                    int const right = std::min(int(fp1), width - 1);
                    for(p.x = left; p.x <= right; p.x++)
                    {
                        real const a12 = areaOfTris(p1, p2, p);
                        real const a23 = areaOfTris(p2, p3, p);
                        real const a31 = areaOfTris(p3, p1, p);

                        real const f1 = a23 * invTotalArea;
                        real const f3 = a12 * invTotalArea;
                        real const f2 = a31 * invTotalArea;

                        shadePixel(shader, v1, v2, v3, p, Vector3(f1,f2,f3));
                    }
                    fp0 += f0;
                    fp1 += f1;
                }
            }
            if(pm.y != pb.y) // draw lower half
            {
                assert(pm.y < pb.y);

                real const invfdy(real(1) / real(pm.y - pb.y));

                real fp0 = (pm.x <  ps.x) ? real(pm.x) : real(ps.x);
                real fp1 = (pm.x >= ps.x) ? real(pm.x) : real(ps.x);

                real const f0(real(fp0 - pb.x) * invfdy);
                real const f1(real(fp1 - pb.x) * invfdy);

                int const top = std::max(pm.y, 0);
                int const bottom = std::min(pb.y, HEIGHT - 1);

                fp0 += real(top - pm.y) * f0;
                fp1 += real(top - pm.y) * f1;

                Point p;
                for(p.y = top; p.y <= bottom; p.y++)
                {
                    int const left = std::max(int(fp0), 0);
                    int const right = std::min(int(fp1), width - 1);
                    for(p.x = left; p.x <= right; p.x++)
                    {
                        real const a12 = areaOfTris(p1, p2, p);
                        real const a23 = areaOfTris(p2, p3, p);
                        real const a31 = areaOfTris(p3, p1, p);

                        real const f1 = a23 * invTotalArea;
                        real const f3 = a12 * invTotalArea;
                        real const f2 = a31 * invTotalArea;

                        shadePixel(shader, v1, v2, v3, p, Vector3(f1,f2,f3));
                    }
                    fp0 += f0;
                    fp1 += f1;
                }
            }
        }

    private: // private utilities

        //! renders a single pixel of a triangle
        template<typename Shader>
        void shadePixel(
            Shader  const & shader, //! the shader to be used
            Vertex  const & v1,     //! first vertex
            Vertex  const & v2,     //! second vertex
            Vertex  const & v3,     //! third vertex
            Point   const & p,      //! point on the triangle
            Vector3 const & coords  //! barycentric coordinates for (v1,v2,v3)
            )
        {
            Vertex interpolated;
            interpolated.pos = Vector3(real(p.x), real(p.y), 0);

            real fz = coords.x * v1.pos.z + coords.y * v2.pos.z + coords.z * v3.pos.z;

            // front- and backplane clipping
            if((fz < this->minZ) || (fz >= this->maxZ))
                return;

            interpolated.pos.z = fz;

            fz -= this->minZ;
            fz *= this->invScaleZ;

            depth_t z = depth_t(real(std::numeric_limits<depth_t>::max() * fz));
            if(z > getDepth(p.x,p.y))
                return;

            Vector2 const uv;

            interpolated.uv = Vector2(coords.x * v1.uv + coords.y * v2.uv + coords.z * v3.uv);

            bool discard = false;

            pixel_t color = shader.shade(interpolated, discard);

            if(discard)
                return;

           setPixel(p.x,p.y,color);
           setDepth(p.x,p.y,z);
        }

        //! Converts a vertex into an integer screen point
        Point vertexToPoint(Vertex const & v)
        {
            return Point {
                int(v.pos.x + halfWidth  + 0.5),
                int(v.pos.y + halfHeight + 0.5)
            };
        }

        //! Calculate the 2D area of given triangle
        template<typename T1, typename T2, typename T3>
        real areaOfTris(T1 const & a, T2 const & b, T3 const & c)
        {
            // https://en.wikipedia.org/wiki/Triangle#Using_coordinates
            return real(0.5) * std::abs((a.x - c.x) * (b.y - a.y) - (a.x - b.x) * (c.y - a.y));
        }
    };
}
