# Weather and Fog Controls Guide

## New Features Added

### 🌧️ Rain Toggle (J Key)

- **Press J**: Toggle rain on/off independently
- Rain particles fall continuously when enabled
- Works separately from the old weather system (P key)
- Automatic particle spawning around camera
- Can be used at any time (day or night)

### 🌫️ Fog Toggle (F Key)

- **Press F**: Toggle fog effect on/off
- Creates atmospheric fog that reduces visibility
- Fog density: Exponential (EXP2 mode) for realistic look
- Fog color: Gray (0.7, 0.7, 0.75)
- Great for creating mysterious/moody atmosphere
- Objects fade into fog at distance

## All Weather Controls

| Key   | Function      | Description                |
| ----- | ------------- | -------------------------- |
| **J** | Rain Toggle   | Enable/disable rain effect |
| **F** | Fog Toggle    | Enable/disable fog effect  |
| **L** | Clear Weather | Remove all weather effects |
| **P** | Rain System   | Old weather system rain    |
| **O** | Snow          | Enable snow particles      |

## Usage Tips

1. **Rainy Day Scene**: Press `J` for rain + `F` for fog
2. **Clear Visibility**: Press `L` to clear all weather
3. **Foggy Morning**: Press `F` alone for fog without rain
4. **Heavy Rain**: Press `J` for independent rain control
5. **Combine Effects**: Use both J and F together for dramatic weather

## Technical Details

### Rain Settings

- **Particle Count**: Up to 2000 raindrops
- **Fall Speed**: -3.0 units/frame (vertical)
- **Spawn Height**: 150-200 units
- **Spawn Radius**: 200 units around camera
- **Color**: Light blue (0.6, 0.6, 0.8)

### Fog Settings

- **Mode**: GL_EXP2 (exponential squared)
- **Density**: 0.035
- **Start Distance**: 80 units
- **End Distance**: 300 units
- **Color**: Gray (0.7, 0.7, 0.75)

## Code Customization

### Change Rain Intensity

```cpp
// Line ~228: Adjust fog density
fogDensity = 0.035f;  // Increase for thicker fog (try 0.05 for heavy fog)

// Line ~1178: Adjust rain spawn rate
for (int i = 0; i < 20; i++) {  // Increase for heavier rain (try 30-40)
```

### Change Fog Thickness

```cpp
// Line ~5393: In keyboard function
glFogf(GL_FOG_DENSITY, fogDensity);  // Change fogDensity value
```

### Change Fog Color

```cpp
// Line ~5390: Fog color
GLfloat fogColor[4] = {0.7f, 0.7f, 0.75f, 1.0f};
// Try {0.9f, 0.9f, 0.9f, 1.0f} for white fog
// Try {0.5f, 0.5f, 0.6f, 1.0f} for darker fog
```

## Example Scenes

**Scene 1: Rainy City**

```
Press T - Switch to night
Press J - Enable rain
Result: Rainy night scene with falling rain
```

**Scene 2: Foggy Morning**

```
Press T - Switch to day
Press F - Enable fog
Result: Foggy daytime city with reduced visibility
```

**Scene 3: Storm**

```
Press T - Switch to night
Press J - Enable rain
Press F - Enable fog
Result: Stormy night with rain and fog
```

**Scene 4: Clear Day**

```
Press T - Switch to day
Press L - Clear all weather
Result: Bright sunny day with full visibility
```

## Feature Comparison

### New J Key Rain vs Old P Key Rain

- **J Key**: Independent toggle, works anytime
- **P Key**: Part of old weather system
- **Recommendation**: Use J key for better control

### Fog Modes

- **EXP2 Mode**: More realistic, gradual fade
- **LINEAR Mode**: Sharp fog start/end distances
- **Current**: Using EXP2 for better visuals
