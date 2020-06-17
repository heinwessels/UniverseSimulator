# Universe Simulator
An universe simulator I'm building to learn C++. It will have a `n`-body gravitation simulation, and combining bodies on collision. My goal is to start with a random "gas cloud", and end up with multiple bodies orbiting a larger body. A secondary goal is that it ends up with circular orbits (and not just elliptical orbits). And if it creates twin orbiting suns like Tatooine has, that would be an amazing bonus.

![](gifs/second.gif)

Here's an example of the simulation. Although the bodies are rendered as squares to save time, the calculations are applied as circles. It's visible that a gas cloud creates orbiting bodies (planets) around a much larger body (star). It's interesting to see that most of the bodies orbit in the same direction. If there's one going the oposite direction it typically crashes into another body, loses it's speed, and then crashes into the sun.


## Optimizations Applied:
- Removed trigometric functions from calculating gravity the force's two components.
- The gravity force calculation is `O(nlogn)` by calculating the force between two bodies only once.
- All the forces acting on a single body is accumulated first, and only applied onto the body once at the end of the main loop.
- The previous point allows storing only a single instance of all the bodies. Otherwise, applying a force to a body will change that body's state before other bodies' calculations are done, resulting in a inaccurate simulation.
- Using arbitray units rather than simulating the actual unniverse. For example, distance is in `pixels`, speed in `pixels/second`, etc. This avoids costly conversions between `km` and `pixels`, and using the `G`-constant.
- `kcachegrind` shows that `SDL_PollEvent` to handle keyboard input used >50% of the processing time. Therefore I implemented UPS/FPS limits, and now only call `handle_input` once for every rendered frame.
- Changed body storage from `vector` to `array`, because `vector::at()` was taking a significant chunk of processing (>40%). (From `kcachegrind`)
- Changed `pow(x, 2)` to `x*x`. `Pow` is much slower than multiplication for `^2` according to the internet. (Found major culprit using `kcachegrind`)

## Things I like
- Combining bodies on collision are (reasonably) realistic
  - Combined body's location is at Center of Mass of two colliding bodies.
  - Combined body's speed follows law of Conservation of Momentum
  - Combined body's radius is based on combining the colliding bodies' area (mimicing mass)
- Gravity is calculated as a 3D force, since in 2D stable orbits isn't possible (it creates a flower pattern, instead of conic sections).
- Uses Perlin Noise to generate a random gas cloud, i.e. bodies with random mass, location and speed. Using Perlin noise ensures basicaly creates clusters of bodies with similar speed and mass, mimicing a steady state gas cloud where clusters of gas are moving in mostly the same direction (probably).  The hope is that it would help create orbiting bodies, instead of all bodies simply collapsing into a single star.
- Ability to start/stop/single-step the simulation.
- A good Updates-Per-Seconds (UPS) and Frames-Per-Second (FPS) system, similar to Factorio. This is printed in the GUI.
- Option center itself around it's total Center of Mass.
- Take a screenshot every n updates to make some sweet gifs.
- Change scale factor to zoom in/out using keyboard input, with intuitive way to select a factor from `1` to `0.00001`.

## TODO
- ~~Run `kcachegrind` and optimize accordingly.~~
  - Next contender is calculations done in vector template. Deemed not worth the gain in performance.
- Handle stationary/neglable bodies correctly during collisions. Currently they might lose some of their traits.
- Improve UPS/FPS system to prioritize UPS over FPS by specifying a minimum FPS or UPS.

## Another GIF
Here is another example of a simulation, this one almost resulting in a circular orbit. Notice how twice the the almost-circular orbiting body collides with other bodies from very high and elliptical orbits. These collisions are almost perfectly timed to make the main orbit *more* circular, hitting the main body almost at it's very efficient apoapsis on one occation.


![](gifs/first.gif)