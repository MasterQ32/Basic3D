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

    Texture<> floorTex = loadtex("floor.png");
    Texture<> ceilingTex = loadtex("ceiling.png");
    Texture<> wallTex = loadtex("wall.png");
    Texture<> enemyTex = loadtex("enemy.png");

    Material<Pixel32> mtlWhite = { Pixel32(0xFF, 0xFF, 0xFF), &enemyTex };

    std::array<Vertex<fixed>, 4> vertices
    {
        Vertex<fixed> { Vector3<fixed>(16, 16, 0.5), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(16, 48, 0.5), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>(48, 16, 0.5), Vector2<fixed>(1, 0) },
        Vertex<fixed> { Vector3<fixed>(48, 48, 0.5), Vector2<fixed>(1, 1) },
    };

    auto begin = std::chrono::high_resolution_clock::now();

    Violent3D::Renderer<WIDTH, HEIGHT> renderer(&image);

    renderer.Material = &mtlWhite;

    renderer.drawTriangle(vertices[0], vertices[1], vertices[2]);
    renderer.drawTriangle(vertices[2], vertices[1], vertices[3]);

    mtlWhite.Texture = &wallTex;
    vertices[0].pos.z = 1.0;
    vertices[1].pos.z = 0.0;
    vertices[2].pos.z = 0.0;
    vertices[3].pos.z = 1.0;

    renderer.drawTriangle(vertices[0], vertices[1], vertices[2]);
    renderer.drawTriangle(vertices[2], vertices[1], vertices[3]);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "rasterizer time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us" << std::endl;

    writeppm("result-rasterizer.ppm", image.data(), WIDTH, HEIGHT);
}

static void test_spriterender()
{
    using namespace Fays;

    Texture<> solidTex = loadtex("floor.png");
    Texture<> translucentTex = loadtex("alphatest.png");
    Texture<> bigTex = loadtex("enemy.png");

    Image<WIDTH, HEIGHT> image;

    image.clear(Colors::clDarkBlue);

    auto begin = std::chrono::high_resolution_clock::now();

    Fays::Renderer<WIDTH, HEIGHT> renderer(&image);

    renderer.drawRect(Rect { 10, 10, WIDTH - 20, HEIGHT - 20 }, Colors::clRed);
    renderer.fillRect(Rect { 12, 12, WIDTH - 24, HEIGHT - 24 }, Colors::clWhite);

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
    renderer.drawLine(120, 40,  199, 199, Colors::clMagenta);
    renderer.drawLine(120, 199, 199, 40, Colors::clCyan);

    renderer.draw(bigTex, Rect { 160 - 40, 120 - 80, 80, 160 } );

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

namespace Live
{
    using namespace Basic3D;
    using namespace Violent3D;

    float angle = 0.0f;

    Violent3D::Renderer<screenSize_X, screenSize_Y> * renderer;

    Material<Pixel32> * mtl[3];

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

    static Material<Pixel32> mtl0 = { Pixel32(0xFF, 0xFF, 0xFF), &floorTex };
    static Material<Pixel32> mtl1 = { Pixel32(0xFF, 0xFF, 0xFF), &ceilingTex };
    static Material<Pixel32> mtl2 = { Pixel32(0xFF, 0xFF, 0xFF), &wallTex };

    mtl[0] = &mtl0;
    mtl[1] = &mtl1;
    mtl[2] = &mtl2;

    static Violent3D::Renderer<WIDTH, HEIGHT> ren;
    ren.MinZ = -80;
    ren.MaxZ =  80;
    ren.setZBuffer(&zbuffer);

    renderer = &ren;
}

static Basic3D::Matrix4<> matrix =
{
    -0.4552882, -0.3561772, -0.9138938, -0.9137153,
    0, 1.689785, -0.219609, -0.2195661,
    -1.21664, 0.1332879, 0.3419953, 0.3419285,
    -0.9931068, -3.696177, 5.849844, 6.048681,
};

template<typename T1, typename T2, int sizeA, int sizeB>
static void drawModel(
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
        Live::renderer->drawTriangle(transformed[indices[i+0]], transformed[indices[i+1]], transformed[indices[i+2]]);
}

void renderFrame(Screen &screen)
{
    using namespace Live;

    /*
    angle += 0.01f;
    for(int i = 0; i < terrain::vertices.size(); i++)
    {
        auto & v = vertices[i];

        auto xy = rotate(Vector2<>(v.pos.x, v.pos.y), 0.01f);
        v.pos.x = xy.x;
        v.pos.y = xy.y;

        auto xz = rotate(Vector2<>(v.pos.x, v.pos.z), 0.015f);
        v.pos.x = xz.x;
        v.pos.z = xz.y;

        auto yz = rotate(Vector2<>(v.pos.y, v.pos.z), 0.020f);
        v.pos.y = yz.x;
        v.pos.z = yz.y;
    }
    */

    renderer->setRenderTarget(&screen);

    auto begin = std::chrono::high_resolution_clock::now();

    zbuffer.clear();
    screen.clear(0x58, 0x7c, 0xef);

    renderer->Material = mtl[0];
    drawModel(terrain::vertices, terrain::indices);

    renderer->Material = mtl[1];
    drawModel(skydome::vertices, skydome::indices);



    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "rasterizer time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us" << std::endl;

}

