#pragma once

namespace Basic3D
{
    template<int WIDTH, int HEIGHT, typename RenderTarget, typename ZBuffer>
    class Renderer
    {
    public:
        static constexpr int width  = WIDTH;
        static constexpr int height = HEIGHT;

        typedef typename RenderTarget::pixel_t pixel_t;
        typedef typename ZBuffer::depth_t      depth_t;
    private:
        RenderTarget * renderTarget;
        ZBuffer * zbuffer;
    protected:
        Renderer(RenderTarget * target = nullptr, ZBuffer * zbuffer = nullptr) :
            renderTarget(target),
            zbuffer(zbuffer)
        {

        }
    public:
        void setRenderTarget(RenderTarget * target) {
            this->renderTarget = target;
        }
        RenderTarget * getRenderTarget() const {
            return this->renderTarget;
        }

        void setZBuffer(ZBuffer * target) {
            this->zbuffer = target;
        }
        ZBuffer * getZBuffer() const {
            return this->zbuffer;
        }

    protected:
        void setPixel(int x, int y, pixel_t const & color) {
            renderTarget->setPixel(x, y, color);
        }

        pixel_t getPixel(int x, int y) {
            return renderTarget->getPixel(x, y);
        }

        void setDepth(int x, int y, depth_t const & depth) {
            zbuffer->setDepth(x, y, depth);
        }

        depth_t getDepth(int x, int y) {
            return zbuffer->getDepth(x, y);
        }
    };
}
