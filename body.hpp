#include <vector>
#include "vec3.hpp"
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
    float radius;         // In meter
    Vec3<float> pos;          // In meter
    Vec3<float> speed;        // In m / s

    Vec3<float> last_force;
    Vec3<float> last_acc;

    Body();
    Body(float mass, float radius, Vec3<float> pos);

    void apply_force(Vec3<float> force, float time_step);
};


#endif