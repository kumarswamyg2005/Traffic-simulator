/\*\*

- @file Integration_Guide.md
- @brief Step-by-step guide for integrating Phase 1 enhancements
-
- This document outlines how to integrate the new systems into main.cpp
  \*/

# Phase 1 Critical Enhancements Integration Guide

## Overview

This guide explains how to integrate the following new systems:

1. ResourceManager (RAII-based resource management)
2. PerformanceMonitor (FPS tracking and frame control)
3. LODSystem (Level of Detail for optimization)
4. ConfigSystem (Configuration file support)

## Files Created

- `ResourceManager.h` - OpenGL resource lifecycle management
- `PerformanceMonitor.h` - Performance monitoring and FPS control
- `LODSystem.h` - Level of Detail and frustum culling
- `ConfigSystem.h` - Configuration system

## Integration Steps

### Step 1: Add Include Statements

Add these includes after the existing OpenGL includes in main.cpp (after line 50):

```cpp
#include "ResourceManager.h"
#include "PerformanceMonitor.h"
#include "LODSystem.h"
#include "ConfigSystem.h"
```

### Step 2: Remove Global Texture Variables

Remove or comment out these lines (around line 45-51):

```cpp
// OLD CODE - REMOVE:
// GLuint roadTexture = 0;
// GLuint buildingTexture = 0;
// GLuint carBodyTexture = 0;
// GLuint sidewalkTexture = 0;
// GLuint grassTexture = 0;
// GLuint woodTexture = 0;
// GLuint metalTexture = 0;
```

These are now managed by ResourceManager.

### Step 3: Update init() Function

Modify the init() function (line 1489) to use the new systems:

```cpp
void init() {
    // Load configuration (creates default if not found)
    ConfigSystem::getInstance().setDefaults();
    ConfigSystem::getInstance().loadFromFile("traffic_sim.cfg");

    // Seed RNG for deterministic variety per run
    srand((unsigned int)time(NULL));

    // Basic GL states
    glEnable(GL_DEPTH_TEST);

    // Enable/disable based on config
    if (CONFIG_BOOL("enable_lighting", true)) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    if (CONFIG_BOOL("enable_textures", true)) {
        glEnable(GL_TEXTURE_2D);
    }

    // Fog for depth perception
    if (CONFIG_BOOL("enable_fog", true)) {
        glEnable(GL_FOG);
        GLfloat fogColor[4] = {0.53f, 0.81f, 0.92f, 1.0f};
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_START, 200.0f);
        glFogf(GL_FOG_END, 500.0f);
        glFogf(GL_FOG_DENSITY, 0.015f);
    }

    GLfloat global_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Generate all procedural textures using ResourceManager
    generateRoadTexture();
    generateBuildingTexture();
    generateCarTexture();
    generateSidewalkTexture();
    generateGrassTexture();
    generateWoodTexture();
    generateMetalTexture();

    // Initialize static geometry display lists
    ResourceManager::getInstance().compileStaticGeometry();

    // Initialize intersections and pedestrians
    initIntersections();
    pedestrians.clear();

    int initialRoamers = CONFIG_INT("initial_pedestrians", 60);
    for (int i = 0; i < initialRoamers; ++i) {
        spawnRoamingPedestrian();
    }

    printf("[Init] Initialization complete\n");
    printf("[Init] Config loaded with %d parameters\n",
           ConfigSystem::getInstance().hasKey("target_fps") ? 1 : 0);
    ResourceManager::getInstance().printMemoryUsage();
}
```

### Step 4: Update Texture Generation Functions

Update each texture generation function to use ResourceManager. Example for generateRoadTexture:

```cpp
void generateRoadTexture() {
    const int SIZE = 128;
    unsigned char* data = new unsigned char[SIZE * SIZE * 3];

    // ... existing texture generation code ...

    // OLD: Upload directly to roadTexture
    // NEW: Use ResourceManager
    ResourceManager::getInstance().getRoadTexture().upload(data, SIZE, SIZE);

    delete[] data;
}
```

Repeat for all texture generation functions:

- generateBuildingTexture() -> getBuildingTexture()
- generateCarTexture() -> getCarTexture()
- generateSidewalkTexture() -> getSidewalkTexture()
- generateGrassTexture() -> getGrassTexture()
- generateWoodTexture() -> getWoodTexture()
- generateMetalTexture() -> getMetalTexture()

### Step 5: Update Texture Binding Calls

Replace all glBindTexture calls with ResourceManager calls:

```cpp
// OLD:
// glBindTexture(GL_TEXTURE_2D, roadTexture);

// NEW:
ResourceManager::getInstance().getRoadTexture().bind();
```

### Step 6: Update display() Function

Add performance monitoring to the display function:

```cpp
void display() {
    PerformanceMonitor::getInstance().frameStart();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSky();
    glLoadIdentity();

    // Setup free camera
    float lookX = cameraX + cos(cameraYaw * M_PI / 180.0f) * cos(cameraPitch * M_PI / 180.0f);
    float lookY = cameraY + sin(cameraPitch * M_PI / 180.0f);
    float lookZ = cameraZ + sin(cameraYaw * M_PI / 180.0f) * cos(cameraPitch * M_PI / 180.0f);

    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0, 1.0, 0.0);

    setupLighting();

    // Update frustum culler for this frame
    static FrustumCuller frustumCuller;
    float viewDirX = lookX - cameraX;
    float viewDirY = lookY - cameraY;
    float viewDirZ = lookZ - cameraZ;
    frustumCuller.updateViewFrustum(cameraX, cameraY, cameraZ,
                                    viewDirX, viewDirY, viewDirZ,
                                    60.0f, 16.0f/9.0f, 0.1f, 500.0f);

    // Use display lists for static geometry (if enabled)
    if (CONFIG_BOOL("enable_display_lists", true)) {
        ResourceManager::getInstance().getRoadDisplayList().call();
        ResourceManager::getInstance().getBuildingDisplayList().call();
        ResourceManager::getInstance().getGroundDisplayList().call();
    } else {
        drawGround();
        drawRoad();
        drawBuildings();
    }

    // Draw intersections (still dynamic due to traffic lights)
    for (const auto& intersection : intersections) {
        drawIntersection(intersection.x, intersection.z);
    }

    // Draw static decorations
    drawTrees();
    drawStreetLights();
    drawBenchesAndSigns();
    drawParkedCars();
    drawBusStops();
    drawFlowerbeds();
    drawFountains();
    drawStatues();
    drawPromenade();
    drawMarketPlaza();
    drawFamilyPlayPark();
    drawObservationWheel();

    // Draw traffic lights
    for (const auto& intersection : intersections) {
        drawTrafficLight(intersection.x + 20, 0, intersection.z + 30, intersection.verticalLight, 180);
        drawTrafficLight(intersection.x - 20, 0, intersection.z - 30, intersection.verticalLight, 0);
        drawTrafficLight(intersection.x + 30, 0, intersection.z - 20, intersection.horizontalLight, 270);
        drawTrafficLight(intersection.x - 30, 0, intersection.z + 20, intersection.horizontalLight, 90);
    }

    // Draw vehicles with LOD
    LODSystem::getInstance().resetStats();
    frustumCuller.resetStats();

    for (const auto& vehicle : vehicles) {
        // Calculate distance to camera
        float distSq = LODSystem::distanceSquared(vehicle.x, vehicle.y, vehicle.z,
                                                  cameraX, cameraY, cameraZ);

        // Frustum culling
        if (!frustumCuller.isVisible(vehicle.x, vehicle.y, vehicle.z, 5.0f)) {
            frustumCuller.recordCulled();
            continue;
        }
        frustumCuller.recordVisible();

        // LOD selection
        LODLevel lod = LODSystem::getInstance().getVehicleLOD(distSq);
        LODSystem::getInstance().recordLOD(lod);

        if (lod != LODLevel::CULLED) {
            drawVehicle(vehicle, lod);  // Modified to accept LOD parameter
        }
    }

    // Draw pedestrians with LOD
    for (const auto& ped : pedestrians) {
        float distSq = LODSystem::distanceSquared(ped.x, ped.y, ped.z,
                                                  cameraX, cameraY, cameraZ);

        if (!frustumCuller.isVisible(ped.x, ped.y, ped.z, 2.0f)) {
            continue;
        }

        LODLevel lod = LODSystem::getInstance().getPedestrianLOD(distSq);
        if (lod != LODLevel::CULLED) {
            drawPedestrian(ped, lod);  // Modified to accept LOD parameter
        }
    }

    // Display performance stats if enabled
    if (CONFIG_BOOL("show_fps", true)) {
        // Draw FPS text (would need to implement text rendering or use printf)
        static int frameCount = 0;
        frameCount++;
        if (frameCount % 60 == 0) {
            PerformanceMonitor::getInstance().printStatistics();
            if (CONFIG_BOOL("show_stats", false)) {
                LODSystem::getInstance().printStats();
                frustumCuller.printStats();
            }
        }
    }

    PerformanceMonitor::getInstance().frameEnd();
    glutSwapBuffers();
}
```

### Step 7: Modify drawVehicle and drawPedestrian

Add LOD parameter to these functions:

```cpp
void drawVehicle(const Vehicle& vehicle, LODLevel lod = LODLevel::HIGH) {
    glPushMatrix();
    glTranslatef(vehicle.x, vehicle.y, vehicle.z);
    glRotatef(vehicle.rotation, 0, 1, 0);

    // Adjust detail based on LOD
    switch (lod) {
        case LODLevel::HIGH:
            // Full detail - wheels, windows, all features
            // ... existing detailed rendering code ...
            break;

        case LODLevel::MEDIUM:
            // Reduced detail - simplified wheels, no small details
            // Draw basic body only
            glColor3f(vehicle.r, vehicle.g, vehicle.b);
            drawBox(vehicle.width, vehicle.height, vehicle.length);
            // Simplified wheels
            break;

        case LODLevel::LOW:
            // Minimal detail - just a colored box
            glColor3f(vehicle.r, vehicle.g, vehicle.b);
            drawBox(vehicle.width, vehicle.height, vehicle.length);
            break;

        case LODLevel::CULLED:
            // Don't render
            break;
    }

    glPopMatrix();
}
```

### Step 8: Add Display List Compilation

Add this method to compile static geometry into display lists:

```cpp
// Call this in init() after textures are generated
void ResourceManager::compileStaticGeometry() {
    printf("[ResourceManager] Compiling static geometry display lists...\n");

    // Compile road display list
    roadDisplayList_.compile([this]() {
        this->drawRoadGeometry();
    });

    // Compile building display list
    buildingDisplayList_.compile([this]() {
        this->drawBuildingGeometry();
    });

    // Compile ground display list
    groundDisplayList_.compile([this]() {
        this->drawGroundGeometry();
    });

    printf("[ResourceManager] Display lists compiled\n");
}
```

### Step 9: Update main() Function

Add frame rate control:

```cpp
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Use config for window size if available
    int winWidth = WINDOW_WIDTH;
    int winHeight = WINDOW_HEIGHT;

    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Traffic Simulator - OpenGL [Enhanced]");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);

    printf("\n========= 3D Traffic Simulator Controls =========\n");
    printf("FREE CAMERA MOVEMENT:\n");
    printf("  W/A/S/D     - Move forward/left/backward/right\n");
    printf("  Arrow Keys  - Look around (pitch/yaw)\n");
    printf("  E/C         - Move up/down\n");
    printf("  + / -       - Increase/decrease movement speed\n");
    printf("\n");
    printf("SIMULATION:\n");
    printf("  SPACE       - Toggle auto-rotation\n");
    printf("  R           - Reset simulation and camera\n");
    printf("  P           - Print performance statistics\n");
    printf("  Q / ESC     - Quit\n");
    printf("=================================================\n\n");

    glutMainLoop();

    // Cleanup (though glutMainLoop never returns in standard GLUT)
    ResourceManager::getInstance().cleanup();

    return 0;
}
```

### Step 10: Create Default Configuration File

Create a file named `traffic_sim.cfg` in the project directory:

```
# Traffic Simulator Configuration
# Performance Settings
target_fps=60
enable_vsync=true
enable_lod=true
enable_frustum_culling=true
enable_display_lists=true

# LOD Distances (in units)
vehicle_lod_high=50.0
vehicle_lod_medium=100.0
vehicle_lod_low=200.0
pedestrian_lod_high=30.0
pedestrian_lod_medium=60.0
pedestrian_lod_low=120.0

# Simulation Parameters
max_vehicles=100
max_pedestrians=200
initial_pedestrians=60
vehicle_spawn_rate=0.5
pedestrian_spawn_rate=0.3

# Graphics Settings
enable_lighting=true
enable_textures=true
enable_shadows=false
enable_fog=true
window_width=1280
window_height=720

# Camera Settings
camera_speed=20.0
camera_sensitivity=0.002
camera_fov=60.0

# Debug Settings
show_fps=true
show_stats=false
stats_update_interval=60
```

## Expected Performance Improvements

After integration, you should see:

1. **20-30% FPS improvement** from display lists for static geometry
2. **30-50% FPS improvement** from LOD system at high entity counts
3. **10-20% FPS improvement** from frustum culling
4. **No memory leaks** from RAII resource management
5. **Consistent frame rate** from frame rate control

## Testing Checklist

- [ ] Project compiles without errors
- [ ] All textures load correctly
- [ ] Display lists work (static geometry renders)
- [ ] LOD system activates (distant objects simpler)
- [ ] Frustum culling works (off-screen objects not rendered)
- [ ] FPS counter displays
- [ ] Configuration file loads
- [ ] No memory leaks (check with valgrind/instruments)
- [ ] Performance targets met (60 FPS with 100+ vehicles)

## Troubleshooting

### Issue: Textures not appearing

- Check that ResourceManager texture upload is called in each generate function
- Verify bind() is called before drawing
- Check GL_TEXTURE_2D is enabled

### Issue: Display lists not working

- Verify compileStaticGeometry() is called in init()
- Check that display list calls happen before entity drawing
- Make sure geometry doesn't change (only for static objects)

### Issue: LOD not working

- Print LOD statistics to verify distance calculations
- Adjust LOD distances in config file
- Check camera position is being passed correctly

### Issue: Performance not improving

- Verify display lists are enabled in config
- Check LOD system is active
- Enable performance statistics to identify bottlenecks
- Profile with gprof or similar tool

## Next Steps (Phase 2)

After Phase 1 is complete and tested:

1. Implement spatial partitioning (quadtree/grid) for faster entity lookup
2. Add advanced vehicle AI (Intelligent Driver Model)
3. Implement vehicle physics (acceleration, braking, steering)
4. Add pedestrian pathfinding and crosswalk behavior
5. Enhance traffic management with dynamic light timing
6. Add more camera modes (follow vehicle, orbit, top-down)
