#include <vector> 
#include "vec2f.hpp"
#include "vec2l.hpp"
#include <stdio.h>
#include <cmath>

#ifndef BODY_HPP
#define BODY_HPP

using namespace std;

class Body{

    public:

    bool neglible;
    bool stationary;
    float mass;         // In kg
    int radius;         // In meter
    Vec2f pos;          // In meter
    Vec2f speed;        // In m / s

    Vec2f last_force;
    Vec2f last_acc;

    Body();
    Body(float mass, float radius, Vec2f pos);

    void apply_force(Vec2f force, float time_step);
};


#endif