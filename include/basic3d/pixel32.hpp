#pragma once

#include <cstdint>

namespace Basic3D
{
    //! 32 bit wide pixel format with either BGRA or RGBA pixels.
    struct Pixel32
    {
        union {
            struct {
#if defined(BASIC3D_BGRA)
                uint8_t blue;
                uint8_t green;
                uint8_t red;
#else
                uint8_t red;
                uint8_t green;
                uint8_t blue;
#endif
                uint8_t alpha;
            };
            uint32_t value;
        };

        Pixel32() = default;

        Pixel32(uint32_t value) : value(value) { }

#if defined(BASIC3D_BGRA)
        Pixel32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) : blue(b), green(g), red(r), alpha(a) { }
#else
        Pixel32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) : red(r), green(g), blue(b), alpha(a) { }
#endif

        Pixel32(Pixel32 const &) = default;
    };

    //! Returns true if the given pixel value is non-transparent.
    static inline bool alphaTest(Pixel32 const & pixel) {
        return (pixel.alpha & 0x80);
    }

    static_assert(sizeof(Pixel32)        == 4, "pixel_t must be of size 4!");
    static_assert(sizeof(Pixel32::value) == 4, "pixel_t::value must be of size 4!");
    static_assert(sizeof(Pixel32::red)   == 1, "pixel_t::red must be of size 1!");
    static_assert(sizeof(Pixel32::green) == 1, "pixel_t::green must be of size 1!");
    static_assert(sizeof(Pixel32::blue)  == 1, "pixel_t::blue must be of size 1!");
    static_assert(sizeof(Pixel32::alpha) == 1, "pixel_t::alpha must be of size 1!");
}
