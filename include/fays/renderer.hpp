#pragma once

#include "rect.hpp"

#include "../basic3d/texture.hpp"
#include "../basic3d/renderer.hpp"

#include <cmath>

namespace Fays
{
    //! Renders 2D sprites and primitives with or without depth testing.
    template<int WIDTH, int HEIGHT, typename RenderTarget = Basic3D::Image<WIDTH,HEIGHT>, typename ZBuffer = Basic3D::ZBuffer<WIDTH,HEIGHT,uint16_t>>
    class Renderer :
        public Basic3D::Renderer<WIDTH, HEIGHT, RenderTarget, ZBuffer>
    {
        typedef Basic3D::Texture<pixel_t> Texture;

        struct ClipRect {
            int left, top, right, bottom;
        };
    public:
        explicit Renderer(RenderTarget * target = nullptr, ZBuffer * zbuffer = nullptr) :
            Basic3D::Renderer<WIDTH, HEIGHT, RenderTarget, ZBuffer>(target, zbuffer)
        {

        }

        Renderer(Renderer const &) = delete;
        Renderer(Renderer &&) = delete;

        ~Renderer()
        {

        }

    public:
        //! draws a line
        void drawLine(int x0, int y0, int x1, int y1, pixel_t color)
        {
            // TODO: Adjust with clipping and culling!

            int const dx =  std::abs(x1-x0);
            int const sx = x0 < x1 ? 1 : -1;
            int const dy = -std::abs(y1-y0);
            int const sy = y0 < y1 ? 1 : -1;
            int err = dx+dy;
            int e2; /* error value e_xy */

            while(1)
            {
                pixel(x0, y0) = color;

                if ((x0 == x1) && (y0 == y1))
                    break;

                e2 = 2*err;
                if (e2 > dy) /* e_xy+e_x > 0 */
                {
                    err += dy;
                    x0 += sx;
                }
                if (e2 < dx) /* e_xy+e_y < 0 */
                {
                    err += dx;
                    y0 += sy;
                }
            }
        }

        //! draws a rectangle frame
        void drawRect(Rect const & rect, pixel_t color)
        {
            if(cull(rect.x, rect.y, rect.width, rect.height))
                return;

            ClipRect const clip = clipRect(rect.x, rect.y, rect.width, rect.height);

            if(clip.left == rect.left())
            {
                for(int y = clip.top; y < clip.bottom; y++)
                    pixel(clip.left, y) = color;
            }
            if(clip.right == rect.right())
            {
                for(int y = clip.top; y < clip.bottom; y++)
                    pixel(clip.right - 1, y) = color;
            }

            if(clip.top == rect.top())
            {
                for(int x = clip.left; x < clip.right; x++)
                    pixel(x, clip.top) = color;
            }
            if(clip.bottom == rect.bottom())
            {
                for(int x = clip.left; x < clip.right; x++)
                    pixel(x, clip.bottom - 1) = color;
            }
        }


        //! draws a scaled sprite
        void fillRect(Rect const & rect, pixel_t color)
        {
            if(cull(rect.x, rect.y, rect.width, rect.height))
                return;

            ClipRect const clip = clipRect(rect.x, rect.y, rect.width, rect.height);

            for(int y = clip.top; y < clip.bottom; y++)
            {
                for(int x = clip.left; x < clip.right; x++)
                {
                    pixel(x, y) = color;
                }
            }
        }

        //! draws a non-scaled sprite
        void draw(Texture const & texture, int const x, int const y)
        {
            if(cull(x, y, texture.width, texture.height))
                return;

            ClipRect const clip = clipRect(x, y, texture.width, texture.height);

            for(int sy = clip.top, ty = (clip.top - y); sy < clip.bottom; sy++, ty++)
            {
                for(int sx = clip.left, tx = (clip.left - x); sx < clip.right; sx++, tx++)
                {
                    auto color = texture.at(tx, ty);
                    if(Basic3D::alphaTest(color))
                        pixel(sx, sy) = color;
                }
            }
        }

        //! draws a scaled sprite
        void draw(Texture const & texture, Rect const & rect)
        {
            if(cull(rect.x, rect.y, rect.width, rect.height))
                return;

            ClipRect const clip = clipRect(rect.x, rect.y, rect.width, rect.height);

            for(int y = clip.top; y < clip.bottom; y++)
            {
                for(int x = clip.left; x < clip.right; x++)
                {
                    int const tx = (texture.width * (x - rect.x)) / rect.width;
                    int const ty = (texture.height * (y - rect.y)) / rect.height;

                    auto color = texture.at(tx, ty);
                    if(Basic3D::alphaTest(color))
                        pixel(x, y) = color;
                }
            }
        }

        //! draws a non-scaled sprite with depth testing
        template<bool writeZ>
        void draw(Texture const & texture, depth_t z, int x, int y)
        {
            if(cull(x, y, texture.width, texture.height))
                return;

            ClipRect const clip = clipRect(x, y, texture.width, texture.height);

            for(int sy = clip.top, ty = (left - x); sy < clip.bottom; sy++, ty++)
            {
                for(int sx = clip.left, tx = (top - y); sx < clip.right; sx++, ty++)
                {
                    if(depth(sx, sy) > z)
                        continue;

                    auto color = texture.at(tx, ty);
                    if(!Basic3D::alphaTest(color))
                        continue;

                    pixel(sx, sy) = color;

                    if(writeZ)
                        depth(sx, sy) = z;
                }
            }
        }

        //! draws a scaled sprite with depth testing
        template<bool writeZ>
        void draw(Texture const & texture, depth_t z, Rect const & rect)
        {
            if(cull(rect.x, rect.y, rect.width, rect.height))
                return;

            ClipRect const clip = clipRect(rect.x, rect.y, rect.width, rect.height);

            for(int y = clip.top; y < clip.bottom; y++)
            {
                for(int x = clip.left; x < clip.right; x++)
                {
                    if(depth(sx, sy) > z)
                        continue;

                    int const tx = (texture.width * (x - rect.x)) / rect.width;
                    int const ty = (texture.height * (y - rect.y)) / rect.height;

                    auto color = texture.at(tx, ty);
                    if(!Basic3D::alphaTest(color))
                        continue;

                    pixel(x, y) = color;

                    if(writeZ)
                        depth(sx, sy) = z;
                }
            }
        }

    private:
        //! returns true when the given rectangle is not on the screen
        bool cull(int x, int y, int w, int h)
        {
            if((x >= width) || (y >= height))
                return true;
            if(((x + w) < 0) || (y + h) < 0)
                return true;
            return false;
        }

        //! Calculates the on-screen rectangle of the given rect
        ClipRect clipRect(int x, int y, int w, int h)
        {
            return ClipRect {
                std::max(0, x),
                std::max(0, y),
                std::min(width,  x + w),
                std::min(height, y + h),
            };
        }
    };
}
