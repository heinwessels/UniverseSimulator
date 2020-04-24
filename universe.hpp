#include <vector> 
#include "vec2.hpp"

#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

using namespace std;

class Universe {

    class Body{
        bool attracted;
        bool attractable;
        int mass;
        int radius;
        float pos;
        float speed;
    };

    vector<Body> bodies;
};





#endif