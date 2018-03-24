#pragma once

#include "pixel.hpp"
#include "texture.hpp"

#include <array>

namespace Basic3D
{
    template<int WIDTH, int HEIGHT, typename Pixel = Basic3D::Pixel32>
    class Image :
        public Texture<Pixel>
    {
    public:
        typedef Pixel pixel_t;
    private:
        std::array<pixel_t, WIDTH * HEIGHT> pixels;
    public:
        Image() : Texture(pixels.data(), WIDTH, HEIGHT)
        {

        }

    public: // operators
        //! clears the image to the given rgb values
        void clear(int r, int g, int b) {
            this->clear(pixel_t(r,g,b));
        }

        //! clears the image to the given color
        void clear(pixel_t color) {
            for(int i = 0; i < (WIDTH * HEIGHT); i++)
                this->pixels[i] = color;
        }

    public: // data accessors
        pixel_t * data() {
            return this->pixels.data();
        }

        pixel_t const * data() const {
            return this->pixels.data();
        }

        pixel_t * scanline(int y) {
            return &this->pixels[y * WIDTH];
        }

        pixel_t const * scanline(int y) const {
            return &this->pixels[y * WIDTH];
        }

        //! gets a pixel value
        pixel_t & pixel(int x, int y) {
            return this->scanline(y)[x];
        }

        //! gets a pixel value
        pixel_t const & pixel(int x, int y) const {
            return this->scanline(y)[x];
        }

        //! returns the size of the image in bytes.
        constexpr size_t size() const {
            return sizeof(pixel_t) * WIDTH * HEIGHT;
        }
    };
}
