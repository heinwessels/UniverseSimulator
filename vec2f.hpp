#ifndef VEC2_HPP
#define VEC2_HPP


class Vec2f {

    public:

    float x, y;

    Vec2f(){}
    Vec2f(float i, float j) : x(i), y(j) {}

    Vec2f& operator += (const Vec2f & v){
    x += v.x;
    y += v.y;
    return *this;
    }
    Vec2f operator + (const Vec2f &v){
        Vec2f t (*this);
        t += v;
        return t;
    }
    Vec2f& operator -= (const Vec2f & v){
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2f operator - (const Vec2f &v){
        Vec2f t (*this);
        t -= v;
        return t;
    }

};





#endif