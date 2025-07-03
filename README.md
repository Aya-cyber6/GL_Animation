# 3D Character Animation and Camera Management

## Overview
This project demonstrates 3D character animation and dynamic camera control using OpenGL. It features an animated skinned mesh character combined with static environment models, allowing users to view the scene from different camera modes: first-person, free-fly, and cinematic.

## Features
- Load and render animated FBX character models with skeletal animation.
- Render static OBJ environment models.
- Three camera modes:
  - **First-Person:** Camera follows character perspective.
  - **Free-Fly:** User-controlled camera around the scene.
  - **Cinematic:** Smooth, automatic camera following the character.
- Keyboard controls to switch camera modes and animations.
- Play/pause animations.
- Basic Phong lighting model with directional light.

## Requirements
- OpenGL development environment
- GLFW library for window/context and input management
- GLAD for OpenGL function loading
- Assimp library for loading 3D models and animations
- C++ compiler supporting C++11 or later

## File Structure
- `main.cpp`: Contains the main application and event callbacks.
- `Engine` class: Manages window creation, input handling, rendering, animation, and camera updates.
- Shader and rendering utilities (Phong shading, lighting) included in linked headers.
- 3D models located in the `Content` folder:
  - Animated character: `Content/cartoon/source/stt.fbx`
  - Static environment: `Content/Station/stationgarden.obj`

## Controls
### Camera Modes:
- Press `C` for First-Person Camera  
- Press `V` for Free-Fly Camera (default)  
- Press `B` for Cinematic Camera  

### Animation Selection:
- Keys **0** to **3** - Switch between different animations

### Animation Control:
- **Space** - Play/Pause animation

### Exit:
- **ESC** or **Q** - Quit the application

### Mouse:
- Left-click + drag to control camera orientation in free-fly mode

## How to Build and Run
1. Ensure all dependencies (GLFW, GLAD, Assimp) are installed and linked properly.
2. Compile the source files with your C++ compiler.
3. Run the executable. The window will open with the animated scene.
4. Use keyboard and mouse controls to interact with the animation and camera.

## Notes
- The project initializes with a free-fly camera positioned behind the character.
- Animation timing and pause/play functionality are managed internally with precise time calculations.
- Proper error handling and resource management (e.g., deleting allocated objects) are implemented.

## Rapor
https://docs.google.com/document/d/1Kb8SRBrA7iHRzKZELqXtprR3cTYU8gZ8KM0o3kowPiE/edit?usp=sharing
(only accessible through iüc school email)

## Screenshot & Video
![image](https://github.com/user-attachments/assets/a968056e-11f4-42c8-a83c-d657664e6e23)  
📹 [Watch the demo](https://youtu.be/409cszuZyEI)

## Credits
This project is based on [ogldev](http://ogldev.atspace.co.uk/) tutorials.
