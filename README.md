**Please Note:** This simulation was rewritten which can render circles, and has a much better name. It's called Universe Simulator 2, and can be found [here](https://github.com/heinwessels/UniverseSimulator2).

# Universe Simulator
This is a universe simulator I'm building to learn C++, and my first real project in this language. It has a `n`-body gravitation simulation, which combines bodies on collision. My goal is to start with a random "gas cloud" and end up with multiple bodies orbiting a larger body. The secondary goal is that the system should end up with circular orbits, and not just orbits with high eccentricity. If it creates twin orbiting suns like Tatooine has, then that would be an amazing bonus.

Here's an example of the simulation:

![](gifs/second.gif)

The bodies in the simulation are rendered as squares to save time, however, the calculations are applied as circles. It's visible that a gas cloud creates orbiting bodies (planets) around a much larger body (star). It's interesting to see that most of the bodies orbit in the same direction. If there's one going the oposite direction it typically crashes into some other body, loses it's speed, and then crashes into the "*sun*".


## Lessons Learned / Optimizations:
- The gravity force calculation is `0.5 * O(n^2)` by calculating the force between two bodies only once. This is possible by applying Netwon's 3rd law.
- All the forces acting on a single body are accumulated first, and only applied onto the body once - at the end of the main loop.
- The previous point allows for storing of only a single instance of all the bodies. Otherwise, applying a force to a body will change that body's state before other bodies' calculations are done, resulting in an inaccurate simulation.
- `kcachegrind` showed that `SDL_PollEvent`, which handles keyboard input, used >50% of the processing time. Therefore FPS limits were implemented which made it possible to only call `handle_input` once for every rendered frame.
- Changed body storage from `std::vector` to `std::array`, because `std::vector::at()` was taking a significant chunk of processing, >40% according to `kcachegrind`. The `std::vector[index]` notation was not known before changing, so it was not tested.
- Changed `pow(x, 2)` to `x*x`. The `pow` funtion is much slower than multiplication for `x^2` according to the internet. Found this culprit using `kcachegrind`.
- Removed trigometric functions for calculating the gravity force's two components, and now rather using vector mathmatics.

## Things I like
- Combining bodies on collision are (reasonably) realistic
  - Combined body's location is at Center of Mass of two colliding bodies.
  - Combined body's speed follows Law of Conservation of Momentum
  - Combined body's radius is based on combining the colliding bodies' area (mimicing mass)
- Gravity is calculated as a 3D force, since in 2D stable orbits aren't possible (it creates a flower pattern, instead of conic sections).
- Perlin Noise is used to generate a random gas cloud, i.e. bodies with random mass, location and speed. Using Perlin noise basicaly creates clusters of bodies with similar speed and mass, mimicing a steady state gas cloud where clusters of gas are moving in mostly the same direction (probably).  The hope is that it would help create orbiting bodies instead of all bodies simply collapsing into a single star.
- Ability to start/stop/single-step the simulation.
- A good Updates-Per-Seconds (UPS) and Frames-Per-Second (FPS) system, similar to Factorio. This is printed in the GUI.
- Option to center the universe around it's combined Center of Mass for easier viewing.
- Take a screenshot every `n` updates to make some sweet gifs.
- Change scale factor to zoom in/out using keyboard input, with intuitive way to select a factor from `5` to `0.00001`.

## TODO
- ~~Run `kcachegrind` and optimize accordingly.~~
  - Next contender is calculations done in my vector template (`Vec3`). Deemed not worth the gain in performance.
- Handle stationary/neglable bodies correctly during collisions. Currently they might lose some of their traits.
- Improve UPS/FPS system to prioritize UPS over FPS by specifying a minimum FPS or UPS.
- Initially used arbitrary units rather than real-world units to save processing time. For example, distance in `pixels`, speed in `pixels/second`, and not using the `G`-constant. However, the very slight increase in performance is not worth the big sacrifice in readability. Therefore, change back to real-world units at some point.
- If rendered body is big enough, rather render as circle. Processing time increase will be neglable.


## Build
This was developed on Ubuntu using C++17. The compile command used is:

`clang++-6.0 -g ${fileDirname}/*.cpp -o ${fileDirname}/run.exe -lSDL2 -lSDL2_ttf -std=c++17 -O3`

## Another GIF
Here is another example of a simulation, this one almost resulting in a circular orbit of one body (i.e. *earth*) around the other (i.e. *sun*). Notice how this *earth*-body collides with other bodies, from very eccentric orbits, which circularises its orbit even more. And these collisions are almost perfectly timed to achieve circularisation. The last collision hits *earth* at the most efficient location in orbit to circularise its orbit, namely the apoapsis.


![](gifs/first.gif)
