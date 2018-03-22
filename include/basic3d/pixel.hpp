#pragma once

#include <cstdint>

namespace Basic3D
{
    struct pixel_t
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

        pixel_t() = default;

        pixel_t(uint32_t value) : value(value) { }

#if defined(BASIC3D_BGRA)
        pixel_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) : blue(b), green(g), red(r), alpha(a) { }
#else
        pixel_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF) : red(r), green(g), blue(b), alpha(a) { }
#endif

        pixel_t(pixel_t const &) = default;
    };

    static_assert(sizeof(pixel_t)        == 4, "pixel_t must be of size 4!");
    static_assert(sizeof(pixel_t::value) == 4, "pixel_t::value must be of size 4!");
    static_assert(sizeof(pixel_t::red)   == 1, "pixel_t::red must be of size 1!");
    static_assert(sizeof(pixel_t::green) == 1, "pixel_t::green must be of size 1!");
    static_assert(sizeof(pixel_t::blue)  == 1, "pixel_t::blue must be of size 1!");
    static_assert(sizeof(pixel_t::alpha) == 1, "pixel_t::alpha must be of size 1!");
}
