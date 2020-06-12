#ifndef VEC3_HPP
#define VEC3_HPP

template <class T>
class Vec3 {

    public:
    T x, y, z;

    Vec3() : x(0), y(0), z(0) {};
    Vec3(T i) : x(i), y(i), z(i) {}
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
    Vec3& operator += (const T &c){
        x += c;
        y += c;
        z += c;
        return *this;
    }
    Vec3 operator + (const T &c){
        Vec3 t (*this);
        t += c;
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
    Vec3& operator -= (const T &c){
        x -= c;
        y -= c;
        z -= c;
        return *this;
    }
    Vec3 operator - (const T &c){
        Vec3 t (*this);
        t -= c;
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
    Vec3& operator *= (const T &c){
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }
    Vec3 operator * (const T &c){
        Vec3 t (*this);
        t *= c;
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
    Vec3& operator /= (const T &c){
        x /= c;
        y /= c;
        z /= c;
        return *this;
    }
    Vec3 operator / (const T &c){
        Vec3 t (*this);
        t /= c;
        return t;
    }
};

#endif