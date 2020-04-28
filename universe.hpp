#include <vector> 
#include "vec2f.hpp"
#include "body.hpp"
#include <stdio.h>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

using namespace std;

class Universe {

public:

    float time_step = 60*60; 

    const float g = 6.67408e-11f; // In m^3 kg^(-1) s^(-2)

    int screenWidth = 640;
    int screenHeight = 480;
    float screenScale = 2e6;
    SDL_Window *gWindow;
    SDL_Renderer *gRenderer;

    vector<Body> bodies;

    Universe();

    bool step_universe();
    void step_through_bodies();
    Vec2f calculate_gravity_force_between(Body& this_body, Body& that_body);

    bool screen_render();
    bool screen_init();
};





#endif