#pragma once

#include "pixel.hpp"
#include "texture.hpp"

#include <array>

namespace Basic3D
{
    template<int _width, int _height>
    class Image :
        public Texture
    {
    private:
        std::array<pixel_t, _width * _height> pixels;
    public:
        Image() : Texture(pixels.data(), _width, _height)
        {

        }

    public: // operators
        //! clears the image to the given rgb values
        void clear(int r, int g, int b) {
            this->clear(pixel_t(r,g,b));
        }

        //! clears the image to the given color
        void clear(pixel_t color) {
            for(int i = 0; i < (_width * _height); i++)
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
            return &this->pixels[y * _width];
        }

        pixel_t const * scanline(int y) const {
            return &this->pixels[y * _width];
        }

        //! gets a pixel value
        pixel_t & at(int x, int y) {
            return this->scanline(y)[x];
        }

        //! gets a pixel value
        pixel_t const & at(int x, int y) const {
            return this->scanline(y)[x];
        }
    };
}
