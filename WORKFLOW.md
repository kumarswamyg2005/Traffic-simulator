# 3D Traffic Simulator - Code Workflow Documentation

## Overview

This is a comprehensive 3D traffic simulation system built using OpenGL and GLUT. The simulator creates a realistic urban environment with traffic lights, vehicles, pedestrians, weather effects, and dynamic day/night cycles.

---

## System Architecture

### Core Components

1. **Graphics Engine (OpenGL/GLUT)**

   - Uses fixed-function OpenGL pipeline
   - GLUT for window management and input handling
   - Double-buffered rendering with depth testing

2. **Simulation Systems**

   - Traffic management system with smart traffic lights
   - Vehicle physics and collision detection
   - Pedestrian behavior and pathfinding
   - Weather particle systems
   - Day/night cycle with dynamic lighting

3. **Rendering Systems**
   - Procedural texture generation
   - Level of Detail (LOD) system
   - Multiple camera modes
   - Particle effects (exhaust, rain, snow)

---

## Data Structures

### Main Entity Types

```cpp
struct Vehicle {
    - Type: CAR, TRUCK, BUS
    - Position: (x, y, z) coordinates
    - Direction vectors and rotation
    - Physical dimensions (width, height, depth)
    - Speed and wheel animation
    - Crash physics (rollAngle, pitchAngle, velocities)
    - Traffic state (isWaiting, waitingAtIntersection)
}

struct Pedestrian {
    - Position and movement direction
    - Visual appearance (colors, accessories)
    - Behavior state (roaming, crossing, waiting)
    - Animation data (walkPhase, legAngle)
    - Target destination and pathfinding
}

struct Intersection {
    - Position (x, z)
    - Traffic light states (horizontal, vertical)
    - Timing system with adaptive green lights
    - Performance metrics (throughput, wait times)
}

struct Particle {
    - Position (x, y, z) and velocity (vx, vy, vz)
    - Lifetime tracking
    - Color and transparency (r, g, b, a)
    - Used for weather and exhaust effects
}
```

---

## Program Flow

### 1. Initialization (`main()`)

```
1. Initialize GLUT context
2. Create window (1400x900)
3. Set up display callbacks
4. Call init() to configure OpenGL
5. Print control instructions
6. Enter GLUT main loop
```

### 2. OpenGL Setup (`init()`)

```
1. Seed random number generator
2. Enable depth testing and lighting
3. Configure lights (LIGHT0, LIGHT1)
4. Set global ambient lighting
5. Enable color material for realistic colors
6. Generate all procedural textures:
   - Road texture
   - Building textures (hospital, mall, mart)
   - Sidewalk and grass textures
   - Wood and metal textures
7. Initialize intersections
8. Spawn initial pedestrians
```

### 3. Main Loop (Timer-Driven Updates)

The simulation runs at approximately 60 FPS through a timer callback:

```
timer() callback (every ~16ms):
├── Update simulation systems
│   ├── updateTrafficLights()
│   ├── updateVehicles()
│   ├── updateCrashedVehicles()
│   ├── updateCrashEffects()
│   ├── updatePedestrians()
│   ├── updateWeatherParticles()
│   └── updateExhaustParticles()
│
├── Spawn new entities
│   ├── Spawn vehicles based on traffic density
│   ├── Spawn roaming pedestrians
│   └── Spawn crosswalk pedestrians
│
├── Update day/night cycle
│
├── Update camera position
│   ├── Free camera mode
│   ├── Follow car mode
│   ├── Follow pedestrian mode
│   └── Building view mode
│
├── Request display refresh
└── Schedule next timer call
```

---

## Key Systems Explained

### Traffic Light System

**Adaptive Traffic Control:**

- Each intersection has independent traffic lights
- Base timing: 120 frames green, 30 frames yellow, then switch
- Adaptive extension: Up to 60 extra green frames if traffic is heavy
- Monitors throughput and wait times to optimize flow

**Update Process:**

```
For each intersection:
1. Increment timer
2. Check if timer exceeds duration
3. If changing from green:
   - Calculate recent traffic metrics
   - Decide if extra green time needed
   - Transition through yellow state
4. Switch lights (horizontal ↔ vertical)
5. Reset timer
```

### Vehicle Movement System

**Movement Logic:**

```
For each vehicle:
1. Check if crashed → apply crash physics
2. Check for collisions with other vehicles
3. Check traffic light at next intersection
4. If red light → slow down and stop
5. If green or far from intersection:
   - Calculate turn probability
   - Smoothly interpolate direction if turning
   - Update position based on speed and direction
6. Handle lane changes
7. Animate wheels based on speed
8. Generate exhaust particles
```

**Collision Detection:**

- Simple distance-based checks between vehicles
- If distance < threshold → trigger crash
- Apply realistic crash physics (rotation, sliding)
- Vehicles settle after crash and become obstacles

### Pedestrian System

**Two Pedestrian Types:**

1. **Crosswalk Pedestrians:**

   - Wait at intersections for perpendicular green light
   - Cross along designated crosswalks
   - Respect traffic signals

2. **Roaming Pedestrians:**
   - Walk along sidewalks
   - Navigate city streets randomly
   - Can turn at intersections
   - Pause occasionally for realism

**Movement Algorithm:**

```
For each pedestrian:
1. If roaming:
   - Move toward target destination
   - Check if reached target → pick new destination
   - Handle turns at intersections
   - Avoid going out of bounds
2. If at crosswalk:
   - Wait for safe crossing light
   - Cross when perpendicular light is red
   - Respawn at random intersection when done
3. Update walk animation (leg swing, arm swing)
4. Apply accessories (backpack, hat) for variety
```

### Weather System

**Particle-Based Weather:**

**Rain:**

- Particles fall vertically with slight wind drift
- Fast-moving, thin appearance
- Bluish-gray color with transparency

**Snow:**

- Slower falling speed
- Gentle swaying motion
- White particles with soft glow
- Accumulation effect (visual only)

**Update Process:**

```
For each particle:
1. Update position based on velocity
2. Apply gravity and wind
3. Decrease lifetime
4. If particle reaches ground or expires → respawn at top
5. Render with alpha blending
```

### Day/Night Cycle

**Time System:**

- `timeOfDay` ranges from 0.0 (midnight) to 1.0 (next midnight)
- 0.25 = sunrise, 0.5 = noon, 0.75 = sunset
- Can toggle automatic cycling or manual switch

**Lighting Adjustments:**

```
Based on timeOfDay:
1. Calculate sun position in sky
2. Adjust ambient light intensity
3. Change directional light color:
   - Dawn/Dusk: Orange/reddish tones
   - Noon: Bright white light
   - Night: Reduced intensity, bluer tones
4. Update sky colors (gradient from horizon to zenith)
5. Show stars at night
6. Adjust building window lights (on at night)
```

### Camera System

**Four Camera Modes:**

1. **Free Camera (Mode 1):**

   - WASD movement in 3D space
   - Arrow keys for pitch/yaw control
   - E/C for vertical movement
   - +/- to adjust movement speed

2. **Follow Car (Mode 2):**

   - Camera tracks selected vehicle
   - Positioned behind and above vehicle
   - Smooth following with lag
   - N/M to switch between vehicles

3. **Follow Pedestrian (Mode 3):**

   - Similar to follow car but for pedestrians
   - Closer camera distance for detail
   - N/M to cycle through pedestrians

4. **Building View (Mode 4):**
   - Fixed bird's eye view
   - Shows entire city layout
   - Good for observing traffic patterns

---

## Rendering Pipeline

### Display Function Flow

```
display():
├── Clear buffers (color + depth)
├── Setup perspective projection
├── Setup camera view (based on mode)
├── Configure lighting (setupLighting)
│   ├── Set ambient light
│   ├── Set directional light (sun)
│   └── Adjust for time of day
│
├── Render scene elements:
│   ├── drawSky() - Gradient sky with stars
│   ├── drawGround() - Textured ground plane
│   ├── drawBuildings() - City buildings with LOD
│   ├── drawAllParks() - Park structures
│   ├── drawIntersection() - Roads and crosswalks
│   ├── drawTrafficLight() - All traffic signals
│   │
│   ├── Render all vehicles:
│   │   ├── Set vehicle color
│   │   ├── Apply crash rotation if crashed
│   │   ├── Draw vehicle body (box)
│   │   ├── Draw windows
│   │   ├── Draw wheels with rotation
│   │   └── Draw headlights/taillights
│   │
│   └── Render all pedestrians:
│       ├── Draw body parts with animation
│       ├── Apply walking motion
│       └── Draw accessories (backpack, hat)
│
├── Render particle effects:
│   ├── Weather particles (rain/snow)
│   └── Exhaust particles from vehicles
│
├── Draw UI elements (HUD)
│   ├── FPS counter
│   ├── Vehicle count
│   ├── Time of day
│   └── Weather status
│
└── Swap buffers (double buffering)
```

---

## User Interaction

### Keyboard Controls

**Camera Modes:**

- `1` - Free camera
- `2` - Follow car
- `3` - Follow pedestrian
- `4` - Building view
- `N/M` - Next/Previous target (in follow modes)

**Movement (Free Camera):**

- `W/A/S/D` - Forward/Left/Back/Right
- `Arrow Keys` - Look around
- `E/C` - Up/Down
- `+/-` - Adjust speed

**Environment:**

- `T` - Toggle day/night
- `L` - Clear weather
- `P` - Rain
- `O` - Snow

**Traffic:**

- `0` - Minimal traffic (density 1)
- `7` - Low traffic (density 5)
- `8` - Medium traffic (density 10)
- `9` - High traffic (density 20)
- `5/6` - Decrease/Increase density

**Simulation:**

- `SPACE` - Toggle auto-rotation
- `R` - Reset simulation
- `Q/ESC` - Quit

---

## Performance Optimization

### LOD (Level of Detail) System

- Buildings use simplified geometry when far from camera
- Reduces polygon count for distant objects
- Three LOD levels: High, Medium, Low
- Distance-based switching

### Texture Management

- Procedurally generated textures (no file I/O)
- Reusable texture atlas approach
- Efficient texture binding and state changes

### Particle Limits

- Maximum weather particles to prevent slowdown
- Particle recycling instead of constant creation/deletion
- Distance-based culling for exhaust particles

---

## Technical Features

1. **Procedural Generation:**

   - All textures generated algorithmically
   - Random pedestrian appearances
   - Varied building heights and colors

2. **Physics Simulation:**

   - Basic crash physics with rotation and sliding
   - Gravity for particles
   - Realistic vehicle acceleration/deceleration

3. **Smart Traffic Management:**

   - Adaptive traffic light timing
   - Queue detection
   - Throughput optimization

4. **Realistic Effects:**
   - Dynamic shadows (via lighting)
   - Specular highlights on vehicles
   - Alpha blending for transparency
   - Exhaust particle trails

---

## Build & Run

### Compilation

```bash
make
```

### Execution

```bash
./traffic_simulator
```

### Requirements

- OpenGL
- GLUT/FreeGLUT
- C++ compiler with C++11 support
- macOS, Linux, or Windows (with proper GL libraries)

---

## File Structure

```
main.cpp              - Main simulation code (5890 lines)
LODSystem.h           - Level of Detail management
parks.h               - Park structure definitions
ResourceManager.h     - Resource loading utilities
ConfigSystem.h        - Configuration management
PerformanceMonitor.h  - Performance tracking
json.hpp              - JSON parsing library
tiny_gltf.h           - GLTF model loading
stb_image.h           - Image loading
stb_image_write.h     - Image writing
Makefile              - Build configuration
traffic_sim.cfg       - Simulation settings
```

---

## Future Enhancements

Potential areas for expansion:

- Multi-lane highways
- Traffic accidents with emergency vehicles
- More complex pedestrian AI
- Building interiors
- Public transportation (buses, trams)
- Weather effects on traffic (slower speeds in rain)
- Save/load simulation states
- Network multiplayer support

---

## Credits

**Repository:** https://github.com/kumarswamyg2005/Traffic-simulator  
**Author:** kumarswamyg2005  
**Technology Stack:** OpenGL, GLUT, C++

---

_Last Updated: November 15, 2025_
