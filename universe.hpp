#include <vector>
#include <chrono>
#include "vec3.hpp"
#include "body.hpp"
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "PerlinNoise.hpp"

#ifndef UNIVERSE_HPP

#define UNIVERSE_HPP

class Universe {

public:

    const static int MAX_BODIES = 100000;

    uint32_t gseed = 802;

    float time_step = 0.1;

    const bool render = true;
    const float UPS_limit = 10000.0;
    const float FPS_limit = 5.0;
    bool universe_centralize = true;
    float universe_scale_factor = 0.5;
    int take_screenshot_every = 5; // [steps], 0 to disable. Overwrites UPS/FPS settings

    int screenWidth = 1000;
    int screenHeight = 1000;
    TTF_Font *gfont;
    const char * gfont_path = "PixelSplitter-Bold.ttf";
    int gfont_size = 18;
    SDL_Window *gWindow;
    SDL_Renderer *gRenderer;

    bool show_force_on_body = false;
    bool show_acc_on_body = false;
    bool show_speed_on_body = false;

    Body bodies [MAX_BODIES];
    int num_of_bodies = 0;

    enum UniverseState {idle, running, single_step};
    UniverseState state = running;

    Universe();

    bool step_universe();
    void step_through_bodies();
    void check_for_collisions_and_combine();
    Vec3<float> calculate_gravity_force_between(Body& this_body, Body& that_body);
    Vec3<float> calculate_universe_com();
    void center_universe_around(Vec3<float> center);
    void init_random_bodies();

    void screen_render(float ups, float fps);
    void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Rect *rect, SDL_Color *color);
    bool handle_input();
    bool screen_init();
    void screenshot(string path);
};





#endif