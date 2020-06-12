# UniverseSimulator
An universe simulator I'm building to learn C++. It will have a n-body gravitation simulation, and combining bodies on collision. My goal is to start with a random "gas cloud", and end up with multiple bodies orbiting a larger body. A secondary goal is that it ends up with circular orbits (and not just elliptical). And if it creates twin orbiting suns like Tatooine has, that would be an amazing bonus.


## Optimizations Applied:
- Removed trigometric functions from calculating gravity force's two components.
- The gravity force calculation is `O(nlogn)` by calculating the force between two bodies only once.
- All the forces acting on a single body is accumulated first, and only applied to the body once at the end of the main loop.
- The previous point allows storing only a single instance of all the bodies. Otherwise, applying a force to a body will change that body's state before other bodies' calculations are done, resulting in a inaccurate simulation.
- Using arbitray units rather than simulating the actual unniverse. For example, distance is in `pixels`, speed in `pixels/second`, etc. This avoids costly conversions between `km` and `pixels`.
- `kcachegrind` shows that `SDL_PollEvent` used >50% of the processing time. Therefore implemented UPS/FPS limits, and only call `handle_input` once for every rendered frame.

## Things I like
- Combining bodies on collision are (reasonably) realistic
  - Combined body's location is at Center of Mass of two colliding bodies.
  - Combined body's speed follows law of Conservation of Momentum
  - Combined body's radius is based on combining the colliding bodies' area (mimicing mass)
- Gravity is calculated as a 3D force, since in 2D stable orbits isn't possible (it creates a flower pattern, instead of conic sections).
- Uses Perlin Noise to generate a random gas cloud, i.e. bodies with random mass, location and speed. Using Perlin noise ensures basicaly creates clusters of bodies with similar speed and mass, mimicing a steady state gas cloud where clusters of gas are moving in mostly the same direction (probably).  The hope is that it would help create orbiting bodies, instead of all bodies simply collapsing into a single star.
- Ability to start/stop/single-step the simulation.
- A good Updates-Per-Seconds (UPS) and Frames-Per-Second (FPS) system, similar to Factorio. This is printed in the GUI.
- When below a configurable number of bodies are left the universe can center itself around it's total Center of Mass.

## TODO
- Run `kcachegrind` and optimize accordingly.
- Handle stationary bodies correctly during collisions.
- Improve UPS/FPS system to prioritize UPS over FPS.
