#include "universe.hpp"

Universe::Universe(){
    printf("Universe Startup\n");

    Body moon (7.35e22f, Vec2f(0, 0));
    Body earth (6e24f, Vec2f(-3.844e8f, 0.0));

    printf("%.5f x 10^20 N\n", calculate_gravity_force_between(moon, earth).x / 1e20f);
}


void Universe::step_through_bodies(){

    int n = bodies.size();
    static Vec2f forces [100];

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
                    forces[j] += Vec2f(-force.x,-force.y);  // Inverting it for the other body
            }
        }
    }

    // Apply the forces to all the bodies
    for (int i = 0; i < n; i++)
        bodies.at(i).apply_force(forces[i], time_step);
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