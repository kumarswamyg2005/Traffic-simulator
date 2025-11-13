# Camera Control Guide

## Camera Modes

The simulator has 4 different camera modes that you can switch between:

### Mode 1: FREE Camera (Press '1')

- **Description**: Free-flying camera that you control manually
- **Controls**:
  - `W` - Move forward
  - `S` - Move backward
  - `A` - Strafe left
  - `D` - Strafe right
  - `E` - Move up
  - `C` - Move down
  - `Arrow Keys` - Look around (pitch and yaw)
  - `+/=` - Increase camera speed
  - `-/_` - Decrease camera speed

### Mode 2: FOLLOW CAR (Press '2')

- **Description**: Camera follows a specific vehicle from behind
- **Controls**:
  - `N` - Switch to **next** vehicle
  - `M` - Switch to **previous** vehicle
- **Note**: The camera smoothly follows the selected car

### Mode 3: FOLLOW PEDESTRIAN (Press '3')

- **Description**: Camera follows a specific pedestrian from behind
- **Controls**:
  - `N` - Switch to **next** pedestrian
  - `M` - Switch to **previous** pedestrian
- **Note**: The camera smoothly follows the selected pedestrian

### Mode 4: BUILDING VIEW (Press '4')

- **Description**: Bird's eye view of the entire city
- **Controls**: None (fixed position)

## Important Notes

1. **M and N keys only work in modes 2 and 3**: If you press M or N while in FREE (mode 1) or BUILDING VIEW (mode 4), you'll see a message telling you to switch to a follow mode first.

2. **Check the console**: When you press M or N, the console will show which vehicle/pedestrian you're now following (e.g., "Following vehicle 5 of 12").

3. **Mode persistence**: The simulator remembers which vehicle/pedestrian you were following when you switch modes.

## Common Issues

### "Camera keeps changing when I press M/N"

- **Solution**: Make sure you're in the correct mode (2 for cars, 3 for pedestrians)
- The console will show a message if you're in the wrong mode

### "Nothing happens when I press M or N"

- **Cause**: You might be in mode 1 (FREE) or mode 4 (BUILDING VIEW)
- **Solution**: Press '2' for car follow mode or '3' for pedestrian follow mode first

### "Camera jumps around erratically"

- **Cause**: The followed entity (car/pedestrian) might be moving or turning sharply
- **Solution**: Press N or M to switch to a different, more stable target
