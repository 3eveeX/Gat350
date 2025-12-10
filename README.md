Gat350 is a lightweight C++ rendering framework designed to mimic the core functionality of a modern game engine.
It contains  an OpenGL rendering pipeline, allowing you to explore rendering techniques at multiple levels of abstraction.

The project provides support for loading models, applying materials, handling multiple light types, and running post-processing effects—all within a clean educational environment.



**Features**
 - OpenGL real-time renderer

 - Vertex transformation pipeline

 - UV mapping and texture sampling

 - Back-face culling and clipping

 - Per-vertex and per-pixel shading

_**Supports multiple classic shading techniques**_:

Gouraud Shading:

 - Lighting computed at vertices

 - Smooth colour interpolation

 - Fast and efficient

Phong Shading:

 - Per-pixel normal interpolation

 - Accurate specular highlights

 - More realistic than Gouraud

**Shader Support**

 - Custom vertex processing (model, view, projection transforms)

 - Custom fragment shader functions

 - Normal mapping (if enabled)

 - Basic texture sampling

 - Specular, diffuse, and ambient lighting

**Material System**

 - Diffuse color

 - Specular exponent

 - Ambient term

 - Textured and untextured materials

 - Support for transparency and colour blending

 - Simple Blinn–Phong lighting model

**Post-Processing**

Includes a programmable post-processing stage with effects like:

 - Greyscale

 - Invert

 - Scanlines

 - Grain

 - Colour tint

 - Outlines based on the normal map of a texture



**Prerequisites**

Make sure the following dependencies are installed:

 - Visual Studio 2022 (or newer)

 - SDL3

 - GLM

 - GLAD

 - OpenGL 4.3+ capable GPU

 - C++17 or later



Setting Up in Visual Studio:

Open the .sln file in Visual Studio.

Ensure the project uses:

x64 configuration

Debug or Release build

Add required include directories:

SDL3 include path

GLM include path

Link SDL3 libraries (if displaying rendered output):

SDL3.lib

Build the project using Build → Build Solution.



**Software Renderer Pipeline**

1. Vertex Processing

 - Model → World → View → Projection

 - Perspective divide

 - Screen-space conversion

2. Clipping

 - Against near plane and screen bounds

3. Rasterisation

 - Barycentric triangle rasterisation

 - Depth buffer

 - Texture coordinate interpolation

4. Fragment Shading

 - Gouraud OR Phong shading

 - Diffuse + specular + ambient

 - Texture sampling

 - Optional normal maps

**Attribution**

This project uses the following libraries:

SDL3

GLM 

OpenGL

GLAD 

Influenced by:

Custom course content from GAT350

**Conclusion**

Gat350 is a hands-on exploration of rendering technology—covering both software rasterisation and hardware-accelerated OpenGL rendering.
Feel free to explore the project, study the pipeline, or extend it with your own features.
Suggestions, issues, and improvements are always welcome!
