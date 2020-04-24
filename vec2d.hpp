#ifndef VEC2D_HPP
#define VEC2D_HPP


class Vec2d {

    public:

    long x, y;

    Vec2d(){}
    Vec2d(long i, long j) : x(i), y(j) {}

    Vec2d& operator += (const Vec2d & v){
    x += v.x;
    y += v.y;
    return *this;
    }
    Vec2d operator + (const Vec2d &v){
        Vec2d t (*this);
        t += v;
        return t;
    }
    Vec2d& operator -= (const Vec2d & v){
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2d operator - (const Vec2d &v){
        Vec2d t (*this);
        t -= v;
        return t;
    }
    Vec2d& operator *= (const Vec2d & v){
        x *= v.x;
        y *= v.y;
        return *this;
    }
    Vec2d operator * (const Vec2d &v){
        Vec2d t (*this);
        t *= v;
        return t;
    }

};

#endif