# Technical Development Report: 3D Planetary System
## 1. Project Objectives
The main objective of this project is to develop an interactive 3D planetary system simulation using modern OpenGL (Core Profile). The system allows the user to dynamically edit the scale and translation speed of each planet in real time, while keeping the central star (the Sun) as a fixed reference point in the center of the world.

The project is structured and delivered in 10 separate, incremental, and fully compilable stages to show the chronological evolution of the software.


## 2. Chronological Development Phases 

***Stage 1 (Window Configuration):** Setup and initialization of the graphics window and the main execution loop using the SFML library.
***Stage 2 (Primitive Drawing):** Implementation of the 5 planets represented as flat 2D circles positioned at the center of the system.
***Stage 3 (Intrinsic Rotation):** Application of mathematical transformations to make the flat 2D circles rotate continuously around their own local axes.
***Stage 4 (3D Sphere Generation):** Transition from flat 2D primitives to full 3D environments by generating procedural sphere meshes using spherical coordinates.
***Stage 5 (Orbital Physics):** Integration of transformation matrices via GLM to make the 3D spheres orbit dynamically around the central Sun, utilizing custom shaders.
***Stage 6 (Basic Camera Interaction):** Implementation of a basic navigation system (orbiting camera) around the origin, capturing keyboard inputs to control the view's angle and distance.
***Stage 7 (Solid Rendering):** Transition from wireframe rendering to solid rasterization, allowing OpenGL to fill the triangles.
***Stage 8 (Dynamic Focus Tracking):** Implementation of an intelligent targeting system using `glm::lookAt` where pressing keys 0 to 4 recalculates the camera's focus point to automatically track each moving planet.
***Stage 9 (Texture Mapping & Calibration):** Replacement of flat colors with real image textures (.jpg/.png) projected onto the 3D spheres using UV coordinate mapping.
***Stage 10 (Interactive Panel & Control CLI):** Integration of a primitive interface using ImGui to enable real-time terminal interaction, allowing the user to dynamically modify the size and speed of the planets.


## 3. Encountered Difficulties and Technical Solutions

1. **Ecosystem Integration in CMakeLists.txt:** Integrating Dear ImGui alongside SFML and OpenGL in the same build system was highly problematic and caused numerous linking and header resolution errors (with GLM and other libraries constantly throwing critical warnings). The solution required debugging the CMake scripts to manage the external dependencies and ensure proper static linking.

2. **Multiplatform and Remote Repository Fixes:** When pushing and committing changes to the remote repository, compilation errors arose regarding Linux and macOS compatibility. The root `CMakeLists.txt` was modified with minor updates to include native system libraries, ensuring the code builds smoothly across platforms.

3. **Window Resizing and Centering Objects:** A major challenge was maintaining the system centered and properly proportioned when resizing the graphics window. Initially, modifying the window dimensions distorted the spheres into oval shapes or pushed them off-center. With assistance, I solved this by dynamically recalculating the aspect ratio inside the resize event and updating the `glViewport` to keep all objects perfectly centered.

4. **Planet Collision in Stage 09:** While developing the texture mapping phase, I noticed that some planets visually collided and overlapped with each other because the provisional orbital distances from previous stages were too short. I resolved this issue by recalculating and increasing the initial position spacing on the X-axis for each planet.

5. **Size Overlapping in Stage 10:** In Stage 10, when expanding the planet size limits using the new ImGui sliders, massive scales caused the celestial bodies to overlap and fuse again. I successfully solved this problem by implementing an `if-else` statement in the update loop that dynamically pushes the planets outward onto safer orbital ¡paths whenever their size surpasses a certain limit.

6. **Syntax and Asset Loading Issues:** The project presented countless standard syntax errors during development and initial bugs when trying to fetch the correct paths for the texture files, which were gradually fixed by restructuring the code.

## 4. Use of External Resources and AI Declaration

In compliance with academic integrity guidelines, the following sources are declared:

***Academic Materials and University Notes:** The core logic of the OpenGL pipeline, time management via `sf::Clock` (`deltaTime`), and the trigonometric math for procedural sphere generation were developed based on the theoretical fundamentals and documentation provided throughout the academic course. The software architecture is designed to implement these standard computer graphics conceptual principles.
* **Use of Artificial Intelligence Assistance:** AI was utilized strictly as an engineering support tool for:
  1. Designing, organizing, and debugging the multi-library `CMakeLists.txt` script headers (SFML, OpenGL, ImGui, GLM).
  2. Finding and resolving common syntax errors and compiler warnings.
  3. Formulating the proper spacing offsets to eliminate the planet collision issue encountered in Stage 9 and centering objects inside the viewport layout.
* **Third-Party Open-Source Software:** The architecture relies on standard, legitimate libraries: SFML for windows/input, GLAD for function loading, GLM for linear algebra transformations, and Dear ImGui structures.
***Graphic Textures:** All planetary maps used for diffuse rendering are public domain assets sourced from open simulations online.