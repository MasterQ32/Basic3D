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
    public:
        real MinZ;
        real MaxZ;
    public:
        explicit Renderer(RenderTarget * target = nullptr, ZBuffer * zbuffer = nullptr) :
            Basic3D::Renderer<WIDTH, HEIGHT, RenderTarget, ZBuffer>(target, zbuffer),
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
        Point vertexToPoint(Vertex const & v)
        {
            // return Point { int(v.pos.x), int(v.pos.y) };
            return Point {
                int(v.pos.x + halfWidth  + 0.5),
                int(v.pos.y + halfHeight + 0.5)
            };
        }

        template<typename Shader>
        void drawTriangle(Shader const & shader, Vertex const & v1, Vertex const & v2, Vertex const & v3)
        {
            Point const p1 = vertexToPoint(v1);
            Point const p2 = vertexToPoint(v2);
            Point const p3 = vertexToPoint(v3);

            // this is horrible, but:
            // it sorts the vertices top(vt), to bottom(vb) with a middle point (vm)
            Point const & pt = ((p1.y <= p2.y) && (p1.y <= p3.y)) ? p1 : ((p2.y <= p3.y) ? p2 : p3);
            Point const & pb = ((p1.y >  p2.y) && (p1.y >  p3.y)) ? p1 : ((p2.y >  p3.y) ? p2 : p3);
            Point const & pm = (((&pt == &p1) && (&pb == &p3)) || ((&pt == &p3) && (&pb == &p1))) ? p2 :
                (((&pt == &p2) && (&pb == &p3)) || ((&pt == &p3) && (&pb == &p2))) ? p1 :
                p3;

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

            real const totalArea = areaOfTris(p1, p2, p3);

            if(pt.y != pm.y) // Draw upper half
            {
                assert(pt.y < pm.y);

                real const fdy(real(pm.y - pt.y));

                Vector2 const f0(real(std::min(pm.x, ps.x) - pt.x) / fdy, real(1.0));
                Vector2 const f1(real(std::max(pm.x, ps.x) - pt.x) / fdy, real(1.0));

                Vector2 fp0(real(pt.x), real(pt.y));
                Vector2 fp1(real(pt.x), real(pt.y));

                Point p;
                for(p.y = pt.y; p.y < pm.y; p.y++)
                {
                    for(p.x = int(fp0.x); p.x <= int(fp1.x); p.x++)
                    {
                        // TODO: Implement improved culling
                        if(p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
                            continue;

                        real const a12 = areaOfTris(p1, p2, p);
                        real const a23 = areaOfTris(p2, p3, p);
                        real const a31 = areaOfTris(p3, p1, p);

                        real const f1 = a23 / totalArea;
                        real const f3 = a12 / totalArea;
                        real const f2 = a31 / totalArea;

                        shadePixel(shader, v1, v2, v3, p, Vector3(f1,f2,f3));
                    }
                    fp0 += f0;
                    fp1 += f1;
                }
            }
            if(pm.y != pb.y) // draw lower half
            {
                assert(pm.y < pb.y);

                real const fdy(real(pm.y - pb.y));

                Vector2 fp0 = (pm.x <  ps.x) ? Vector2(real(pm.x), real(pm.y)) : Vector2(real(ps.x), real(ps.y));
                Vector2 fp1 = (pm.x >= ps.x) ? Vector2(real(pm.x), real(pm.y)) : Vector2(real(ps.x), real(ps.y));

                Vector2 const f0(real(fp0.x - pb.x) / fdy, real(1.0));
                Vector2 const f1(real(fp1.x - pb.x) / fdy, real(1.0));

                Point p;
                for(p.y = pm.y; p.y < pb.y; p.y++)
                {
                    for(p.x = int(fp0.x); p.x <= int(fp1.x); p.x++)
                    {
                        // TODO: Implement improved culling
                        if(p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
                            continue;

                        real const a12 = areaOfTris(p1, p2, p);
                        real const a23 = areaOfTris(p2, p3, p);
                        real const a31 = areaOfTris(p3, p1, p);

                        real const f1 = a23 / totalArea;
                        real const f3 = a12 / totalArea;
                        real const f2 = a31 / totalArea;

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
            if((fz < MinZ) || (fz >= MaxZ))
                return;

            interpolated.pos.z = fz;

            fz -= MinZ;
            fz /= (MaxZ - MinZ);

            depth_t z = depth_t(real(std::numeric_limits<depth_t>::max() * fz));
            if(z > depth(p.x,p.y))
                return;

            Vector2 const uv;

            interpolated.uv = Vector2(coords.x * v1.uv + coords.y * v2.uv + coords.z * v3.uv);

            bool discard = false;

            pixel_t color = shader.shade(interpolated, discard);

            if(discard)
                return;

            pixel(p.x,p.y) = color;
            depth(p.x,p.y) = z;
        }

        template<typename T1, typename T2, typename T3>
        real areaOfTris(T1 const & a, T2 const & b, T3 const & c)
        {
            // https://en.wikipedia.org/wiki/Triangle#Using_coordinates
            return real(0.5) * std::abs((a.x - c.x) * (b.y - a.y) - (a.x - b.x) * (c.y - a.y));
        }
    };
}
