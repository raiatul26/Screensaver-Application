#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include"VectorOp.h"
#include<random>
using namespace std;

typedef uniform_real_distribution<> urd;
struct RequestWrapper {
   urd xRan;
   urd yRan;
   urd zRan;
   urd radRan;
   urd velRanX;
   urd velRanY;
   urd velRanZ;
   urd colorRan;
   mt19937 ran;
};

class BounceBall{
public:
    Vector<float> centre;
    Vector<float> velocity;
    double radius;
    float red;
    float green;
    float blue;
    float mass;

    BounceBall(){
        radius=red=green=blue=mass=0.0;
    }

    BounceBall(const BounceBall& a){
        centre = a.centre;
        velocity = a.velocity;
        radius = a.radius;
        red = a.red;
        green = a.green;
        blue = a.blue;
        mass = a.mass;
    }

    void *appearBall(urd& xRan,urd& yRan,urd& zRan,urd& radRan,urd& velRanX,urd& velRanY,urd& velRanZ,urd& colorRan,mt19937& ran){
        centre.x = xRan(ran);
        centre.y = yRan(ran);
        centre.z = zRan(ran);
        radius = radRan(ran);
        velocity.x = velRanX(ran);
        velocity.y = velRanY(ran);
        velocity.z = velRanZ(ran);
        red = colorRan(ran);
        green = colorRan(ran);
        blue = colorRan(ran);
        mass = radius * radius * radius;
    }


};

#endif // BALL_H_INCLUDED