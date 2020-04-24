#include "universe.hpp"

Universe::Universe(){
    printf("Universe Startup\n");

    Body moon (7.35e22f, Vec2f(0, 0));
    Body earth (6e24f, Vec2f(-3.844e8f, 0.0));

    printf("%.5f x 10^20 N\n", calculate_gravity_force_between(moon, earth).x / 1e20f);
}


Vec2f Universe::calculate_gravity_force_between(Body& this_body, Body& that_body){

    // Calculate the distance cubed
    Vec2f dpos = (this_body.pos - that_body.pos);
    float r3 = pow(dpos.x, 2) + pow(dpos.y, 2);
    r3 *=  sqrt(r3);

    // Calculate the absolute force divided by the distance
    float f = g * this_body.mass * that_body.mass / r3;

    // Return it's components
    return Vec2f(f * dpos.x, f * dpos.y);    
}