# 3D Traffic Simulator - OpenGL Project

A stunning, realistic 3D traffic simulation built with OpenGL and GLUT, featuring intelligent traffic light control, multiple vehicle types, full 3D graphics with lighting and fog effects, and a detailed urban environment.

## New in this build (Nov 2025)

- Procedural textures for roads, sidewalks, grass, car bodies, and multiple building facades.
- New special buildings: Hospitals (with red cross signage), Marts (striped fascia), and Malls (glassy facades), placed cleanly in city blocks away from roads.
- More lively parks and plazas; additional benches, lamps, planters, and animated features.
- Vehicles now use textured bodies; motorcycles and bicycles include a simple human rider model.
- Better alignment of buildings with the road grid; dimensions tuned to the 40u road + 8u sidewalks.

## Features

### 🚗 Advanced 3D Vehicle System

- **Three vehicle types** - Cars, Trucks, and Buses with distinct 3D models
- **Realistic 3D modeling** - Detailed vehicles with roofs, windows, wheels, headlights, and taillights
- **Working headlights** - Glowing lights with emission effects
- **Intelligent collision avoidance** - vehicles maintain safe distances based on vehicle size
- **Lane discipline** - vehicles stay in their designated lanes
- **Realistic physics** - smooth movement and variable speeds
- **Auto-spawning** - new vehicles appear randomly from all directions
- **4-way traffic** - vehicles moving in all cardinal directions

### 🚦 3D Traffic Light System

- **Four 3D traffic lights** at intersection corners
- **Red-Yellow-Green cycle** with realistic timing
- **Glowing light bulbs** with emission effects for realism
- **Smart vehicle stopping** - vehicles obey traffic signals
- **Proper signal phasing** - horizontal and vertical lights alternate
- **Directional orientation** - lights face the correct traffic flow

### 🏙️ Detailed 3D Urban Environment

- **12 3D buildings** with realistic architecture, roofs, and lit windows
- **10 trees** with 3D trunks and spherical foliage
- **4 street lights** with poles and glowing bulbs
- **Textured roads** with proper asphalt coloring
- **Detailed road markings** - yellow center lines, white lane dividers
- **Zebra crosswalks** at all four intersection approaches
- **Sidewalks** elevated along all roads
- **Grass areas** surrounding the road network

### 🎨 Advanced Visual Effects

- **Full 3D perspective** with dynamic camera system
- **Professional lighting** - Dual light sources (sun + fill light)
- **Atmospheric fog** for depth perception
- **Smooth shading** on all 3D objects
- **Emission lighting** on headlights and traffic signals
- **Auto-rotating camera** for cinematic view
- **60 FPS smooth animation**
- **Sky blue background** for realism

### 🎮 Interactive Camera Controls

- **Arrow Keys** - Rotate camera view / Adjust camera height
- **+ / -** - Zoom in and out
- **SPACE** - Toggle automatic camera rotation
- **R** - Reset simulation
- **Q / ESC** - Quit application

## Controls

### Camera Controls

- **↑ Up Arrow** - Increase camera height
- **↓ Down Arrow** - Decrease camera height
- **← Left Arrow** - Rotate camera left
- **→ Right Arrow** - Rotate camera right
- **+** or **=** - Zoom in (decrease distance)
- **-** or **\_** - Zoom out (increase distance)
- **SPACE** - Toggle auto-rotation on/off

### Simulation Controls

- **R** - Reset simulation (clear all vehicles, reset lights and camera)
- **Q** or **ESC** - Quit the application

## Building and Running

### Prerequisites

- macOS with OpenGL and GLUT frameworks (pre-installed on most Mac systems)
- g++ compiler (install via Xcode Command Line Tools)

### Installation

1. **Install Xcode Command Line Tools** (if not already installed):

```bash
xcode-select --install
```

2. **Navigate to project directory**:

```bash
cd "/Users/kumaraswamy/Desktop/untitled folder 2"
```

3. **Build the project**:

```bash
make
```

4. **Run the simulation**:

```bash
make run
```

Or simply:

```bash
./traffic_simulator
```

### Alternative Build (without Makefile)

```bash
g++ -std=c++11 -o traffic_simulator main.cpp -framework OpenGL -framework GLUT
./traffic_simulator
```

## How It Works

### 3D Rendering System

- **Perspective projection** with 45° field of view
- **Dynamic camera** positioned in 3D space orbiting the intersection
- **Dual lighting** - Directional sun light + ambient fill light
- **Fog effects** for atmospheric depth (linear fog from 100-300 units)
- **Smooth shading** using OpenGL Gouraud shading
- **Depth testing** for proper 3D object rendering

### Traffic Light Logic

- **Green Phase**: 180 frames (~3 seconds)
- **Yellow Phase**: 60 frames (~1 second)
- **Red Phase**: While other direction is green/yellow
- Lights alternate between horizontal and vertical traffic flow
- Four lights positioned at intersection corners, facing appropriate directions

### Vehicle Behavior

1. **Three vehicle types spawn** with different probabilities:
   - 70% Cars (compact, 4x3x8 units)
   - 15% Trucks (larger, 5x5x12 units with cargo area)
   - 15% Buses (longest, 5x6x15 units with multiple windows)
2. They move at variable speeds (0.6 - 1.1 units/frame)
3. They check for:
   - Red lights ahead (stopping ~15-35 units before intersection)
   - Other vehicles in the same lane (maintaining safe distance based on vehicle length)
4. They stop automatically when needed
5. They despawn after traveling 220 units

### Collision Detection

- **Dynamic safe distance** based on vehicle size (vehicle.depth + other.depth + 3)
- Vehicles stop smoothly when approaching obstacles
- Each lane is independently monitored
- Larger vehicles (buses, trucks) require more stopping distance

### 3D Models

- **Vehicles**: Custom 3D models with chassis, roof, windows, wheels (using torus), headlights, taillights
- **Buildings**: Cuboid structures with window arrays and roof overhangs
- **Trees**: Trunk cubes + spherical foliage clusters
- **Traffic Lights**: Pole + housing box + three spherical light bulbs with emission
- **Roads**: Quad surfaces with line markings

## Code Structure

- **3D Geometry Functions**: `drawCube()` for all box-shaped objects
- **Vehicle Struct**: Stores 3D position (x,y,z), type, size, color, direction
- **Traffic Light State Machine**: Manages light transitions with proper timing
- **Lighting Setup**: `setupLighting()` configures dual light sources
- **Environment Drawing**: Separate functions for ground, roads, buildings, trees, lights
- **Camera System**: Dynamic viewing with rotation, zoom, and height controls
- **Update Functions**: Handle physics, AI, and state transitions
- **Timer System**: Runs at ~60 FPS for smooth animation

## Customization

You can easily modify:

- `LIGHT_DURATION` - Time for green/red lights (default: 180 frames)
- `YELLOW_DURATION` - Time for yellow lights (default: 60 frames)
- `cameraHeight` - Initial camera height (default: 80 units)
- `cameraDistance` - Initial zoom level (default: 200 units)
- Vehicle spawn rate in `spawnVehicles()` (default: 3.75% per frame)
- Vehicle type probabilities (currently 70/15/15 for car/truck/bus)
- Building positions, sizes, and colors in `drawBuildings()`
- Lighting colors and positions in `setupLighting()`
- Fog density and range in `init()`

## Technical Details

- **Resolution**: 1400x900 pixels (adjustable)
- **Frame Rate**: ~60 FPS (16ms per frame)
- **Graphics**: OpenGL 2.1 with GLU and GLUT
- **3D Features**: Perspective projection, depth buffering, lighting, fog, smooth shading
- **Language**: C++11
- **Platform**: macOS (easily portable to Linux/Windows with minor header changes)
- **Rendering**: Double-buffered for flicker-free animation
- **Primitives**: Quads, spheres, tori (toruses) for wheels

## What Makes This Realistic

1. **True 3D perspective** - Objects appear smaller in the distance
2. **Professional lighting** - Multiple light sources with ambient, diffuse, and specular components
3. **Atmospheric effects** - Fog adds depth perception
4. **Detailed 3D models** - Vehicles have distinct shapes, roofs, windows, and wheels
5. **Glowing lights** - Emission materials on headlights and traffic signals
6. **Varied vehicle types** - Different sizes and purposes (cars, trucks, buses)
7. **Realistic architecture** - Buildings with windows, roofs, and proper proportions
8. **Dynamic camera** - Cinematic orbiting view of the entire scene
9. **Proper shadows and shading** - Smooth surface normals for realistic lighting
10. **Urban details** - Trees, street lights, sidewalks, crosswalks

## Future Enhancements

Possible improvements:

- Turning vehicles at the intersection
- Pedestrians with animated walking
- Day/night cycle with changing lighting
- Weather effects (rain, snow)
- Traffic density controls via keyboard
- Additional vehicle types (motorcycles, bicycles)
- More complex road networks with multiple intersections
- Textured surfaces using image files
- Particle effects for exhaust smoke
- Sound effects for ambient traffic noise

## License

Free to use and modify for educational purposes.

---

**Enjoy the 3D simulation!** Use the arrow keys and camera controls to explore the intersection from different angles. Watch as cars, trucks, and buses intelligently navigate the traffic system in stunning 3D with realistic lighting and atmospheric effects!
