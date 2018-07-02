#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include"VectorOp.h"
#include<random>
using namespace std;

typedef uniform_real_distribution<> urd;

class Terrain{
public:
    Vector<float> centre;
    Vector<float> velocity;
    float radius;
    float red;
    float green;
    float blue;
    float mass;

    Terrain(){
        radius=red=green=blue=0.0;
        mass=100000.0;
    }

    Terrain(const Terrain& a){
        centre = a.centre;
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
        radius = a.radius;
        red = a.red;
        green = a.green;
        blue = a.blue;
        mass = 10000000000000000.0;
    }

    void appearTerrainx(urd& yRan,urd& zRan,urd& radRan,urd& colorRan,mt19937& ran,float cr){
        centre.x = cr*1.0f;
        centre.y = yRan(ran);
        centre.z = zRan(ran);
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
        radius = radRan(ran);
        red = colorRan(ran);
        green = colorRan(ran);
        blue = colorRan(ran);
        mass = 10000000000000000.0;
    }

    void appearTerrainy(urd& xRan, urd& zRan,urd& radRan,urd& colorRan,mt19937& ran,float cr){
        centre.x = xRan(ran);
        centre.y = cr*1.0f;
        centre.z = zRan(ran);
        velocity.x = 0;
        velocity.y = 0;
        velocity.z = 0;
        radius = radRan(ran);
        red = colorRan(ran);
        green = colorRan(ran);
        blue = colorRan(ran);
        mass = 10000000000000000.0;
    }

};

#endif // TERRAIN_H_INCLUDED