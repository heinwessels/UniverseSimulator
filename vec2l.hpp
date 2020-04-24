#ifndef VEC2D_HPP
#define VEC2D_HPP


class Vec2l {

    public:

    long x, y;

    Vec2l(){}
    Vec2l(long i, long j) : x(i), y(j) {}

    Vec2l& operator += (const Vec2l & v){
    x += v.x;
    y += v.y;
    return *this;
    }
    Vec2l operator + (const Vec2l &v){
        Vec2l t (*this);
        t += v;
        return t;
    }
    Vec2l& operator -= (const Vec2l & v){
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2l operator - (const Vec2l &v){
        Vec2l t (*this);
        t -= v;
        return t;
    }
    Vec2l& operator *= (const Vec2l & v){
        x *= v.x;
        y *= v.y;
        return *this;
    }
    Vec2l operator * (const Vec2l &v){
        Vec2l t (*this);
        t *= v;
        return t;
    }

};

#endif