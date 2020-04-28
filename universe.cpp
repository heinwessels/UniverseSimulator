#include "universe.hpp"

Universe::Universe(){
    printf("Universe Startup\n");

    gRenderer = NULL;
    gWindow = NULL;

    Body sun (250, 50, Vec2f(screenWidth / 2, screenHeight / 2));
    sun.stationary = true;

    Body earth (1, 5, Vec2f(screenWidth / 2 + 250, screenHeight / 2));
    earth.speed = Vec2f(0, 1);

    bodies.push_back(earth);
    bodies.push_back(sun);

    screen_init();
    screen_render();
    while(step_universe()){}
}

bool Universe::step_universe(){

    SDL_Delay(1);

    step_through_bodies();
    return screen_render();
}

void Universe::step_through_bodies(){

    int n = bodies.size();
    static Vec2f forces [100];

    // Step through the n bodies in O(n logn) to calculate the experienced gravitational forces
    forces[0] = Vec2f();
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            
            // Clear the array
            if (!i){
                forces[j] = Vec2f();
            }

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


bool Universe::screen_render(){

    // Clear Screen        
    SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 );
    SDL_RenderClear(gRenderer);

    // Draw the bodies
    SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 0 );
    for (Body& body : bodies){
        /*SDL_RenderDrawPoint(gRenderer, 
                body.pos.x / screenScale + screenWidth / 2.0,
                body.pos.y / screenScale + screenHeight / 2.0
                );*/
        
        SDL_Rect rect;
        rect.x = body.pos.x - body.radius;
        rect.y = body.pos.y - body.radius;
        rect.w = body.radius * 2 + 1;
        rect.h = body.radius * 2 + 1;
        SDL_RenderFillRect(gRenderer, &rect);
    }   

    // Update the screen
    SDL_RenderPresent( gRenderer );

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