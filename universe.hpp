#include <vector> 
#include "vec2f.hpp"
#include "body.hpp"
#include <stdio.h>

#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

using namespace std;

class Universe {

private:
    const float g = 6.67408e-11; 

public:
    Universe();


    // Vec2f calculate_gravity_between(Body& this_body, Body& that_body);

};





#endif