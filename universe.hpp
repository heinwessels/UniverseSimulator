#include <vector>
#include <chrono>
#include "vec3.hpp"
#include "body.hpp"
#include <stdio.h>
#include<iostream>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "PerlinNoise.hpp"

#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

using namespace std;

class Universe {

public:

    float time_step = 0.1;

    const bool render = true;
    const int UPS_limit = 50.0;
    const int FPS_limit = 10.0;

    int screenWidth = 1000;
    int screenHeight = 1000;
    TTF_Font *gfont;
    // const char * gfont_path = "FFFFORWA.TTF";
    const char * gfont_path = "PixelSplitter-Bold.ttf";
    int gfont_size = 18;
    SDL_Window *gWindow;
    SDL_Renderer *gRenderer;



    bool show_force_on_body = false;
    bool show_acc_on_body = false;
    bool show_speed_on_body = false;

    vector<Body> bodies;

    enum UniverseState {idle, running, single_step};
    UniverseState state = running;

    Universe();

    bool step_universe();
    void step_through_bodies();
    void check_for_collisions_and_combine();
    Vec3<float> calculate_gravity_force_between(Body& this_body, Body& that_body);
    void init_random_bodies();

    void screen_render(float ups, float fps);
    void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Rect *rect, SDL_Color *color);
    bool handle_input();
    bool screen_init();
};





#endif