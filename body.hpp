#include <vector> 
#include "vec2f.hpp"
#include <stdio.h>

#ifndef BODY_HPP
#define BODY_HPP

using namespace std;

class Body{

    public:

    bool attracted;
    bool attractable;
    int mass;
    int radius;
    Vec2f pos;
    Vec2f speed;

    Body();
    Body(int mass);
};


#endif