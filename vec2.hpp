#ifndef VEC2_HPP
#define VEC2_HPP


class Vec2 {

    public:

    float x, y;

    Vec2(){}
    Vec2(float i, float j) : x(i), y(j) {}

    Vec2& operator += (const Vec2 & v){
    x += v.x;
    y += v.y;
    return *this;
    }
    Vec2 operator + (const Vec2 &v){
        Vec2 t (*this);
        t += v;
        return t;
    }
    Vec2& operator -= (const Vec2 & v){
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2 operator - (const Vec2 &v){
        Vec2 t (*this);
        t -= v;
        return t;
    }

};





#endif