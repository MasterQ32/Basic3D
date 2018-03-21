#pragma once

#include "pixel.hpp"

namespace Basic3D
{
    class Texture
    {
    public:
        pixel_t const * const data;
        int const width;
        int const height;

    public:
        Texture(pixel_t const * img, int w, int h) : data(img), width(w), height(h) { }
        Texture(Texture const &) = delete;
        Texture(Texture && other) : data(other.data), width(other.width), height(other.height) { }
        ~Texture() = default;

        //! gets a pixel value
        pixel_t at(int x, int y) const {
            return this->data[y * this->width + x];
        }

        //! similar to @ref at, but does wrapping
        pixel_t sample(int x, int y) const {
            return this->data[(y % this->height) * this->width + (x % this->width)];
        }
    };
}
