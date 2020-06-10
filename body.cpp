#include "body.hpp"

Body::Body(){}

Body::Body(float mass, float radius, Vec3<float> pos) : mass(mass), radius(radius), pos(pos) {
    speed = Vec3<float>(0, 0, 0);
    last_force = Vec3<float>(0, 0, 0);
    last_acc = Vec3<float>(0, 0, 0);
    neglible = false;
    stationary = false;
}

void Body::apply_force(Vec3<float> force, float time_step){
    last_force = force;         // For debugging
    Vec3<float> time (time_step, time_step, 0);
    Vec3<float> acceleration = force / Vec3<float>(mass, mass, 0);
    last_acc = acceleration;    // For debugging
    speed += acceleration * time;
    pos += speed * time - Vec3<float>(0.5, 0.5, 0) * acceleration * time * time;
}