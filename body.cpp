#include "body.hpp"

Body::Body(){}

Body::Body(float mass, float radius, Vec3<float> pos) : mass(mass), radius(radius), pos(pos) {
    speed = Vec3<float>();
    last_force = Vec3<float>();
    last_acc = Vec3<float>();
    neglible = false;
    stationary = false;
}

void Body::apply_force(Vec3<float> force, float time_step){

    Vec3<float> acceleration = force / mass;

    last_acc = acceleration;    // For debugging
    last_force = force;         // For debugging

    speed += acceleration * time_step;   // The next formule requires the final speed
    pos += speed * time_step - (acceleration * 0.5 * time_step * time_step);
}