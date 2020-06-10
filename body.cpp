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
    last_force = force;         // For debugging
    Vec3<float> time (time_step);
    Vec3<float> acceleration = force / Vec3<float>(mass);
    last_acc = acceleration;    // For debugging
    speed += acceleration * time;
    pos += speed * time - Vec3<float>(0.5) * acceleration * time * time;
}