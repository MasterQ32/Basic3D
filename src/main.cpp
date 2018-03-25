#include <basic3d.hpp>
#include <irwin3d.hpp>
#include <violent3d.hpp>
#include <fays.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>

// we use this only in here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Basic3D;

const int WIDTH = 320;
const int HEIGHT = 240;

static void writeppm(char const * fileName, Pixel32 const * pixels, int width, int height)
{
    std::ofstream file(fileName, std::ios::binary);
    file << "P6 " << width << " " << height << " 255\n";
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            file.write(reinterpret_cast<char const *>(&pixels[y * width + x].red), 1);
            file.write(reinterpret_cast<char const *>(&pixels[y * width + x].green), 1);
            file.write(reinterpret_cast<char const *>(&pixels[y * width + x].blue), 1);
        }
    }
    file.flush();
}

static Texture<> loadtex(char const * fileName)
{
    int w, h;
    stbi_uc * image = stbi_load(fileName, &w, &h, nullptr, 4);
    if(image == nullptr) {
        std::cout << fileName << " not found!" << std::endl;
        exit(0);
    }

#if defined(BASIC3D_BGRA)
    for(int i = 0; i < w * h; i++)
        std::swap(image[4*i+0], image[4*i+2]);
#endif

    return Texture<>(reinterpret_cast<Pixel32*>(image), w, h);
}

template<int a, int b>
static std::ofstream & operator << (std::ofstream & str, Fixed<a,b> const & f)
{
    str << float(f);
    return str;
}

// typedef Fixed<24, 8> fixed;
typedef float fixed;

static void test_raycaster()
{
    using namespace Irwin3D;

    Image<WIDTH, HEIGHT> image;

    Texture<> floorTex = loadtex("floor.png");
    Texture<> ceilingTex = loadtex("ceiling.png");
    Texture<> wallTex = loadtex("wall.png");
    Texture<> enemyTex = loadtex("enemy.png");

    SimpleScene<Pixel32,fixed> scene;
    scene.getWalls().push_back(Wall<Pixel32,fixed> {
        &wallTex,
        Vector2<fixed>(0.0f, -1.0f),
        Vector2<fixed>(3.0f, -1.0f),
        0,
        3
    });
    scene.getWalls().push_back(Wall<Pixel32,fixed> {
        &wallTex,
        Vector2<fixed>(3.0f, -1.0f),
        Vector2<fixed>(3.0f,  1.0f),
        0,
        3
    });

    std::array<Sprite<Pixel32,fixed>, 1> sprites
    {
        Sprite<Pixel32,fixed> {
            Vector2<fixed>(2.0f, 0.0f),
            &enemyTex
        }
    };

    auto begin = std::chrono::high_resolution_clock::now();

    Irwin3D::Renderer<WIDTH, HEIGHT> renderer(&image);
    renderer.FloorTexture = &floorTex;
    renderer.CeilingTexture = &ceilingTex;
    renderer.drawWalls(scene);
    renderer.drawSprites(sprites);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "raycaster time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us" << std::endl;

    writeppm("result-raycaster.ppm", image.data(), WIDTH, HEIGHT);
}

static void test_rasterizer()
{
    using namespace Violent3D;

    Image<WIDTH, HEIGHT> image;
    image.clear(Colors::clDarkBlue);

    ZBuffer<WIDTH, HEIGHT> zbuffer;
    zbuffer.clear();

    Texture<> floorTex = loadtex("floor.png");
    Texture<> ceilingTex = loadtex("ceiling.png");
    Texture<> wallTex = loadtex("wall.png");
    Texture<> enemyTex = loadtex("enemy.png");

    TextureShader<Pixel32,fixed> mtlWhite = { &enemyTex };

    fixed const size = 48;

    std::array<Vertex<fixed>, 4> vertices
    {
        Vertex<fixed> { Vector3<fixed>(-size, -size, 0.5), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-size,  size, 0.5), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( size, -size, 0.5), Vector2<fixed>(1, 0) },
        Vertex<fixed> { Vector3<fixed>( size,  size, 0.5), Vector2<fixed>(1, 1) },
    };

    auto begin = std::chrono::high_resolution_clock::now();

    Violent3D::Renderer<WIDTH, HEIGHT> renderer(&image, &zbuffer);


    renderer.drawTriangle(mtlWhite, vertices[0], vertices[1], vertices[2]);
    renderer.drawTriangle(mtlWhite, vertices[2], vertices[1], vertices[3]);

    mtlWhite.texture = &wallTex;
    vertices[0].pos.z = 0.0;
    vertices[1].pos.z = 0.0;
    vertices[2].pos.z = 1.0;
    vertices[3].pos.z = 1.0;

    renderer.drawTriangle(mtlWhite, vertices[0], vertices[1], vertices[2]);
    renderer.drawTriangle(mtlWhite, vertices[2], vertices[1], vertices[3]);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "rasterizer time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us" << std::endl;

    writeppm("result-rasterizer.ppm", image.data(), WIDTH, HEIGHT);
}

struct BitmapFont
{
    Texture<> * texture;
    int cx;
    int size;

    struct TexturePointer
    {
        BitmapFont const * font;
        uint16_t c;
        Texture<> operator *()
        {
            return Texture<>(
                font->texture->data + font->texture->width * (font->size * (c / 16)) + font->cx * (c % 16),
                font->cx,
                        font->size,
                16 * font->cx);
        }

        bool operator ==(std::nullptr_t)
        {
            return (font == nullptr);
        }

        bool operator !=(std::nullptr_t)
        {
            return (font != nullptr);
        }
    };

    TexturePointer GetGlyph(uint16_t c, int * advance) const
    {
        if(c >= 128)
            return TexturePointer { nullptr, 0 };
        if(advance != nullptr)
            *advance = cx;
        return TexturePointer { this, c };
    }
};

static void test_spriterender()
{
    using namespace Fays;

    Texture<> solidTex = loadtex("debug.png");
    Texture<> patternTex = loadtex("wall.png");
    Texture<> translucentTex = loadtex("alphatest.png");
    Texture<> bigTex = loadtex("enemy.png");
    Texture<> fontTex = loadtex("courier-11.png");

    BitmapFont font { &fontTex, 8, 11 };

    Image<WIDTH, HEIGHT> image;

    image.clear(Colors::clDarkBlue);

    auto begin = std::chrono::high_resolution_clock::now();

    Fays::Renderer<WIDTH, HEIGHT> renderer(&image);

    renderer.drawRect(Colors::clRed, Rect { 10, 10, WIDTH - 20, HEIGHT - 20 });

    renderer.fillRect(Colors::clWhite, Rect { 12, 12, WIDTH - 24, HEIGHT - 24 });

    renderer.fillRect(patternTex, Rect { 14, 14, WIDTH - 28, HEIGHT - 28 });

    // Draw "L" shape with unscaled
    renderer.draw(solidTex, 14, 14);
    renderer.draw(solidTex, 14, 14+32);
    renderer.draw(solidTex, 14+32, 14+32);

    renderer.draw(translucentTex, 14, 194-32);
    renderer.draw(translucentTex, 14, 194);
    renderer.draw(translucentTex, 14+32, 194);

    renderer.draw(solidTex, Rect { 266, 14, 40, 40 });
    renderer.draw(solidTex, Rect { 266, 54, 40, 40 });
    renderer.draw(solidTex, Rect { 226, 54, 40, 40 });

    renderer.draw(translucentTex, Rect { 266, 146, 40, 40 });
    renderer.draw(translucentTex, Rect { 266, 186, 40, 40 });
    renderer.draw(translucentTex, Rect { 226, 186, 40, 40 });

    // Draw a cross through the bounding rectangle
    // note the adjusted start/end point!
    renderer.drawLine(Colors::clMagenta, 120, 40,  199, 199);
    renderer.drawLine(Colors::clCyan, 120, 199, 199, 40);

    renderer.draw(bigTex, Rect { 160 - 40, 120 - 80, 80, 160 } );

    renderer.drawString(
        "Hello, World!",
        font,
        16,
        (240 - font.size) / 2);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "sprite renderer time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us" << std::endl;

    writeppm("result-sprites.ppm", image.data(), WIDTH, HEIGHT);
}

int _main()
{
    test_raycaster();

    test_rasterizer();

    test_spriterender();

    return 0;
}

#include "common.h"

#define LIVE

namespace stats
{
    using namespace std::chrono;

    static uint64_t meanTime;
    static uint64_t counter;

    static high_resolution_clock::time_point startpoint, endpoint;

    static void begin()
    {
        startpoint = std::chrono::high_resolution_clock::now();
    }

    static void end(char const * what = "???")
    {
        endpoint = std::chrono::high_resolution_clock::now();

        auto count = std::chrono::duration_cast<std::chrono::microseconds>(endpoint - startpoint).count();

        meanTime += count;
        counter ++;

        std::cout << what
                  << " time: "
                  << count
                  << " us / mean: "
                  << (meanTime / counter)
                  << " us"
                  << std::endl;
    }
}

#ifndef LIVE

Texture<> * texlive;

void initFrame(Screen &screen)
{
    using namespace Violent3D;

    screen.clear(Colors::clDimGray);

    ZBuffer<screen.width, screen.height> zbuffer;
    zbuffer.clear();

    static Texture<> floorTex = loadtex("floor.png");
    Texture<> ceilingTex = loadtex("ceiling.png");
    Texture<> wallTex = loadtex("wall.png");
    Texture<> enemyTex = loadtex("enemy.png");

    texlive = &floorTex;

    fixed constexpr sizeX = 48;
    fixed constexpr sizeY = 90;

    std::array<Vertex<fixed>, 4> vertices
    {
        Vertex<fixed> { Vector3<fixed>(-sizeX, -sizeY, 0.5), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-sizeX,  sizeY, 0.5), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( sizeX, -sizeY, 0.5), Vector2<fixed>(1, 0) },
        Vertex<fixed> { Vector3<fixed>( sizeX,  sizeY, 0.5), Vector2<fixed>(1, 1) },
    };
    for(int i = 0; i < vertices.size(); i++)
    {
        vertices[i].pos.x += screen.width / 2;
        vertices[i].pos.y += screen.height / 2;
    }

    Violent3D::Renderer<screen.width, screen.height> renderer(&screen, &zbuffer);

    float angle = 0.0f;
    Vertex<> v[3];
    v[0].uv = Vector2<>(0.0, 0.0);
    v[1].uv = Vector2<>(1.0, 0.0);
    v[2].uv = Vector2<>(0.5, 1.0);
    for(int y = 0; y < 7; y++)
    {
        for(int x = 0; x < 10; x++)
        {
            int const cx = 16 + 32 * x - WIDTH / 2;
            int const cy = 16 + 32 * y - HEIGHT / 2;

            for(int i = 0; i < 3; i++)
            {
                v[i].pos.x = cx + 12 * std::cos(angle + radians(120.0f * i));
                v[i].pos.y = cy + 12 * std::sin(angle + radians(120.0f * i));
                v[i].pos.z = 0.5;
            }

            renderer.drawTriangle(TextureCoordShader<> { }, v[0], v[1], v[2]);

            angle += radians(15.0f);
        }
    }


    writeppm("result-newgen.ppm", screen.data(), screen.width, screen.height);
}

int ticks = 0;

void renderFrame(Screen &screen)
{
    if(ticks++ < 60)
        return;

    using namespace Violent3D;

    ZBuffer<screen.width, screen.height> zbuffer;
    zbuffer.clear();

    Violent3D::Renderer<screen.width, screen.height> renderer(&screen, &zbuffer);

    screen.clear(Colors::clDimGray);

    Vertex<> v[3];
    for(int i = 0; i < 3; i++)
    {
        v[i].pos.x = 80.0f * std::cos(radians(1.0f * ticks + 120.0f * i));
        v[i].pos.y = 80.0f * std::sin(radians(1.0f * ticks + 120.0f * i));
        v[i].pos.z = 0.5f;
    }
    v[0].uv = Vector2<>(0.0, 0.0);
    v[1].uv = Vector2<>(1.0, 0.0);
    v[2].uv = Vector2<>(0.5, 1.0);

    stats::begin();
    auto begin = std::chrono::high_resolution_clock::now();

    renderer.drawTriangle(TextureShader<> { ::texlive }, v[0], v[1], v[2]);

    stats::end("triangle");
}



#else

namespace Live
{
    using namespace Basic3D;
    using namespace Violent3D;

    float angle = 0.0f;

    Violent3D::Renderer<screenSize_X, screenSize_Y> * renderer;

    TextureShader<Pixel32> mtl[3];

    ZBuffer<screenSize_X, screenSize_Y> zbuffer;

    /*
    std::array<Vertex<fixed>, 24> vertices
    {
        Vertex<fixed> { Vector3<fixed>(-1, -1,  1), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-1,  1,  1), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( 1,  1,  1), Vector2<fixed>(1, 1) },
        Vertex<fixed> { Vector3<fixed>( 1, -1,  1), Vector2<fixed>(1, 0) },

        Vertex<fixed> { Vector3<fixed>(-1, -1, -1), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-1,  1, -1), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( 1,  1, -1), Vector2<fixed>(1, 1) },
        Vertex<fixed> { Vector3<fixed>( 1, -1, -1), Vector2<fixed>(1, 0) },

        Vertex<fixed> { Vector3<fixed>(-1, -1, -1), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>(-1, -1,  1), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-1,  1,  1), Vector2<fixed>(1, 0) },
        Vertex<fixed> { Vector3<fixed>(-1,  1, -1), Vector2<fixed>(1, 1) },

        Vertex<fixed> { Vector3<fixed>( 1, -1, -1), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( 1, -1,  1), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>( 1, -1,  1), Vector2<fixed>(1, 0) },
        Vertex<fixed> { Vector3<fixed>( 1, -1, -1), Vector2<fixed>(1, 1) },


        Vertex<fixed> { Vector3<fixed>(-1, -1, -1), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-1, -1,  1), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( 1, -1,  1), Vector2<fixed>(1, 1) },
        Vertex<fixed> { Vector3<fixed>( 1, -1, -1), Vector2<fixed>(1, 0) },

        Vertex<fixed> { Vector3<fixed>(-1,  1, -1), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(-1,  1,  1), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>( 1,  1,  1), Vector2<fixed>(1, 1) },
        Vertex<fixed> { Vector3<fixed>( 1,  1, -1), Vector2<fixed>(1, 0) },
    };

    std::array<int, 36> indices
    {
        0, 1, 3,
        3, 1, 2,

        4, 5, 7,
        7, 5, 6,

        8, 9, 11,
        11, 9, 10,

        12, 13, 15,
        15, 13, 14,

        16, 17, 19,
        19, 17, 18,

        20, 21, 23,
        23, 21, 22,
    };
    */

#include "terrain.hpp"
#include "skydome.hpp"
}

void initFrame(Screen &screen)
{
    test_rasterizer();
    test_raycaster();
    test_spriterender();

    screen.clear(Colors::clBlue);

    using namespace Live;

    static Texture<> floorTex = loadtex("terrain.png");
    static Texture<> ceilingTex = loadtex("sky.png");
    static Texture<> wallTex = loadtex("wall.png");

    mtl[0].texture = &floorTex;
    mtl[1].texture = &ceilingTex;
    mtl[2].texture = &wallTex;

    static Violent3D::Renderer<WIDTH, HEIGHT> ren;
    ren.MinZ = -80;
    ren.MaxZ =  80;
    ren.setZBuffer(&zbuffer);

    renderer = &ren;
}

static Basic3D::Matrix4<> matrix =
{
    -0.4552882f, -0.3561772f, -0.9138938f, -0.9137153f,
    0.0f, 1.689785f, -0.219609f, -0.2195661f,
    -1.21664f, 0.1332879f, 0.3419953f, 0.3419285f,
    -0.9931068f, -3.696177f, 5.849844f, 6.048681f,
};

template<typename Shader, typename T1, typename T2, int sizeA, int sizeB>
static void drawModel(
    Shader const & shader,
    std::array<Violent3D::Vertex<T1>,sizeA> const & vertices,
    std::array<T2,sizeB> const & indices)
{
    static std::array<Violent3D::Vertex<>, sizeA> transformed;
    for(int i = 0; i < vertices.size(); i++)
    {
        transformed[i].pos = Basic3D::transformPerspective(vertices[i].pos, matrix);
        transformed[i].uv = vertices[i].uv;

        transformed[i].pos.x *= (screenSize_X / 2);
        transformed[i].pos.y *= -(screenSize_Y / 2);
    }

    for(int i = 0; i < indices.size(); i += 3)
        Live::renderer->drawTriangle(shader, transformed[indices[i+0]], transformed[indices[i+1]], transformed[indices[i+2]]);
}


uint64_t meanTime;
uint64_t counter;

void renderFrame(Screen &screen)
{
    using namespace Live;

    renderer->setRenderTarget(&screen);

    zbuffer.clear();
    screen.clear(0x58, 0x7c, 0xef);

    stats::begin();

    drawModel(mtl[0], terrain::vertices, terrain::indices);

    drawModel(mtl[1], skydome::vertices, skydome::indices);

    stats::end("rasterizer");
}

#endif
