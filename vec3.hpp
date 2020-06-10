#ifndef VEC3_HPP
#define VEC3_HPP

template <class T>
class Vec3 {

    public:
    T x, y, z;

    Vec3(){};
    Vec3(T i, T j, T k) : x(i), y(j), z(k) {}

    Vec3& operator += (const Vec3 & v){
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3 operator + (const Vec3 &v){
        Vec3 t (*this);
        t += v;
        return t;
    }
    Vec3& operator -= (const Vec3 & v){
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vec3 operator - (const Vec3 &v){
        Vec3 t (*this);
        t -= v;
        return t;
    }
    Vec3& operator *= (const Vec3 & v){
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vec3 operator * (const Vec3 &v){
        Vec3 t (*this);
        t *= v;
        return t;
    }
    Vec3& operator /= (const Vec3 & v){
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    Vec3 operator / (const Vec3 &v){
        Vec3 t (*this);
        t /= v;
        return t;
    }
};

#endif