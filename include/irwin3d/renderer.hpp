#pragma once

#include "../basic3d/vec2.hpp"
#include "../basic3d/utils.hpp"
#include "../basic3d/texture.hpp"
#include "wall.hpp"
#include "sprite.hpp"
#include "scene.hpp"

#include <array>
#include <limits>
#include <algorithm>

namespace Irwin3D
{
    template<int width, int height, typename T = Basic3D::real_t>
    class Renderer
    {
        typedef Basic3D::Vector2<T> vec2_t;
        typedef Basic3D::Texture Texture;
        typedef Basic3D::pixel_t pixel_t;
    private:
        static constexpr T aspect = T(width) / T(height);
    private:
        std::array<T, width> zbuffer;
        std::array<vec2_t, width> protorays;

    public:
        vec2_t CameraPosition;
        T CameraRotation;

        pixel_t CeilingColor;
        pixel_t WallColor;
        pixel_t FloorColor;

        Texture const * CeilingTexture;
        Texture const * FloorTexture;

    public:
        Renderer() :
            zbuffer(),
            protorays(),
            CameraPosition(0,0),
            CameraRotation(0),
            CeilingColor(0x64, 0x95, 0xED),
            WallColor(0xB2, 0x22, 0x22),
            FloorColor(0x8B, 0x45, 0x13),
            CeilingTexture(nullptr),
            FloorTexture(nullptr)
        {
            for(int x = 0; x < width; x++)
            {
                T fx = aspect * (T(2.0) * (x / T(width - 1)) - T(1.0));

                T deltaAngle = std::atan(T(0.5) * fx);

                protorays[x] = rotate(vec2_t::UnitX, deltaAngle);

                // set length of x to 1 for early correct perspective correction
                protorays[x] *= (T(1.0) / protorays[x].x);
            }
        }

        Renderer(Renderer const &) = delete;
        Renderer(Renderer &&) = delete;
        ~Renderer() = default;

    public:

        template<typename TScene>
        void drawWalls(TScene const & scene, pixel_t * const pixels)
        {
            // render the walls
            for (int x = 0; x < width; x++)
            {
                // rotate our precalculated ray
                vec2_t const dir(rotate(protorays[x], this->CameraRotation));

                // Raycast here
                RaycastResult<T> const * const result = scene.castRay(this->CameraPosition, dir);

                int wallHeight = 0;
                Texture const * texture = nullptr;
                if (result != nullptr)
                {
                    // if (result->distance < 0.01)
                    //     continue; // znear discard

                    // project the wall height onto the screen and
                    // adjust the zbuffer
                    wallHeight = int(height / result->distance + 0.5);
                    texture = result->wall->texture;
                    zbuffer[x] = result->distance;
                }
                else
                {
                    // no hit means infinite distance
                    zbuffer[x] = std::numeric_limits<T>::infinity();
                }

                // calculate screen boundaries of the wall
                int const wallTop = (height / 2) - (wallHeight / 2);
                int const wallBottom = (height / 2) + (wallHeight / 2);

                // draw the ceiling with either texture or color
                if (CeilingTexture != nullptr)
                {
                    for (int y = 0; y < wallTop; y++)
                    {
                        T const fy = 1.0f - ((y + 1) / T(height / 2));
                        T const d = 1.0f / fy;

                        vec2_t const pos(CameraPosition + dir * d);

                        int const u(int((CeilingTexture->width - 1) * fract(pos.x)));
                        int const v(int((CeilingTexture->height - 1) * fract(pos.y)));

                        pixels[y * width + x] = CeilingTexture->sample(u, v);
                    }
                }
                else
                {
                    for (int y = 0; y < wallTop; y++)
                        pixels[y * width + x] = CeilingColor;
                }

                // draw the wall
                int const maxy = std::min(height, wallBottom);
                if(texture != nullptr)
                {
                    for (int y = std::max(0, wallTop); y < maxy; y++)
                    {
                        int const u = (int)((texture->width - 1) * fract(result->u));
                        int const v = texture->height * (y - wallTop) / wallHeight;
                        pixels[y * width + x] = texture->sample(u, v);
                    }
                }
                else
                {
                    for (int y = std::max(0, wallTop); y < maxy; y++)
                        pixels[y * width + x] = this->WallColor;
                }

                // draw the ground with either texture or color
                if (FloorTexture != nullptr)
                {
                    for (int y = wallBottom; y < height; y++)
                    {
                        T const fy = ((y - height / 2 + 1) / T(height / 2));

                        T const d = 1.0f / fy;

                        vec2_t const pos(CameraPosition + dir * d);

                        int const u(int((FloorTexture->width - 1) * fract(pos.x)));
                        int const v(int((FloorTexture->height - 1) * fract(pos.y)));

                        pixels[y * width + x] = FloorTexture->sample(u, v);
                    }
                }
                else
                {
                    for (int y = wallBottom; y < height; y++)
                        pixels[y * width + x] = FloorColor;
                }
            }
        }

        template<typename SpriteCollection>
        void sortSprites(SpriteCollection & sprites)
        {
            std::sort(std::begin(sprites), std::end(sprites), [this](Sprite<T> const & a, Sprite<T> const & b) {
                // "a < b"
                return distance2(a.position, this->CameraPosition) < distance2(b.position, this->CameraPosition);
            });
        }

        template<typename SpriteCollection>
        void drawSprites(SpriteCollection const & sprites, pixel_t * const pixels)
        {
            // then draw the sprites
            for(auto const & sprite : sprites)
            {
                auto delta = sprite.position - this->CameraPosition;
                auto angle = angdiff(std::atan2(delta.y, delta.x), this->CameraRotation);

                if (std::abs(angle) > PiOver2)
                    continue;

                auto distance2 = length2(delta);
                if (distance2 < 0.0025f) // 0.05²
                    continue; // discard early

                auto distance = std::sqrt(distance2);

                // if(distance > 100)
                //  continue; // discard far objects

                auto fx = 2.0 * std::tan(angle) / aspect;

                auto cx = int((width - 1) * (0.5 + 0.5 * fx));

                auto texture = sprite.texture;

                // calculate perspective correction
                auto correction = std::sqrt(0.5f * fx * fx + 1);

                // calculate on-screen size
                auto spriteHeight = int(correction * height / distance);
                auto spriteWidth = (texture->width * spriteHeight) / texture->height;

                // discard the sprite when out of screen
                if ((cx + spriteWidth) < 0)
                    continue;
                if ((cx - spriteWidth) >= width)
                    continue;

                // calculate screen positions and boundaries
                auto wallTop = (height / 2) - (spriteHeight / 2);
                auto wallBottom = (height / 2) + (spriteHeight / 2);

                auto left = cx - spriteWidth / 2;

                auto minx = std::max(0, left);
                auto maxx = std::min(width - 1, cx + spriteWidth / 2);

                auto miny = std::max(0, wallTop);
                auto maxy = std::min(height, wallBottom);

                // render the sprite also column major
                for (int x = minx; x < maxx; x++)
                {
                    // Test if we are occluded by a sprite
                    if (zbuffer[x] < distance)
                        continue;

                    auto u = (texture->width - 1) * (x - left) / (spriteWidth - 1);

                    for (int y = miny; y < maxy; y++)
                    {
                        auto v = (texture->height - 1) * (y - wallTop) / (spriteHeight - 1);
                        pixel_t c = texture->sample(u, v);

                        // alpha testing
                        if ((c.alpha & 0x80) == 0)
                            continue;

                        pixels[y * width + x] = c;
                    }
                }
            }
        }
    };
}
