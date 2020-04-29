#include "universe.hpp"

Universe::Universe(){
    printf("Universe Startup\n");

    state = idle;

    gRenderer = NULL;
    gWindow = NULL;

    Body sun (250, 20, Vec2f(screenWidth / 2, screenHeight / 2));

    Body earth (10, 5, Vec2f(screenWidth / 2 + 250, screenHeight / 2));
    earth.speed = Vec2f(0, 1);

    Body moon (10, 5, Vec2f(screenWidth / 2 - 250, screenHeight / 2));
    moon.speed = Vec2f(0, 1);

    bodies.push_back(sun);
    bodies.push_back(earth);
    bodies.push_back(moon);

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
    static Vec2f forces [100];

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

                // Calculate the force between the two bodies
                Vec2f force = calculate_gravity_force_between(bodies.at(i), bodies.at(j));

                // Apply the force to the body if it's not stationary (both of them) 
                if (!bodies.at(i).stationary)
                    forces[i] += force;
                if (!bodies.at(j).stationary)
                    forces[j] -= force;  // Inverting it for the other body
            }
        }
    }

    // Apply the forces to all the bodies
    for (int i = 0; i < n; i++)
        if (!bodies.at(i).stationary)
            bodies.at(i).apply_force(forces[i], time_step);
}

void Universe::check_for_collisions_and_combine(){
    int i = 0, j = 0;
    while(i < bodies.size()){
        j = i+1;
        while(j < bodies.size()){
            
            Vec2f delta = bodies.at(i).pos - bodies.at(j).pos;
            float d2 = pow(delta.x, 2) + pow(delta.y, 2);
            if(d2 < pow(bodies.at(i).radius + bodies.at(j).radius, 2)){
                // Collision!

                printf("Collision!\n");

                // Combine the bodies
                Vec2f mi = Vec2f(bodies.at(i).mass, bodies.at(i).mass);
                Vec2f mj = Vec2f(bodies.at(j).mass, bodies.at(j).mass);
                bodies.at(i).radius = sqrt(pow(bodies.at(i).radius, 2) + pow(bodies.at(j).radius, 2));
                bodies.at(i).mass += bodies.at(j).mass;
                bodies.at(i).speed = (bodies.at(i).speed * mi + bodies.at(j).speed * mj) / (mi + mj);
                bodies.at(i).pos = (bodies.at(i).pos + bodies.at(j).pos) / Vec2f(2.0, 2.0);
                bodies.erase(bodies.begin() + j);
            }

            j++;
        }

        i++;
    }
}

Vec2f Universe::calculate_gravity_force_between(Body& this_body, Body& that_body){

    // Calculate the distance cubed
    Vec2f dpos = (that_body.pos - this_body.pos);
    float r3 = pow(dpos.x, 2) + pow(dpos.y, 2);
    r3 *=  sqrt(r3);

    // Calculate the absolute force divided by the distance
    float f = this_body.mass * that_body.mass / r3;

    // Return it's components
    return Vec2f(f * dpos.x, f * dpos.y);    
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