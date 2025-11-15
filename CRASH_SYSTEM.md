# 💥 Vehicle Collision & Crash Physics System

## Overview

Vehicles now realistically detect collisions, crash, flip, and settle with full physics simulation.

## Features

### 1. **Collision Detection**

- Automatic detection when two vehicles get too close
- Calculates impact force from relative velocities
- Considers vehicle types (cars flip easier than trucks/buses)

### 2. **Crash Physics**

- **Gravity** - Vehicles fall realistically
- **Bounce** - Multiple bounces with dampening
- **Rotation** - Spinning and tumbling mid-air
- **Rollover** - Can land upside-down or on side
- **Sliding** - Post-crash sliding with friction

### 3. **Visual Effects**

- Darkened vehicle color after crash (damaged appearance)
- Smoke particles at impact point
- Flickering effects during active crash
- Realistic rotation animations (roll + pitch)

### 4. **Crash Behavior**

**Light Impact** (low speed):

- Vehicle spins slightly
- Small bounce
- Settles quickly upright

**Medium Impact**:

- Significant spin and bounce
- May flip once
- Takes longer to settle

**Heavy Impact** (high speed):

- Multiple flips and spins
- High bounce
- Can land upside-down
- Extended crash sequence

## How It Works

### Crash Sequence

1. **Detection** - Vehicles too close → collision triggered
2. **Impact** - Force calculated, velocities assigned
3. **Airborne** - Vehicle flips/spins with physics
4. **Bounce** - Hits ground, bounces with damping
5. **Settle** - Eventually comes to rest

### Physics Constants

```cpp
GRAVITY = 0.01f          // Downward acceleration
FRICTION = 0.94f         // Velocity decay
BOUNCE_DAMPING = 0.25f   // Energy loss per bounce
```

### Crash States

- `crashed = true` - Vehicle is in crash mode
- `settled = false` - Still moving/bouncing
- `settled = true` - Stopped moving (final position)

## Console Output

When crashes occur, you'll see:

```
💥 CRASH! Vehicle 5 hit vehicle 12
💥 CRASH! Vehicle 23 hit vehicle 8
```

## Tips

**To see more crashes:**

- Increase traffic density (press `6` or `9`)
- Use camera mode 1 (free camera) to see city overview
- Press `4` for building view to watch from above
- Crashes happen naturally - just wait and watch!

**Crash hotspots:**

- Busy intersections
- When lights change
- High-speed roads

## Technical Details

### Vehicle Struct Additions

```cpp
bool crashed          // Is vehicle crashed?
float rollAngle       // Side-to-side flip rotation
float pitchAngle      // Front-to-back flip rotation
float crashVelocityX  // Post-crash X velocity
float crashVelocityZ  // Post-crash Z velocity
float crashVelocityY  // Vertical (bounce) velocity
float crashRotation   // Spin speed
int crashFrame        // Frames since crash
bool settled          // Has stopped moving?
```

### New Functions

- `detectCollisions()` - Checks all vehicle pairs
- `updateCrashedVehicles()` - Physics simulation
- `updateCrashEffects()` - Particle effects
- Modified `drawVehicle()` - Renders crashed state

### Cleanup

Crashed vehicles are automatically removed after 900 frames (~15 seconds) to prevent clutter.

## Adjusting Crash Behavior

### Make crashes more dramatic:

```cpp
// In detectCollisions(), increase flip chance:
v1.crashVelocityY = 0.25f + impactForce * 0.5f;  // Higher jumps
v1.crashRotation = ... * 10.0f;  // Faster spins
```

### Make crashes less frequent:

```cpp
// In detectCollisions(), reduce collision distance:
float collisionDist = (v1.depth + v2.depth) * 0.4f;  // Tighter threshold
```

### Longer crash sequences:

```cpp
// In updateCrashedVehicles(), reduce friction:
const float FRICTION = 0.98f;  // Slides further
```

## Enjoy the Carnage! 🚗💨💥

Watch vehicles crash, flip, spin, and settle in realistic physics-based mayhem!
