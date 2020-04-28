#include "body.hpp"

Body::Body(){}

Body::Body(float mass, float radius, Vec2f pos) : mass(mass), radius(radius), pos(pos) {}



void Body::apply_force(Vec2f force, float time_step){

    Vec2f time (time_step, time_step);

    Vec2f acceleration = force / Vec2f(mass, mass);
    speed += acceleration * time;
    pos += speed * time - Vec2f(0.5, 0.5) * acceleration * time * time;
}