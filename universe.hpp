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

    float time_step = 0.1; 
    float delay = 0;  // in ms

    int render_frame = 100;
    int screenWidth = 1000;
    int screenHeight = 1000;
    SDL_Window *gWindow;
    SDL_Renderer *gRenderer;
    
    bool show_force_on_body = false;
    bool show_acc_on_body = false;

    vector<Body> bodies;

    Universe();

    bool step_universe();
    void step_through_bodies();
    Vec2f calculate_gravity_force_between(Body& this_body, Body& that_body);

    bool screen_render();
    bool screen_init();
};





#endif