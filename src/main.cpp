#include <basic3d.hpp>
#include <irwin3d.hpp>
#include <violent3d.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>

using namespace Basic3D;

const int WIDTH = 320;
const int HEIGHT = 240;

static void writeppm(char const * fileName, pixel_t const * pixels, int width, int height)
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

static Texture loadtex(char const * fileName, int w, int h)
{
    std::ifstream file(fileName, std::ios::binary);
    if(!file.good()) {
        std::cout << fileName << " not found!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::unique_ptr<pixel_t[]> pixels(std::make_unique<pixel_t[]>(w * h));
    file.read(reinterpret_cast<char*>(pixels.get()), w * h * h);
    file.close();
    return Texture(pixels.release(), w, h);
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

    Texture floorTex = loadtex("floor.tex", 32, 32);
    Texture ceilingTex = loadtex("ceiling.tex", 32, 32);
    Texture wallTex = loadtex("wall.tex", 32, 32);
    Texture enemyTex = loadtex("enemy.tex", 64, 120);

    SimpleScene<fixed> scene;
    scene.getWalls().push_back(Wall<fixed> {
        &wallTex,
        Vector2<fixed>(0.0f, -1.0f),
        Vector2<fixed>(3.0f, -1.0f),
        0,
        3
    });
    scene.getWalls().push_back(Wall<fixed> {
        &wallTex,
        Vector2<fixed>(3.0f, -1.0f),
        Vector2<fixed>(3.0f,  1.0f),
        0,
        3
    });

    std::array<Sprite<fixed>, 1> sprites
    {
        Sprite<fixed> {
            Vector2<fixed>(2.0f, 0.0f),
            &enemyTex
        }
    };

    auto begin = std::chrono::high_resolution_clock::now();

    Renderer<WIDTH, HEIGHT, fixed> renderer(image);
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

    image.clear(0x00, 0x00, 0x80);

    Texture floorTex = loadtex("floor.tex", 32, 32);
    Texture ceilingTex = loadtex("ceiling.tex", 32, 32);
    Texture wallTex = loadtex("wall.tex", 32, 32);
    Texture enemyTex = loadtex("enemy.tex", 64, 120);

    Material mtlWhite = { pixel_t(0xFF, 0xFF, 0xFF), &enemyTex };

    std::array<Vertex<fixed>, 4> vertices
    {
        Vertex<fixed> { Vector3<fixed>(16, 16, 0.5), Vector2<fixed>(0, 0) },
        Vertex<fixed> { Vector3<fixed>(16, 48, 0.5), Vector2<fixed>(0, 1) },
        Vertex<fixed> { Vector3<fixed>(48, 16, 0.5), Vector2<fixed>(1, 0) },
        Vertex<fixed> { Vector3<fixed>(48, 48, 0.5), Vector2<fixed>(1, 1) },
    };

    auto begin = std::chrono::high_resolution_clock::now();

    Renderer<WIDTH, HEIGHT> renderer(image);

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

int main()
{
    test_raycaster();

    test_rasterizer();

    return 0;
}
