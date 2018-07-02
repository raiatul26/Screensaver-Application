#ifndef VECTOROP_H_INCLUDED
#define VECTOROP_H_INCLUDED

#include<math.h>

template<typename T>

class Vector {
public:
    T x;
    T y;
    T z;

    Vector(){
        x=y=z=0;
    }

    Vector(T value){
        x=y=z=value;
    }

    Vector(T a, T b, T c){
        x = a;
        y = b;
        z = c;
    }

    Vector(Vector* a){
        this->x = a->x;
        this->y = a->y;
        this->z = a->z;
    }

    void normalize(){
        T mag = sqrt(x*x + y*y + z*z);
        if(mag == 0){
            return;
        }
        x /= mag;
        y /= mag;
        z /= mag;
    }

    void scale(T a){
        x*=a;
        y*=a;
        z*=a;
    }

    void add(const Vector& a){
        x += a.x;
        y += a.y;
        z += a.z;
    }

    void subtract(const Vector& a){
        x -= a.x;
        y -= a.y;
        z -= a.z;
    }

    T dot(const Vector& a){
        return (x * a.x + y * a.y + z * a.z);
    }

    T mod(){
        return sqrt(x * x + y * y + z * z);
    }


    void comp(T& a, T& b, T& c){
        a = x;
        b = y;
        c = z;
    }

    void comp(Vector& a){
        a.x = x;
        a.y = y;
        a.z = z;
    }
};

#endif // VECTOROP_H_INCLUDED