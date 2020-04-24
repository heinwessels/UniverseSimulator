#include <vector> 
#include "vec2f.hpp"
#include "body.hpp"
#include <stdio.h>
#include <cmath>

#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

using namespace std;

class Universe {

public:

    float time_step = 60*60*24; 

    const float g = 6.67408e-11f; // In m^3 kg^(-1) s^(-2)

    vector<Body> bodies;

    Universe();

    void step_through_bodies();
    Vec2f calculate_gravity_force_between(Body& this_body, Body& that_body);
};





#endif