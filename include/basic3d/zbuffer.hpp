#pragma once

#include <cstdint>
#include <array>
#include <limits>

namespace Basic3D
{
    template<int WIDTH, int HEIGHT, typename Depth = std::uint16_t>
    class ZBuffer
    {
    public:
        typedef Depth depth_t;
    private:
        std::array<depth_t, WIDTH * HEIGHT> zvalues;
    public:
        ZBuffer() : zvalues() {
            this->clear();
        }

        void clear(depth_t depth = std::numeric_limits<depth_t>::max()) {
            for(int i = 0; i < WIDTH * HEIGHT; i++)
                this->zvalues[i] = depth;
        }

    public:
        depth_t & depth(int x, int y) {
            return zvalues[y * WIDTH + x];
        }

        depth_t const & depth(int x, int y) const {
            return zvalues[y * WIDTH + x];
        }
    };
}
