#pragma once

#include "pixel32.hpp"

namespace Basic3D
{
    //! A basic texture type that allows sampling of pixels.
    template<typename Pixel = Basic3D::Pixel32>
    class Texture
    {
    public:
        typedef Pixel pixel_t;
    public:
        int width;
        int height;
        int stride;
        pixel_t const * data;

    public:
        Texture() = default;
        Texture(pixel_t const * img, int w, int h, int stride = 0) : width(w), height(h), stride((stride == 0) ? w : stride), data(img) { }
        Texture(Texture const &) = delete;
        Texture(Texture && other) : width(other.width), height(other.height), data(other.data) { }
        ~Texture() = default;

        //! gets a pixel value
        pixel_t at(int x, int y) const {
            return this->data[y * this->stride + x];
        }

        //! similar to @ref at, but does wrapping
        pixel_t sample(int x, int y) const {
            return this->data[(y % this->height) * this->stride + (x % this->width)];
        }

        size_t size() const {
            return sizeof(pixel_t) * stride * height;
        }
    };
}
