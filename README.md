# Basic3D
A collection of software renderers and graphics tools in modern C++

## Irwin3D
A basic raycaster that allows rendering of wall segments as well as sprites.
Has a primitive z-buffer system for sprite-wall-clipping.

### Example
This is a minimal example on how to use Irwin3D:
```cpp
// provide some storage for our target image
std::array<pixel_t, 320 * 240> image;

// create a scene (in this case a list of walls)
SimpleScene<> scene;
scene.getWalls().push_back(Wall<> {
    nullptr,
    Vector2<>(0.0f, -1.0f),
    Vector2<>(3.0f, -1.0f),
    0.0f,
    3.0f
});
scene.getWalls().push_back(Wall<> {
    nullptr,
    Vector2<>(3.0f, -1.0f),
    Vector2<>(3.0f,  1.0f),
    0.0f,
    3.0f
});

// instantiate a renderer and draw the scene
Renderer<320, 240> renderer;
renderer.drawWalls(scene, image.data());

// not included in Basic3D
writeppm("result.ppm", image.data(), 320, 240);
```