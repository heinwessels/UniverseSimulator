#include <vector> 
#include "vec2f.hpp"
#include "vec2l.hpp"
#include <stdio.h>

#ifndef BODY_HPP
#define BODY_HPP

using namespace std;

class Body{

    public:

    bool attracted;
    bool attractable;
    float mass;         // In kg
    int radius;         // In meter
    Vec2f pos;          // In meter
    Vec2f speed;        // In m / s

    Body();
    Body(float mass, Vec2f pos);
};


#endif