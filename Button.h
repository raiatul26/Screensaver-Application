#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <vector>
#include<iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
using namespace std;
typedef void (*ButtonCallback)();
class Button {

public:
    int xPos;
    int yPos;
    int butWidth;
    int butHeight;
    char butText[100];
    bool isPressed;
    //void appearButton(int x, int y, int w, int h, char *s);
    //void appearButton(int x, int y, int w, int h, char *s,vector<float>&,vector<float>&);
    vector<float> notPressedColor;
    vector<float> PressedColor;
    //void setText(char *s);
    ButtonCallback callbackFunction;

    void appearButton(int x, int y, int w, int h, char *s){

        xPos = x;
        yPos = y;
        butWidth = w;
        butHeight = h;
        int i;

        for(i = 0; s[i]!= '\0'; i++){
            butText[i] = s[i];
        }
        butText[i] = '\0';

        isPressed = false;
        notPressedColor.resize(3);
        PressedColor.resize(3);

        notPressedColor[0]=1.0;
        notPressedColor[1]=0.698;
        notPressedColor[2]=0.4;

        PressedColor[0]=1.0;
        PressedColor[1]=0.6;
        PressedColor[2]=0.6;

    }

    void appearButton(int x, int y, int w, int h, char *s,vector<float> &bc,vector<float>& fc)
    {

        xPos = x;
        yPos = y;
        butWidth = w;
        butHeight = h;
        int i;
        for(i = 0; s[i]!= '\0'; i++){
            butText[i] = s[i];

        }
        butText[i] = '\0';
        isPressed = false;
        notPressedColor.resize(3);
        notPressedColor[0] = bc[0];
        notPressedColor[1]=bc[1];
        notPressedColor[2]=bc[2];

        PressedColor.resize(3);
        PressedColor[0]=fc[0];
        PressedColor[1]=fc[1];
        PressedColor[2]=fc[2];

    }

  
  void setText(char *s){

        int i;

        for(i = 0; s[i]!= '\0'; i++){
            butText[i] = s[i];
        }
        butText[i] = '\0';
    }


};

#endif // BUTTON_H_INCLUDED