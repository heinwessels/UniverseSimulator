#include "universe.hpp"

Universe::Universe(){
    printf("Universe Startup\n");

    state = running;

    gRenderer = NULL;
    gWindow = NULL;

    Body sun (250, 20, Vec3<float>(screenWidth / 2, screenHeight / 2, 0));

    Body earth (10, 5, Vec3<float>(screenWidth / 2 + 250, screenHeight / 2, 0));
    earth.speed = Vec3<float>(0, 1, 0);

    Body moon (10, 5, Vec3<float>(screenWidth / 2 - 250, screenHeight / 2, 0));
    moon.speed = Vec3<float>(0, -1, 0);

    // bodies[0] = moon;
    // bodies[1] = sun;
    // bodies[2] = earth;
    // num_of_bodies = 3;

    init_random_bodies();

    if (render){
        screen_init();
        screen_render(0, 0);
    }

    while(step_universe()){}

    // Cleanup
    TTF_Quit();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool Universe::step_universe(){

    using clock = std::chrono::system_clock;
    using sec = std::chrono::duration<double>;

    static float prev_ups = UPS_limit;
    static float prev_fps = FPS_limit;
    static sec last_render_duration (0);

    const auto before = clock::now();

    bool cont = true;

    // Count updates to ensure do not pass max updates per frame,
    // or maximum FPS (taking into account rendering time)
    int count_updates = 0;
    static int count_total_updates = 0;
    sec fps_duration = clock::now() - before;
    if (take_screenshot_every){
        while(count_updates++ < take_screenshot_every){
            step_through_bodies();
            check_for_collisions_and_combine();
        }
    }
    else{
        while (
            (++count_updates < UPS_limit / prev_fps) &&
            ((fps_duration + last_render_duration).count() < 1.0f / FPS_limit)
        ){
            if (state == running || state == single_step){
                step_through_bodies();
                check_for_collisions_and_combine();

                if (state == single_step){

                    state = idle;
                }
            }

            // Calculate how long we've been in this loop
            fps_duration = clock::now() - before;
        }
    }
    fps_duration = clock::now() - before;
    if (take_screenshot_every)
        prev_ups = (float)take_screenshot_every / fps_duration.count();
    count_total_updates += count_updates - 1;


    // If the amount of bodies is low enough, center the universe
    if(universe_centralize){
        center_universe_around(calculate_universe_com());
    }

    // Now render the screen
    if (render){
        // Should we still wait before rendering? Unlikely... Unless we're taking screenshots
        fps_duration = clock::now() - before;
        while(fps_duration.count() < 1.0f/FPS_limit && !take_screenshot_every){
            SDL_Delay(1);
            fps_duration = clock::now() - before;
        }

        // Now render the screen
        const auto before_render = clock::now();
        screen_render(prev_ups, prev_fps);
        if (take_screenshot_every){
            screenshot("screenshot_" + std::to_string(count_total_updates));
        }
        cont = handle_input();  // Handle this here, as SDL_PollEvent is super slow
        sec last_render_duration = clock::now() - before_render;
    }

    // Now remember how long everything took, and display it on the next render
    fps_duration = clock::now() - before;
    prev_fps = 1.0f / fps_duration.count();
    if (!take_screenshot_every)
        prev_ups = (count_updates - 1) * prev_fps;  // -1 Due to how counter works

    return cont;
}

void Universe::step_through_bodies(){

    static Vec3<float> forces [MAX_BODIES];

    // Clear forces array
    for (int i = 0; i < num_of_bodies; i++){
        forces[i].x = 0;
        forces[i].y = 0;
    }

    // Step through the n bodies in O(n logn) to calculate the experienced gravitational forces
    for (int i = 0; i < num_of_bodies; i++){
        for (int j = i + 1; j < num_of_bodies; j++){

            // Is it neccesary to calculate the force between these two bodies?
            if (!bodies[i].stationary || bodies[i].neglible
                    || !bodies[i].stationary || bodies[i].neglible){

                // Calculate the force betwe en the two bodies
                Vec3<float> force = calculate_gravity_force_between(bodies[i], bodies[j]);

                // Apply the force to the body if it's not stationary (both of them)
                if (!bodies[i].stationary)
                    forces[i] += force;
                if (!bodies[j].stationary)
                    forces[j] -= force;  // Inverting it for the other body
            }
        }

        // This will exectute only when all forces on this body has been accounted for
        if (!bodies[i].stationary)
            bodies[i].apply_force(forces[i], time_step);
    }
}

void Universe::check_for_collisions_and_combine(){
    int i = 0, j = 0;
    while(i < num_of_bodies){
        j = i+1;
        while(j < num_of_bodies){

            Vec3<float> delta = bodies[j].pos - bodies[i].pos;
            float d2 = delta.x*delta.x + delta.y*delta.y;
            if(d2 < (bodies[i].radius + bodies[j].radius)*(bodies[i].radius + bodies[j].radius)){
                // Collision! Combine the bodies!
                // TODO Handle stationary objects correctly

                // Calculate the new location using the CoM of the two bodies
                bodies[i].pos = (bodies[i].pos * bodies[i].mass + bodies[j].pos * bodies[j].mass)
                                     / (bodies[i].mass + bodies[j].mass);

                // Calculate speed using law of conservation of energy
                bodies[i].speed = (bodies[i].speed * bodies[i].mass + bodies[j].speed * bodies[j].mass)
                                    / (bodies[i].mass + bodies[j].mass);

                // Calculate the new radius adding the area's of the two bodies
                bodies[i].radius = sqrt(bodies[i].radius*bodies[i].radius + bodies[j].radius*bodies[j].radius);
                bodies[i].mass += bodies[j].mass;

                // Now erase the body, by overwriting it with the last body in the array, and length - 1
                bodies[j] = bodies[--num_of_bodies];
            }

            j++;
        }

        i++;
    }
}

Vec3<float> Universe::calculate_gravity_force_between(Body& this_body, Body& that_body){

    // Calculate the distance cubed
    Vec3<float> dpos = (that_body.pos - this_body.pos);
    float r3 = dpos.x*dpos.x + dpos.y*dpos.y;
    r3 *=  sqrt(r3);

    // Calculate the absolute force divided by the distance
    float f = this_body.mass * that_body.mass / r3;

    // Return it's components
    return Vec3<float>(f * dpos.x, f * dpos.y, 0);
}

Vec3<float> Universe::calculate_universe_com(){
    Vec3<float> com (0);
    float total_mass = 0;
    for (int i = 0; i < num_of_bodies; i++){
        com += bodies[i].pos * bodies[i].mass;
        total_mass += bodies[i].mass;
    }
    return com / total_mass;
}
void Universe::center_universe_around(Vec3<float> center){
    Vec3<float> screen_center (screenWidth/2, screenHeight/2, 0);
    for (int i = 0; i < num_of_bodies; i++)
        bodies[i].pos += screen_center - center;
}

void Universe::screen_render(float ups, float fps){

    // Clear Screen
    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
    SDL_RenderClear(gRenderer);

    // Draw the bodies
    for (int i = 0; i < num_of_bodies; i++){

        SDL_Rect rect;
        if (universe_scale_factor == 1){
            rect.x = bodies[i].pos.x - bodies[i].radius;
            rect.y = bodies[i].pos.y - bodies[i].radius;
        }
        else{
            rect.x = (bodies[i].pos.x - screenWidth/2)*universe_scale_factor + screenWidth/2 - bodies[i].radius;
            rect.y = (bodies[i].pos.y - screenWidth/2)*universe_scale_factor + screenWidth/2 - bodies[i].radius;
        }
        rect.w = bodies[i].radius * 2 + 1;
        rect.h = bodies[i].radius * 2 + 1;
        SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 0 );
        SDL_RenderFillRect(gRenderer, &rect);

        if (show_force_on_body && bodies[i].last_force.x != 0){
            float f = sqrt(bodies[i].last_force.x*bodies[i].last_force.x + bodies[i].last_force.y*bodies[i].last_force.y);
            SDL_SetRenderDrawColor( gRenderer, 0, 255, 0, 0 );
            SDL_RenderDrawLine(
                gRenderer,
                bodies[i].pos.x,
                bodies[i].pos.y,
                bodies[i].pos.x + 3 * bodies[i].radius * bodies[i].last_force.x / f,
                bodies[i].pos.y + 3 * bodies[i].radius * bodies[i].last_force.y / f
            );
        }

        if (show_acc_on_body && bodies[i].last_acc.x != 0){
            float a = sqrt(bodies[i].last_acc.x*bodies[i].last_acc.x + bodies[i].last_acc.y*bodies[i].last_acc.y);
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 255, 0 );
            SDL_RenderDrawLine(
                gRenderer,
                bodies[i].pos.x,
                bodies[i].pos.y,
                bodies[i].pos.x + 3 * bodies[i].radius * bodies[i].last_acc.x / a,
                bodies[i].pos.y + 3 * bodies[i].radius * bodies[i].last_acc.y / a
            );
        }

        if (show_speed_on_body){
            float s = sqrt(bodies[i].speed.x*bodies[i].speed.x + bodies[i].speed.y*bodies[i].speed.y);
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 255, 0 );
            SDL_RenderDrawLine(
                gRenderer,
                bodies[i].pos.x,
                bodies[i].pos.y,
                bodies[i].pos.x + 3 * bodies[i].radius * bodies[i].speed.x / s,
                bodies[i].pos.y + 3 * bodies[i].radius * bodies[i].speed.y / s
            );
        }
    }

    // Print Some Information
    const int MAX_STRING_LEN = 50;
    SDL_Rect rect;
    char text[MAX_STRING_LEN];
    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    snprintf(text, MAX_STRING_LEN, "UPS/FPS : [%2.2f", ups);
    render_text(gRenderer, 10, 10, text, gfont, &rect, &color);
    snprintf(text, MAX_STRING_LEN, "/ %2.2f]", fps);
    render_text(gRenderer, rect.x + 300, 10, text, gfont, &rect, &color);
    snprintf(text, MAX_STRING_LEN, "Num of Bodies:  %d", num_of_bodies);
    render_text(gRenderer, 10, 33, text, gfont, &rect, &color);


    snprintf(text, MAX_STRING_LEN, "Step <.>");
    render_text(gRenderer, 10, screenHeight - 53, text, gfont, &rect, &color);
    snprintf(text, MAX_STRING_LEN, "Start/Stop <Space>: %s", state==running ? "Running" : "Idle");
    render_text(gRenderer, 10, screenHeight - 33, text, gfont, &rect, &color);
    snprintf(text, MAX_STRING_LEN, "Centralize <c>: %s", universe_centralize ? "Yes" : "No");
    render_text(gRenderer, rect.x + 400, screenHeight - 33, text, gfont, &rect, &color);
    snprintf(text, MAX_STRING_LEN, "Scale <+/->: %2.6f", universe_scale_factor);
    render_text(gRenderer, rect.x + 250, screenHeight - 33, text, gfont, &rect, &color);

    // Update the screen
    SDL_RenderPresent( gRenderer );
}

void Universe::render_text(
    SDL_Renderer *renderer,
    int x,
    int y,
    const char *text,
    TTF_Font *font,
    SDL_Rect *rect,
    SDL_Color *color
) {
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->x = x;
    rect->y = y;
    rect->w = surface->w;
    rect->h = surface->h;
    /* This is wasteful for textures that stay the same.
     * But makes things less stateful and easier to use.
     * Not going to code an atlas solution here... are we? */
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}

bool Universe::handle_input(){
    // Handle Input
    SDL_GetKeyboardState(NULL);

    static bool left_pressed_down = false;
    static bool right_pressed_down = false;

    SDL_Event event;
    while(SDL_PollEvent( &event ))
    {
        //User requests quit
        if( event.type == SDL_QUIT )
        {
            return false;
        }

        if (event.type == SDL_KEYDOWN){
            if (event.key.keysym.sym == SDLK_SPACE){
                if (state == idle){
                    state = running;
                }
                else if (state == running){
                    state = idle;
                }
            }
            if (event.key.keysym.sym == SDLK_PERIOD){
                state = single_step;
            }
            if (event.key.keysym.sym == SDLK_KP_PLUS){
                // Calcuate step size depending on current value (eg. step = 0.1 if 0.3, and 0.01 if 0.06, etc.)
                float step = pow(10,floor(log10(
                    universe_scale_factor + pow(10,floor(log10(universe_scale_factor)) - 1)
                )));
                if (step > 0.1)
                    step = 0.1;
                universe_scale_factor += step;
            }
            if (event.key.keysym.sym == SDLK_KP_MINUS){
                float step = pow(10,floor(log10(
                    universe_scale_factor - pow(10,floor(log10(universe_scale_factor)) - 1)
                )));
                if (step > 0.1)
                    step = 0.1;
                universe_scale_factor -= step;
            }
            if (event.key.keysym.sym == SDLK_c){
                universe_centralize = !universe_centralize;
            }
        }
    }

    return true;
}

void Universe::init_random_bodies(){

    srand(gseed);

    // std::cout << "---------------------------------\n";
	// std::cout << "* frequency [0.1 .. 8.0 .. 64.0] \n";
	// std::cout << "* octaves   [1 .. 8 .. 16]       \n";
	// std::cout << "* seed      [0 .. 2^32-1]        \n";
	// std::cout << "---------------------------------\n";
    // perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves)

    double size_frequency = 20;
    double size_octaves = 4;
    const siv::PerlinNoise size_perlin(gseed);
    const double size_fx = screenWidth / size_frequency;
    const double size_fy = screenHeight / size_frequency;

    double speed_frequency = 20;
    double speed_octaves = 4;
    const siv::PerlinNoise speed_perlin(gseed + 5);
    const double speed_fx = screenWidth / speed_frequency;
    const double speed_fy = screenHeight / speed_frequency;

    double density = 0.4;
    double minimim_radius = 1;
    double maximum_radius = 4;
    double spacing_multiplier = 5;
    double speed_multiplier = 1;

    int offset = 30;
    int x = offset, y = offset;
    while (y < screenHeight - offset){

        double size_randomizer = size_perlin.normalizedOctaveNoise2D(x / size_fx, y / size_fy, size_octaves);
        double radius = (size_randomizer + 1) * maximum_radius / 2;
        if (radius > minimim_radius && size_randomizer > 0){
            Body body (
                M_PI*radius*radius*density,
                radius,
                Vec3<float>(x, y, 0));

            double speed_randomizer = speed_perlin.normalizedOctaveNoise2D(x / speed_fx, y / speed_fy, speed_octaves) / 0.1;
            // double speed_randomizer = (rand()%200)/100.0 - 1.0;
            body.speed = Vec3<float>(
                sin(speed_randomizer * M_PI * 2) * size_randomizer * speed_multiplier,
                cos(speed_randomizer * M_PI * 2) * size_randomizer * speed_multiplier,
                0
            );

            bodies[num_of_bodies++] = body;

            // printf("[%d, %d]:\n\tRadius: %.3f\n\tSpeed Rand: %.3f\n", x, y, radius, speed_randomizer);

        }
        else
        {
            radius = minimim_radius;
        }

        x += spacing_multiplier * radius;
        if(x >= screenWidth - offset){
            x = offset;
            y += radius * spacing_multiplier;
        }
    }


}

void Universe::screenshot(string path){
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, ("screenshots/" + path).c_str());
    SDL_FreeSurface(sshot);
}

bool Universe::screen_init(){
    bool success = true;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Set texture filtering to linear
            if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
            {
                printf("Warning: Linear texture filtering not enabled!");
            }

            //Create window
            gWindow = SDL_CreateWindow("Universe Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
            if (gWindow == NULL)
            {
                printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Create renderer for window
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL)
                {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                }
                else
                {
                    // Setup TTF
                    TTF_Init();
                    gfont = TTF_OpenFont(gfont_path, 18);
                    if (gfont == NULL) {
                        fprintf(stderr, "error: font not found\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        return success;
}