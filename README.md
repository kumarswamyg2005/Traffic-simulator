# 3D Traffic Simulator

A realistic 3D traffic simulation system built with OpenGL and GLUT, featuring dynamic traffic management, pedestrian AI, weather effects, and day/night cycles.

![Traffic Simulator](https://img.shields.io/badge/OpenGL-3D%20Graphics-green)
![Language](https://img.shields.io/badge/C++-11-blue)
![Platform](https://img.shields.io/badge/Platform-Cross--Platform-orange)

## Features

### 🚗 Traffic System

- **Smart Traffic Lights** - Adaptive timing based on traffic flow
- **Multiple Vehicle Types** - Cars, trucks, and buses with realistic physics
- **Collision Detection** - Realistic crash physics with vehicle damage
- **Dynamic Traffic Density** - Adjustable from minimal to heavy traffic
- **Exhaust Effects** - Particle system for vehicle emissions

### 🚶 Pedestrian System

- **Crosswalk Pedestrians** - Wait for signals and cross safely
- **Roaming Pedestrians** - Walk along sidewalks throughout the city
- **Unique Appearances** - Randomized clothing, accessories, and body types
- **Realistic Animation** - Walking cycles with leg and arm movement
- **Smart Pathfinding** - Navigate intersections and avoid obstacles

### 🌤️ Weather & Environment

- **Day/Night Cycle** - Dynamic lighting and sky colors
- **Weather Effects** - Rain and snow with particle systems
- **Realistic Lighting** - Time-based ambient and directional lighting
- **Starry Nights** - Visible stars during nighttime
- **Dynamic Shadows** - Lighting-based shadow simulation

### 🎥 Camera System

- **Free Camera** - Fly through the city with WASD controls
- **Follow Car** - Track vehicles from behind
- **Follow Pedestrian** - Close-up pedestrian perspective
- **Building View** - Bird's eye view of the entire city

### 🏙️ City Environment

- **Procedural Buildings** - Varied heights and textures
- **Special Landmarks** - Hospital, mall, and mart buildings
- **Park Areas** - Multiple themed parks with attractions
- **Textured Roads** - Realistic road, sidewalk, and grass textures
- **Street Infrastructure** - Traffic lights, benches, and signs

## Controls

### Camera Modes

| Key       | Action                                   |
| --------- | ---------------------------------------- |
| `1`       | Free camera mode                         |
| `2`       | Follow car camera                        |
| `3`       | Follow pedestrian camera                 |
| `4`       | Building view (bird's eye)               |
| `N` / `M` | Next / Previous target (in follow modes) |

### Free Camera Movement

| Key          | Action                             |
| ------------ | ---------------------------------- |
| `W` / `S`    | Move forward / backward            |
| `A` / `D`    | Move left / right                  |
| `E` / `C`    | Move up / down                     |
| `Arrow Keys` | Look around (pitch/yaw)            |
| `+` / `-`    | Increase / decrease movement speed |

### Environment Controls

| Key | Action                 |
| --- | ---------------------- |
| `T` | Toggle day/night cycle |
| `L` | Clear weather          |
| `P` | Rain                   |
| `O` | Snow                   |

### Traffic Density

| Key       | Action                                |
| --------- | ------------------------------------- |
| `0`       | Minimal traffic (density: 1)          |
| `7`       | Low traffic (density: 5)              |
| `8`       | Medium traffic (density: 10, default) |
| `9`       | High traffic (density: 20)            |
| `5` / `6` | Decrease / Increase density           |

### General Controls

| Key         | Action                      |
| ----------- | --------------------------- |
| `SPACE`     | Toggle auto-rotation        |
| `R`         | Reset simulation and camera |
| `Q` / `ESC` | Quit application            |

## Installation

### Prerequisites

**macOS:**

```bash
# OpenGL and GLUT are pre-installed
# Just need Xcode Command Line Tools
xcode-select --install
```

**Linux (Ubuntu/Debian):**

```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install freeglut3-dev
sudo apt-get install libglu1-mesa-dev
```

**Windows:**

- Install MinGW or Visual Studio
- Download FreeGLUT binaries
- Configure OpenGL libraries

### Build

```bash
# Clone the repository
git clone https://github.com/kumarswamyg2005/Traffic-simulator.git
cd Traffic-simulator

# Compile
make

# Run
./traffic_simulator
```

### Clean Build

```bash
make clean
make
```

## Architecture

### Core Components

```
┌─────────────────────────────────────────┐
│         Main Application Loop           │
│  (GLUT Timer Callback - 60 FPS)        │
└─────────────────────────────────────────┘
                    │
        ┌───────────┴───────────┐
        ▼                       ▼
┌──────────────┐        ┌──────────────┐
│   Update     │        │   Render     │
│   Systems    │        │   Systems    │
└──────────────┘        └──────────────┘
        │                       │
        ├─── Traffic Lights     ├─── Sky & Ground
        ├─── Vehicles           ├─── Buildings
        ├─── Pedestrians        ├─── Vehicles
        ├─── Collisions         ├─── Pedestrians
        ├─── Weather            ├─── Particles
        ├─── Day/Night          ├─── UI/HUD
        └─── Camera             └─── Effects
```

### Key Technologies

- **Graphics:** OpenGL (fixed-function pipeline)
- **Windowing:** GLUT/FreeGLUT
- **Language:** C++11
- **Build System:** Make
- **Textures:** Procedurally generated
- **Physics:** Custom collision detection and response

## Technical Highlights

### Smart Traffic Management

- Adaptive traffic light timing based on vehicle queue lengths
- Traffic flow optimization at each intersection
- Real-time throughput and wait time monitoring

### Realistic Physics

- Vehicle crash detection with impact forces
- Post-crash physics (sliding, rotation, settling)
- Gravity-based particle systems
- Smooth vehicle acceleration and braking

### Performance Optimization

- **LOD System:** Level-of-detail rendering for buildings
- **Particle Limits:** Maximum particle counts to maintain framerate
- **Distance Culling:** Skip rendering distant effects
- **Efficient Texturing:** Procedural generation eliminates file I/O

### Procedural Generation

- All textures generated algorithmically at runtime
- Random pedestrian appearances (colors, accessories, proportions)
- Varied building heights and configurations
- Dynamic particle generation and recycling

## Project Structure

```
Traffic-simulator/
├── main.cpp              # Main simulation code (5890 lines)
├── LODSystem.h           # Level of Detail management
├── parks.h               # Park structure definitions
├── ResourceManager.h     # Resource loading utilities
├── ConfigSystem.h        # Configuration management
├── PerformanceMonitor.h  # Performance tracking
├── json.hpp              # JSON parsing library
├── tiny_gltf.h          # GLTF model loading
├── stb_image.h          # Image loading utility
├── stb_image_write.h    # Image writing utility
├── Makefile             # Build configuration
├── traffic_sim.cfg      # Simulation settings
├── WORKFLOW.md          # Detailed workflow documentation
└── README.md            # This file
```

## Documentation

- **[WORKFLOW.md](WORKFLOW.md)** - Comprehensive technical documentation covering:
  - System architecture and data structures
  - Program flow and main loop
  - Detailed explanation of all systems
  - Rendering pipeline
  - Performance optimizations
  - Future enhancement ideas

## Screenshots

The simulator features:

- Multiple intersections with synchronized traffic lights
- Diverse vehicle types navigating city streets
- Pedestrians crossing at crosswalks and walking on sidewalks
- Dynamic weather effects (rain/snow particles)
- Day and night lighting with realistic sky gradients
- Detailed city environment with parks and buildings

## Performance

- **Target FPS:** 60
- **Typical Performance:** 50-60 FPS on modern hardware
- **Vehicles:** Up to 100+ simultaneous vehicles
- **Pedestrians:** Up to 220 pedestrians
- **Particles:** Thousands of weather/exhaust particles

## Known Limitations

- Fixed-function OpenGL (legacy pipeline)
- No anti-aliasing
- Simple collision detection (bounding boxes)
- No persistent save/load functionality
- Single-threaded execution

## Future Enhancements

- [ ] Modern OpenGL (shaders, VBO/VAO)
- [ ] Multi-lane highways and interchanges
- [ ] Emergency vehicles (ambulance, police, fire truck)
- [ ] Public transportation (buses on routes, trams)
- [ ] Building interiors with windows
- [ ] Weather effects on vehicle speed
- [ ] Traffic accident reporting system
- [ ] Configurable city layouts
- [ ] Save/load simulation states
- [ ] Network multiplayer support

## Contributing

Contributions are welcome! Areas for improvement:

- Code refactoring and optimization
- Modern OpenGL migration
- Additional vehicle/pedestrian behaviors
- New weather effects
- Enhanced UI/HUD elements
- Bug fixes and performance improvements

## License

This project is open source. Feel free to use, modify, and distribute.

## Author

**kumarswamyg2005**

- GitHub: [@kumarswamyg2005](https://github.com/kumarswamyg2005)
- Repository: [Traffic-simulator](https://github.com/kumarswamyg2005/Traffic-simulator)

## Acknowledgments

- OpenGL and GLUT communities
- STB libraries for image handling
- JSON for Modern C++ library
- TinyGLTF for model loading

---

**Built with ❤️ using OpenGL and C++**

_Last Updated: November 15, 2025_
