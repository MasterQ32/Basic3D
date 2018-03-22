# Basic3D
A collection of software renderers and graphics tools in modern C++

## Irwin3D
A basic raycaster that allows rendering of wall segments as well as sprites.
Has a primitive z-buffer system for sprite-wall-clipping.

![Preview Irwin3D](https://puu.sh/zMp5I/9972bf1406.png)

### Example
This is a minimal example on how to use Irwin3D:
```cpp
// provide some storage for our target image
Image<320,240> image;

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
Renderer<320, 240> renderer(image);
renderer.drawWalls(scene);

// not included in Basic3D
writeppm("result.ppm", image.data(), 320, 240);
```

## Violent3D
A basic triangle rasterizer for 3D rendering. Allows to draw either textured or single-colored polygons.

![Preview Violent3D](https://puu.sh/zMNVC/6c66c35a02.png)

### Example 
How to draw a triangle
```cpp
Image<320, 240> image;
image.clear(0x00, 0x00, 0x80);

Renderer<320, 240> renderer(image);

// setup a material for rendering with color
Material mtlWhite = { pixel_t(0xFF, 0xFF, 0xFF), nullptr };
renderer.Material = &mtlWhite;

// rasterize the quad
std::array<Vertex<fixed>, 4> vertices
{
	Vertex<fixed> { Vector3<fixed>(16, 16, 0.5), Vector2<fixed>(0, 0) },
	Vertex<fixed> { Vector3<fixed>(16, 48, 0.5), Vector2<fixed>(0, 1) },
	Vertex<fixed> { Vector3<fixed>(48, 16, 0.5), Vector2<fixed>(1, 0) },
	Vertex<fixed> { Vector3<fixed>(48, 48, 0.5), Vector2<fixed>(1, 1) },
};
renderer.drawTriangle(vertices[0], vertices[1], vertices[2]);
renderer.drawTriangle(vertices[2], vertices[1], vertices[3]);

writeppm("result-rasterizer.ppm", image.data(), 320, 240);
```