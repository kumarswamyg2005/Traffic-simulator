# Camera Mode 3: First-Person Pedestrian View

## Overview

Camera Mode 3 now provides an **immersive first-person perspective** from a pedestrian's point of view, looking over their shoulder as they walk through the city.

## How It Works

### Camera Position

- **Location**: Back of the pedestrian's head (0.2 units behind head center)
- **Height**: ~1.5–1.7 meters (scaled by pedestrian height)
- **Vision**: You see the world from their eye level

### Camera Direction (Look Target)

- **Direction**: Forward in the pedestrian's walking direction
- **Distance**: 25 units ahead (so you see far down the street)
- **Gaze**: Slightly downward (~95% of eye height) for natural human perspective

### Smooth Transitions

**Two types of smoothing:**

1. **Between pedestrians (ON SWITCH)**

   - When you press **N** or **M** to cycle, the camera **instantly resets** to the new person's head
   - Then smoothly interpolates to their view
   - This gives a natural "head snap" effect

2. **While following same person (CONTINUOUS)**
   - Smooth factor: `0.02f` (very smooth)
   - Camera and look-target continuously interpolate
   - Reduces jitter/shake as pedestrian moves and turns

## Controls

| Key     | Action                                                            |
| ------- | ----------------------------------------------------------------- |
| `3`     | Activate Camera Mode 3 (Follow Pedestrian)                        |
| `N`     | Next pedestrian (smooth head snap, then resume smooth follow)     |
| `M`     | Previous pedestrian (smooth head snap, then resume smooth follow) |
| `1`–`4` | Switch to other camera modes                                      |

## Key Settings

Located in `main.cpp`:

```cpp
// Line ~5065: Eye height
float eyeHeight = std::max(1.50f, 1.70f * p.heightScale);

// Line ~5069: Back offset (how far behind head)
float backOffset = 0.2f;

// Line ~5074: Look distance (how far ahead to see)
float lookDistance = 25.0f;

// Line ~5016: Smooth interpolation factor
float smoothFactor = 0.02f;  // Lower = smoother (but less responsive)
```

## Visual Experience

- **Immersive**: You experience the city from a human-scale perspective
- **Natural movement**: Head bobs and sways as pedestrian walks
- **Dynamic**: View updates as pedestrian changes direction
- **Smooth**: No jarring camera snaps (except on person switch)

## Troubleshooting

**Camera too shaky?**
→ Lower `smoothFactor` (e.g., `0.015f`)

**Camera too far behind?**
→ Reduce `backOffset` (e.g., `0.1f`)

**Look distance too short?**
→ Increase `lookDistance` (e.g., `30.0f`)

**Can't see pedestrian's head?**
→ Adjust `eyeHeight` or `backOffset`
