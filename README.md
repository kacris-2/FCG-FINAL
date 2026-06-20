
# 3D Planetary System - Compilation and User Manual

This file provides the necessary instructions to configure, build, run, and interact with the 10 sequential development stages of the project.


## 1. How to Build All Stages Together

To build the entire project, you must open your terminal, navigate to the root directory of the project (for example, if your folder is named `fcg-final`, your path should look like `C:\fcg-final`), and execute the following commands:

```bash
# Configure the project and generate the build directory
cmake -S . -B build

# Compile all executable stages concurrently
cmake --build build

#Comandlines to launch the executables

For example: 
# To run Stage 9 (Texture mapping and fixed orbits):
.\build\bin\Debug\Tappa09.exe

# To run Stage 10 (Final version with interactive CLI terminal console):
.\build\bin\Debug\Tappa10.exe

On Linux/ macOS 
# Relative execution paths for UNIX environments:
./build/bin/Tappa09
./build/bin/Tappa10


#Schematic List of User Interface Controls

Stage 6 to stage 10: Interactive Camera Controls (Keyboard)

- W/S Keys: Controls the camera distance (Zoom In to get closer / Zoom out to move away from the active target)

- Arrow Keys( Left, Right, Up, Down): Modifies the camera's angula perspective (Yaw and Pitch) to rotate the view smoothly around the focused object

Stage 8 to 10: Dynamic Camera Target Selection (Keyboard)

Allows the user to instantly change the camera's focus point to track different planets in real time:

- 0 Lochs the camera focus on the central Sun
- 1 Tracks and follows Mustafar
- 2 Tracks and follows Tatooine
- 3 Tracks and follows Hoth
- 4 Tracks and follows Endor

Stage 10: Runtime Simulation Scaling & Speed Adjustments

Stage 10 expands the system functionality by unlocking custom controls over the simulation parameters:

Interactive Control Interface: The user can manipulate the graphical interface tools to dynamically change the scale (size) and the orbital rotation velocity (speed) of each individual planet in real time.
