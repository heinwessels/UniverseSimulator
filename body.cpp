#include "body.hpp"

Body::Body(){}

Body::Body(float mass, float radius, Vec2f pos) : mass(mass), radius(radius), pos(pos) {
    speed = Vec2f(0, 0);
    last_force = Vec2f(0, 0);
    last_acc = Vec2f(0, 0);
    neglible = false;
    stationary = false;
}

void Body::apply_force(Vec2f force, float time_step){
    last_force = force;         // For debugging
    Vec2f time (time_step, time_step);
    Vec2f acceleration = force / Vec2f(mass, mass);
    last_acc = acceleration;    // For debugging
    speed += acceleration * time;
    pos += speed * time - Vec2f(0.5, 0.5) * acceleration * time * time;
}