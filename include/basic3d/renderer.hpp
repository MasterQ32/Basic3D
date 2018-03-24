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
        pixel_t & pixel(int x, int y) {
            return renderTarget->pixel(x, y);
        }

        depth_t & depth(int x, int y) {
            return zbuffer->depth(x, y);
        }
    };
}
