#pragma once

#include <cstdint>
#include <array>
#include <limits>

namespace Basic3D
{
    //! Provides a basic z-buffer
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
        void setDepth(int x, int y, depth_t const & value)  {
            zvalues[y * WIDTH + x] = value;
        }

        depth_t const & getDepth(int x, int y) const {
            return zvalues[y * WIDTH + x];
        }
    };
}
