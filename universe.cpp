#include "universe.hpp"

Universe::Universe(){
    printf("Universe Startup\n");

    state = idle;

    gRenderer = NULL;
    gWindow = NULL;

    Body sun (250, 20, Vec3<float>(screenWidth / 2, screenHeight / 2, 0));

    Body earth (10, 5, Vec3<float>(screenWidth / 2 + 250, screenHeight / 2, 0));
    earth.speed = Vec3<float>(0, 1, 0);

    Body moon (10, 5, Vec3<float>(screenWidth / 2 - 250, screenHeight / 2, 0));
    moon.speed = Vec3<float>(0, -1, 0);

    // bodies.push_back(moon);
    // bodies.push_back(sun);
    // bodies.push_back(earth);

    init_random_bodies();

    screen_init();
    screen_render();
    while(step_universe()){}
}

bool Universe::step_universe(){

    static unsigned int frame_count = 0;

    if (delay != 0)
        SDL_Delay(delay);

    if (state == running || state == single_step){
        step_through_bodies();
        check_for_collisions_and_combine();

        if (state == single_step){
            printf("IDLE\n");
            state = idle;
        }
    }

    if(!(frame_count++ % render_frame))
        screen_render();

    return handle_input();
}

void Universe::step_through_bodies(){

    int n = bodies.size();
    static Vec3<float> forces [2000];

    // Clear forces array
    for (int i = 0; i < n; i++){
        forces[i].x = 0;
        forces[i].y = 0;
    }

    // Step through the n bodies in O(n logn) to calculate the experienced gravitational forces
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){

            // Is it neccesary to calculate the force between these two bodies?
            if (!bodies.at(i).stationary || bodies.at(i).neglible
                    || !bodies.at(i).stationary || bodies.at(i).neglible){

                // Calculate the force betwe en the two bodies
                Vec3<float> force = calculate_gravity_force_between(bodies.at(i), bodies.at(j));

                // Apply the force to the body if it's not stationary (both of them)
                if (!bodies.at(i).stationary)
                    forces[i] += force;
                if (!bodies.at(j).stationary)
                    forces[j] -= force;  // Inverting it for the other body
            }
        }

        // This will exectute only when all forces on this body has been accounted for
        if (!bodies.at(i).stationary)
            bodies.at(i).apply_force(forces[i], time_step);
    }
}

void Universe::check_for_collisions_and_combine(){
    int i = 0, j = 0;
    while(i < bodies.size()){
        j = i+1;
        while(j < bodies.size()){

            Vec3<float> delta = bodies.at(j).pos - bodies.at(i).pos;
            float d2 = pow(delta.x, 2) + pow(delta.y, 2);
            if(d2 < pow(bodies.at(i).radius + bodies.at(j).radius, 2)){
                // Collision! Combine the bodies!
                // TODO Handle stationary objects correctly

                // Calculate the new location using the CoM of the two bodies
                bodies.at(i).pos = (bodies.at(i).pos * bodies.at(i).mass + bodies.at(j).pos * bodies.at(j).mass)
                                     / (bodies.at(i).mass + bodies.at(j).mass);

                // Calculate speed using law of conservation of energy
                bodies.at(i).speed = (bodies.at(i).speed * bodies.at(i).mass + bodies.at(j).speed * bodies.at(j).mass)
                                    / (bodies.at(i).mass + bodies.at(j).mass);

                // Calculate the new radius adding the area's of the two bodies
                bodies.at(i).radius = sqrt(pow(bodies.at(i).radius, 2) + pow(bodies.at(j).radius, 2));
                bodies.at(i).mass += bodies.at(j).mass;

                bodies.erase(bodies.begin() + j);
            }

            j++;
        }

        i++;
    }
}

Vec3<float> Universe::calculate_gravity_force_between(Body& this_body, Body& that_body){

    // Calculate the distance cubed
    Vec3<float> dpos = (that_body.pos - this_body.pos);
    float r3 = pow(dpos.x, 2) + pow(dpos.y, 2);
    r3 *=  sqrt(r3);

    // Calculate the absolute force divided by the distance
    float f = this_body.mass * that_body.mass / r3;

    // Return it's components
    return Vec3<float>(f * dpos.x, f * dpos.y, 0);
}

void Universe::screen_render(){

    // Clear Screen
    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
    SDL_RenderClear(gRenderer);

    // Draw the bodies
    for (Body& body : bodies){

        SDL_Rect rect;
        rect.x = body.pos.x - body.radius;
        rect.y = body.pos.y - body.radius;
        rect.w = body.radius * 2 + 1;
        rect.h = body.radius * 2 + 1;
        SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 0 );
        SDL_RenderFillRect(gRenderer, &rect);

        if (show_force_on_body && body.last_force.x != 0){
            float f = sqrt(pow(body.last_force.x, 2) + pow(body.last_force.y, 2));
            SDL_SetRenderDrawColor( gRenderer, 0, 255, 0, 0 );
            SDL_RenderDrawLine(
                gRenderer,
                body.pos.x,
                body.pos.y,
                body.pos.x + 3 * body.radius * body.last_force.x / f,
                body.pos.y + 3 * body.radius * body.last_force.y / f
            );
        }

        if (show_acc_on_body && body.last_acc.x != 0){
            float a = sqrt(pow(body.last_acc.x, 2) + pow(body.last_acc.y, 2));
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 255, 0 );
            SDL_RenderDrawLine(
                gRenderer,
                body.pos.x,
                body.pos.y,
                body.pos.x + 3 * body.radius * body.last_acc.x / a,
                body.pos.y + 3 * body.radius * body.last_acc.y / a
            );
        }

        if (show_speed_on_body){
            float s = sqrt(pow(body.speed.x, 2) + pow(body.speed.y, 2));
            SDL_SetRenderDrawColor( gRenderer, 0, 0, 255, 0 );
            SDL_RenderDrawLine(
                gRenderer,
                body.pos.x,
                body.pos.y,
                body.pos.x + 3 * body.radius * body.speed.x / s,
                body.pos.y + 3 * body.radius * body.speed.y / s
            );
        }
    }

    // Update the screen
    SDL_RenderPresent( gRenderer );
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
            if (event.key.keysym.sym == SDLK_s){
                if (state == idle){
                    printf("RUNNING\n");
                    state = running;
                }
                else if (state == running){
                    state = idle;
                    printf("IDLE\n");
                }
            }
            if (event.key.keysym.sym == SDLK_SPACE){
                state = single_step;
                printf("SINGLE STEP\n");
            }
        }
    }

    return true;
}

void Universe::init_random_bodies(){

    uint32_t seed = 8125;
    srand(seed);

    // std::cout << "---------------------------------\n";
	// std::cout << "* frequency [0.1 .. 8.0 .. 64.0] \n";
	// std::cout << "* octaves   [1 .. 8 .. 16]       \n";
	// std::cout << "* seed      [0 .. 2^32-1]        \n";
	// std::cout << "---------------------------------\n";
    // perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves)

    double size_frequency = 20;
    double size_octaves = 4;
    const siv::PerlinNoise size_perlin(seed);
    const double size_fx = screenWidth / size_frequency;
    const double size_fy = screenHeight / size_frequency;

    double speed_frequency = 20;
    double speed_octaves = 4;
    const siv::PerlinNoise speed_perlin(seed + 5);
    const double speed_fx = screenWidth / speed_frequency;
    const double speed_fy = screenHeight / speed_frequency;

    double density = 0.2;
    double minimim_radius = 2;
    double maximum_radius = 5;
    double spacing_multiplier = 7;
    double speed_multiplier = 20;

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

            bodies.push_back(body);


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
                    //Initialize renderer color
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags))
                    {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                        success = false;
                    }
                }
            }
        }

        return success;
}