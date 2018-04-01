#pragma once

#include "pixel32.hpp"
#include "texture.hpp"

#include <array>

namespace Basic3D
{
    //! Provides a basic render target that
    //! allows getting and setting pixel values.
    //! It also implements a texture so it can be used in another render process.
    template<int WIDTH, int HEIGHT, typename Pixel = Basic3D::Pixel32>
    class Image :
        public Texture<Pixel>
    {
    public:
        typedef Pixel pixel_t;
        static constexpr int width  = WIDTH;
        static constexpr int height = HEIGHT;
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
        //! Returns a pointer to the first pixel
        pixel_t * data() {
            return this->pixels.data();
        }

        //! Returns a pointer to the first pixel
        pixel_t const * data() const {
            return this->pixels.data();
        }

        //! Returns a pointer to the given scanline
        pixel_t * scanline(int y) {
            return &this->pixels[y * WIDTH];
        }

        //! Returns a pointer to the given scanline
        pixel_t const * scanline(int y) const {
            return &this->pixels[y * WIDTH];
        }

        //! sets a pixel value
        void setPixel(int x, int y, pixel_t const & color) {
            this->scanline(y)[x] = color;
        }

        //! gets a pixel value
        pixel_t const & getPixel(int x, int y) const {
            return this->scanline(y)[x];
        }

        //! returns the size of the image in bytes.
        constexpr size_t size() const {
            return sizeof(pixel_t) * WIDTH * HEIGHT;
        }
    };
}
