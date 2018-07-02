#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "Ball.h"
#include "Box.h"
#include "Terrain.h"
#include<math.h>
#include<vector>
#include<mutex>
#define Cr 1
mutex m;
class Physics{
public:
    void wallCollision(BounceBall& ball,Box& box){
        if(ball.centre.x + ball.radius > box.boxWidth){
            ball.centre.x = box.boxWidth - ball.radius;
            ball.velocity.x *= -1.0;
        }
        if(ball.centre.x - ball.radius < -box.boxWidth){
            ball.centre.x = -box.boxWidth + ball.radius;
            ball.velocity.x *= -1.0;
        }
        if(ball.centre.y + ball.radius > box.boxHeight){
            ball.centre.y = box.boxHeight - ball.radius;
            ball.velocity.y *= -1.0;
        }
        if(ball.centre.y - ball.radius < -box.boxHeight){
            ball.centre.y = -box.boxHeight + ball.radius;
            ball.velocity.y *= -1.0;
        }
        if(ball.centre.z + ball.radius > box.boxDepth){
            ball.centre.z = box.boxDepth - ball.radius;
            ball.velocity.z *= -1.0;
        }
        if(ball.centre.z - ball.radius < -box.boxDepth){
            ball.centre.z = -box.boxDepth + ball.radius;
            ball.velocity.z *= -1.0;
        }
    }

    void updateBalls(BounceBall& ball1, BounceBall& ball2){
        Vector<float> t(ball2.centre);
        t.subtract(ball1.centre);
        Vector<float> velocity1(ball1.velocity);
        Vector<float> velocity2(ball2.velocity);
        t.normalize();
        Vector<float> t1(t),t2(t),t3(t),t4(t);
        float u1 = t.dot(velocity1);
        float u2 = t.dot(velocity2);
        float mass1 = ball1.mass;
        float mass2 = ball2.mass;
        float v1 =( Cr*mass2*(u2-u1) + mass1*u1 + mass2*u2)/(mass1 + mass2);
        float v2 =( Cr*mass1*(u1 - u2) + mass2*u2 + mass1*u1)/(mass1 + mass2);
        t.scale(u1);
        velocity1.subtract(t);
        t1.scale(v1);
        velocity1.add(t1);
        t2.scale(u2);
        velocity2.subtract(t2);
        t3.scale(v2);
        velocity2.add(t3);
        velocity1.comp(ball1.velocity);
        velocity2.comp(ball2.velocity);
        Vector<float> fpos(ball1.centre);
        t4.scale(ball1.radius + ball2.radius + 0.000001);
        fpos.add(t4);
        fpos.comp(ball2.centre);
    }


    void updateobst(BounceBall& ball, Terrain& ter){
        Vector<float> t(ter.centre);
        t.subtract(ball.centre);
        Vector<float> velocity1(ball.velocity);
        Vector<float> velocity2(ter.velocity);
        t.normalize();
        Vector<float> t1(t),t2(t),t3(t),t4(t);
        float u1 = t.dot(velocity1);
        float u2 = 0;//t.dot(velocity2);
        float mass1 = ball.mass;
        float mass2 = ter.mass;
        float v1 =( Cr*mass2*(u2-u1) + mass1*u1 + mass2*u2)/(mass1 + mass2);
        float v2 = 0;//( Cr*mass1*(u1 - u2) + mass2*u2 + mass1*u1)/(mass1 + mass2);
        t.scale(u1);
        velocity1.subtract(t);
        t1.scale(v1);
        velocity1.add(t1);
        t2.scale(u2);
        //velocity2.subtract(t2);
        t3.scale(v2);
        //velocity2.add(t3);
        velocity1.comp(ball.velocity);
        //velocity2.comp(ter.velocity);
        Vector<float> fpos(ball.centre);
        t4.scale(ball.radius + ter.radius + 0.000001);
        fpos.add(t4);
        //fpos.comp(ter.centre);
    }

    void ballCollisionDetection(vector<BounceBall>& balls){
        int numb=balls.size();
        bool collisionCheck[numb][numb];
        for(int i=0;i<numb;i++){
           
            for(int j=i+1;j<numb;j++){
                    collisionCheck[i][j] = false;
                }
            }
        
        for(int i=0;i<numb;i++){
           
            for(int j=i+1;j<numb;j++){
                float x = balls[i].centre.x - balls[j].centre.x;
                float y = balls[i].centre.y - balls[j].centre.y;
                float z = balls[i].centre.z - balls[j].centre.z;
                float r = balls[i].radius + balls[j].radius;
                float d = sqrt(x*x + y*y + z*z);
                if(d<=r){
                    collisionCheck[i][j] = true;
                }
                else{
                    continue;
                }
            }
          

        }
    
        for(int i=0;i<numb;i++){
          
            for(int j=i+1;j<numb;j++){
                if(collisionCheck[i][j]){
                    collisionCheck[i][j] = false;
                    updateBalls(balls[i],balls[j]);
                }
                else{
                    continue;
                }
            }
           
        }
    
    }

    
    void TerrainCollisionDetection(vector<BounceBall>& balls,vector<Terrain>& obst){
        int numb=balls.size();
        int numt=obst.size();
        //int num = numb + numt;
        bool collisionCheck[numb][numt];
        for(int i=0;i<numb;i++){       
            for(int j=0;j<numt;j++){
                collisionCheck[i][j] = false;
            }
        }
        for(int i=0;i<numb;i++){
           
            for(int j=0;j<numt;j++){
                float x = balls[i].centre.x - obst[j].centre.x;
                float y = balls[i].centre.y - obst[j].centre.y;
                float z = balls[i].centre.z - obst[j].centre.z;
                float r = balls[i].radius + obst[j].radius;
                float d = sqrt(x*x + y*y + z*z);
                if(d<=r){
                    collisionCheck[i][j] = true;
                }
                else{
                    continue;
                }
            }
          
        }
        for(int i=0;i<numb;i++){
          
            for(int j=0;j<numt;j++){
                if(collisionCheck[i][j]){
                    collisionCheck[i][j] = false;
                    updateobst(balls[i],obst[j]);
                }
                else{
                    continue;
                }
            }
           
        }
    }


             
};

#endif // PHYSICS_H_INCLUDED