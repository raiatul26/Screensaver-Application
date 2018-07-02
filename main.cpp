/*----------------------------------------------------------------------------------------
 *
 *  project:    N ball Screen saver
 *  author:     Dwij Upadhyay & Atul Rai
 *  note:
 *
/*----------------------------------------------------------------------------------------
 *  Includes
 */


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <utility>
#include<string>
#include<cstring>
#include<mutex>
// #ifdef WIN32
//  #define WIN32_LEAN_AND_MEAN
//  #include <Windows.h>
// #endif

#include <GL/glut.h>
#include "Box.h"
#include "Ball.h"
#include "Button.h"
#include "VectorOp.h"
#include "Physics.h"
#include "Terrain.h"

#define pi 3.141
int winw = 500;
int winh = 500;
#define BOUNDS    1



#define WATERSIZE 600
#define DAMP 20

float water[2][WATERSIZE][WATERSIZE];
int spin_x, spin_y, spin_z; /* x-y rotation and zoom */
int old_x, old_y, move_z;
int depth = 3;
int i = 0;

int t = 0, f = 1;
/*----------------------------------------------------------------------------------------
 *  Global Variables
 */
Box box;
vector<BounceBall> balls;
vector<Terrain> obst;
Physics physics;
Button increaseSpeed, decreaseSpeed, pauseButton;
float prevRed;
float prevGreen;
float prevBlue;
int numb;
GLuint *texture = new GLuint[2];

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
GLfloat xRotated, yRotated, zRotated;
static int slices = 50;
static int stacks = 50;
static int lon = 50;
static int lat = 50;
float maxVel = 20.0;
int curBall ;
vector<Vector<float>> old_speed;
bool funcPause = false;
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-0.15f;
// XZ position of the camera
float x=0.0f,z=-0.4f;
/*
 *  A structure to represent the mouse information
 */
void calcwater() {
    int x, y;
    float n;
    for(y = 1; y < WATERSIZE-1; y++) {
        for(x = 1; x < WATERSIZE-1; x++) {
            n = ( water[t][x-1][y] +
                  water[t][x+1][y] + 
                  water[t][x][y-1] + 
                  water[t][x][y+1]
                  ) /2;
            n -= water[f][x][y];
            n = n - (n / DAMP);
            water[f][x][y] = n;
        }
    }

    y = 0;
    for(x = 1; x < WATERSIZE-1; x++) {
            n = ( water[t][x-1][y] +
                  water[t][x+1][y] + 
                  water[t][x][y+1]
                  ) /2;
            n -= water[f][x][y];
            n = n - (n / DAMP);
            water[f][x][y] = n;
    }
    
    
    x = 0;
    for(y = 1; y < WATERSIZE-1; y++) {
            n = ( water[t][x+1][y] + 
                  water[t][x][y-1] + 
                  water[t][x][y+1]
                  ) /2;
            n -= water[f][x][y];
            n = n - (n / DAMP);
            water[f][x][y] = n;
    }

    x = WATERSIZE-1;
    for(y = 1; y < WATERSIZE-1; y++) {
            n = ( water[t][x-1][y] +
                  water[t][x][y-1] + 
                  water[t][x][y+1]
                  ) /2;
            n -= water[f][x][y];
            n = n - (n / DAMP);
            water[f][x][y] = n;
    }
    y = WATERSIZE-1;
    for(x = 1; x < WATERSIZE-1; x++) {
            n = ( water[t][x-1][y] +
                  water[t][x+1][y] + 
                  water[t][x][y-1] 
                  ) /2;
            n -= water[f][x][y];
            n = n - (n / DAMP);
            water[f][x][y] = n;
    }

}
class GUI {
  public:
    struct Image {
            unsigned long sizeX;
            unsigned long sizeY;
            char *data;
};
typedef struct Image Image;
 Image image[2];


int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
// temporary color storage for bgr-rgb conversion.

    file =fopen(filename, "rb");
    fseek(file, 18, SEEK_CUR);
    fread(&image->sizeX, 4, 1, file);
    fread(&image->sizeY, 4, 1, file);
    size = image->sizeX * image->sizeY * 3;
    fread(&planes, 2, 1, file);
    fread(&bpp, 2, 1, file);
    fseek(file, 24, SEEK_CUR);
    image->data = (char *) malloc(size);
    fread(image->data, size, 1, file);
    return 1;
}

// Bitmap image[3];
// image[0].loadBMP("floor8.bmp");
// image[1].loadBMP("floor4.bmp");
// image[2].loadBMP("floor2.bmp");
// int q=0;

Image * loadTexture1(){
    Image *image1;
    image1 = (Image *) malloc(sizeof(Image));
    ImageLoad("floor8.bmp", image1);
    return image1;
}
Image * loadTexture2(){
    Image *image2;
    image2 = (Image *) malloc(sizeof(Image));
    ImageLoad("floor8.bmp", image2);
    return image2;
}
// void loadTexture(GLuint texture, const char* filename)
// {
//    Bitmap image; 
//    image.loadBMP(filename); 

//    glBindTexture(GL_TEXTURE_2D, texture);

//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE , GL_MODULATE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image.data);
// }
void initializeImage(){

    glClearColor (0.5, 0.5, 0.5, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
   
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     Image *image2 = loadTexture2();
          // Image *image2 = loadTexture2();
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image2->data);
  

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //   // Image *image2 = loadTexture2();
    // glGenTextures(2, texture);
    // glBindTexture(GL_TEXTURE_2D, texture[1]);
    // // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    // // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    // glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,
    // GL_RGB, GL_UNSIGNED_BYTE, image2->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // // glEnable(GL_TEXTURE_2D);
     // glShadeModel(GL_FLAT);
    //  glClearColor (0.5, 0.5, 0.5, 0.0);
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     // Image *image1= loadTexture1();
    //       Image *image1 = loadTexture1();
    // // glGenTextures(1, texture);

    // glBindTexture(GL_TEXTURE_2D, texture[1]);
    // // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    // // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    // glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
    // GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    //   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // glBindTexture(GL_TEXTURE_2D, texture[1]);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
     glShadeModel(GL_FLAT);



}

    void initializeButton(){
        char s[100] = "Nothing";
        vector<float> notPressedColor(3);
        vector<float> PressedColor(3);
        notPressedColor[0]=0.2f;
        notPressedColor[1]=0.6f;
        notPressedColor[2]=1.0f;
        PressedColor[0]=0.8f;
        PressedColor[1]=0.6f;
        PressedColor[2]=1.0f;
        strcpy(s,"+");
        increaseSpeed.appearButton(150,0,50,25, s,PressedColor,notPressedColor);
        strcpy(s,"-");
        decreaseSpeed.appearButton(0,0,50,25, s,PressedColor,notPressedColor);
        strcpy(s,"Pause");
        pauseButton.appearButton(60,0,80,30,s);
    }


    void showBall(BounceBall &ball){
        // const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
        // const double a = t*90.0;
        glColor3f(ball.red, ball.green, ball.blue);
        
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        
        glLoadIdentity();
        //if(z <= 0.0f && z>-5.0f){
         gluLookAt(  x, 0.0f, z,
            x+lx, 0.0f,  z+lz,
            0.0f, 1.0f,  0.0f);
        // }

                glTranslatef(ball.centre.x,ball.centre.y,ball.centre.z-3.5);
                glRotatef(xRotated,1.0,0.0,0.0);
    // rotation about Y axis
                glRotatef(yRotated,0.0,1.0,0.0);
    // rotation about Z axis
                glRotatef(zRotated,0.0,0.0,1.0);
                glScalef(1.0,1.0,1.0);
                glutSolidSphere(ball.radius,slices,stacks);



    }

    void showTerrain(Terrain &t){
        glColor3f(t.red, t.green, t.blue);
        
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        
        glLoadIdentity();
            glLoadIdentity();
        //if(z <= 0.0f && z>-5.0){
         gluLookAt(  x, 0.0f, z,
            x+lx, 0.0f,  z+lz,
            0.0f, 1.0f,  0.0f);
        // }
        glTranslatef(t.centre.x,t.centre.y,t.centre.z-3.5);
        glutSolidSphere(t.radius,lon,lat);

    }

    // void ballclickchecker(Mouse &mouse,vector<BounceBall> &balls);
    // {
    //      bool ballselect[5] = {false};
    //     if(mouse.lmb == 1)
    //      for (int i = 0; i < 5; i++)
    //     {
    //     if()

    //     }


    // }
  void Font(void *font,char *text,int x,int y)
    {
    glRasterPos2i(x, y);

    while( *text != '\0' )
    {
        glutBitmapCharacter( font, *text );
        ++text;
    }
}

    void showButton(Button *b){
        // clear the drawing buffer.

    if(b)
    {
         
        glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        // glOrtho(0,box.boxWidth,box.boxWidth,0,-1,1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        int fontx;
        int fonty;
     if(!b->isPressed){
                glColor3f(b->notPressedColor[0], b->notPressedColor[1], b->notPressedColor[2]);
             }
            else{
                glColor3f(b->PressedColor[0], b->PressedColor[1], b->PressedColor[2]);
            }

            glBegin(GL_QUADS);
            glVertex2i( b->xPos     , b->yPos );
            glVertex2i( b->xPos     , b->yPos+b->butHeight );
            glVertex2i( b->xPos+b->butWidth, b->yPos+b->butHeight );
            glVertex2i( b->xPos+b->butWidth, b->yPos );
        glEnd();

        /*
         *  Draw an outline around the button with width 3
         */
        glLineWidth(3);
 if(!b->isPressed){
                glColor3f(b->notPressedColor[0] + 0.2f, b->notPressedColor[1]+0.2f, b->notPressedColor[2]+0.2f);
             }
            else{
                glColor3f(b->PressedColor[0] + 0.2f, b->PressedColor[1] + 0.2f, b->PressedColor[2] + 0.2f);
            }
  

        glBegin(GL_LINE_STRIP);
            glVertex2i( b->xPos+b->butWidth, b->yPos );
            glVertex2i( b->xPos     , b->yPos      );
            glVertex2i( b->xPos     , b->yPos+b->butHeight );
        glEnd();
 if(!b->isPressed){
                glColor3f(b->notPressedColor[0] - 0.2f, b->notPressedColor[1] - 0.2f, b->notPressedColor[2] -0.2f);
             }
            else{
                glColor3f(b->PressedColor[0] - 0.2f, b->PressedColor[1] - 0.2f, b->PressedColor[2] - 0.2f);
            }

        glBegin(GL_LINE_STRIP);
            glVertex2i( b->xPos, b->yPos+b->butHeight );
            glVertex2i( b->xPos+b->butWidth, b->yPos+b->butHeight );
            glVertex2i( b->xPos+b->butWidth, b->yPos      );
        glEnd();

        glLineWidth(1);


        /*
         *  Calculate the x and y coords for the text string in order to center it.
         */
        fontx = b->xPos + (b->butWidth - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,reinterpret_cast<unsigned char*>(b->butText))) / 2 ;
        fonty = b->yPos + (b->butHeight+10)/2;


        glColor3f(1,1,1);
        this->Font(GLUT_BITMAP_HELVETICA_10,b->butText,fontx,fonty);
    }
    }



    void showBox(Box &box){
        // angle of rotation for the camera direction


        glMatrixMode(GL_MODELVIEW);
    // clear the drawing buffer.

        glLoadIdentity();
        //if(z <= 0.0f && z>-5.0f){
         gluLookAt(  x, 0.0f, z,
            x+lx, 0.0f,  z+lz,
            0.0f, 1.0f,  0.0f);
        // }

        glTranslatef(0.0,0.0,-3.5);
        //glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        // glColor3f(0.0, 1.0,1.0);
        //Back face
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-box.boxWidth, -box.boxHeight, -box.boxDepth);
          glTexCoord2f(10.0, 0.0);
        glVertex3f(-box.boxWidth, box.boxHeight, -box.boxDepth);
         glTexCoord2f(10.0, 10.0);
        glVertex3f(box.boxWidth, box.boxHeight, -box.boxDepth);
            glTexCoord2f(0.0, 10.0);
        glVertex3f(box.boxWidth, -box.boxHeight, -box.boxDepth);
        glEnd();

        // //Front face
        // glBegin(GL_LINE_LOOP);
        // glVertex3f(-box.boxWidth, -box.boxHeight, box.boxDepth);
        // glVertex3f(-box.boxWidth, box.boxHeight, box.boxDepth);
        // glVertex3f(box.boxWidth, box.boxHeight, box.boxDepth);
        // glVertex3f(box.boxWidth, -box.boxHeight, box.boxDepth);
        //     //glVertex3f(-box.boxWidth, -box.boxHeight, box.boxDepth);
        // glEnd();

        //Left face
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
        glVertex3f(-box.boxWidth, -box.boxHeight, -box.boxDepth);
          glTexCoord2f(30.0, 0.0);
        glVertex3f(-box.boxWidth, -box.boxHeight, box.boxDepth);
         glTexCoord2f(30.0, 3.0);
        glVertex3f(-box.boxWidth, box.boxHeight, box.boxDepth);
            glTexCoord2f(0.0, 3.0);
        glVertex3f(-box.boxWidth, box.boxHeight, -box.boxDepth);
        glEnd();

        //Right face
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);       
             glVertex3f(box.boxWidth, -box.boxHeight, -box.boxDepth);
               glTexCoord2f(30.0, 0.0);
        glVertex3f(box.boxWidth, -box.boxHeight, box.boxDepth);
         glTexCoord2f(30.0, 3.0);
        glVertex3f(box.boxWidth, box.boxHeight, box.boxDepth);
            glTexCoord2f(0.0, 3.0);
        glVertex3f(box.boxWidth, box.boxHeight, -box.boxDepth);
        glEnd();

        //Top face
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
        glVertex3f(-box.boxWidth, box.boxHeight, -box.boxDepth);
          glTexCoord2f(6.0, 0.0);
        glVertex3f(box.boxWidth, box.boxHeight, -box.boxDepth);
         glTexCoord2f(6.0, 30.0);
        glVertex3f(box.boxWidth, box.boxHeight, box.boxDepth);
            glTexCoord2f(0.0, 30.0);
        glVertex3f(-box.boxWidth, box.boxHeight, box.boxDepth);
        glEnd();
         //Back face
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
        glVertex3f(-box.boxWidth, box.boxHeight, box.boxDepth);
          glTexCoord2f(6.0, 0.0);
        glVertex3f(box.boxWidth, box.boxHeight, box.boxDepth);
         glTexCoord2f(6.0, 30.0);
        glVertex3f(box.boxWidth, -box.boxHeight, box.boxDepth);
            glTexCoord2f(0.0, 30.0);
        glVertex3f(-box.boxWidth, -box.boxHeight,box.boxDepth);
        glEnd();
        glDisable(GL_TEXTURE_2D);
         int  tmp;
glPushMatrix();
    glTranslatef(0, -1, 0.5);
    glRotatef(0, 0, 1, 0);
    glRotatef(180, 1, 0, 0);

    
    calcwater();
    glBegin(GL_POINTS);
    for(int i = 0; i < 600; i++) {
        for(int j = 0; j < 600; j++) {
                glColor3f(0,0,1);
            glVertex3f((float)(j-600/2)/100,water[t][j][i],(float)(i-600/2)/100 );
        }
    }
    glEnd();

    tmp = t; t = f; f = tmp;


    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
      
        glColor3f(1.0,1.0,5.0);
        //glColor3f(0.6,0.76,5.0);
         glBindTexture(GL_TEXTURE_2D, texture[1]);
         glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glBegin(GL_QUADS);

       //glColor3f(1.0,1.0,5.0);
            glTexCoord2f(0.0, 0.0);
        glVertex3f(-box.boxWidth, -box.boxHeight, -box.boxDepth);
          glTexCoord2f(10.0, 0.0);
        glVertex3f(box.boxWidth, -box.boxHeight, -box.boxDepth);
         glTexCoord2f(10.0, 10.0);
        glVertex3f(box.boxWidth, -box.boxHeight, box.boxDepth);
            glTexCoord2f(0.0, 10.0);
        glVertex3f(-box.boxWidth, -box.boxHeight, box.boxDepth);
        glEnd();

    }

    void randInitBalls()
    {
        balls.reserve(numb);
        mt19937 ran((random_device())());
        uniform_real_distribution<> xRan(-box.boxWidth,box.boxWidth);
        uniform_real_distribution<> yRan(-box.boxHeight, box.boxHeight);
        uniform_real_distribution<> zRan(-box.boxDepth, box.boxDepth);
        uniform_real_distribution<> radRan(0.1,0.3);
        uniform_real_distribution<> velRanX(-0.01f, +0.01f);
        uniform_real_distribution<> velRanY(-0.01f, +0.01f);
        uniform_real_distribution<> velRanZ(-0.01f, +0.01f);
        uniform_real_distribution<> colorRan(0.2, 1.0);
    
        for (int i = 0; i < numb; i++)
        {
            BounceBall newBall;
            newBall.appearBall(xRan, yRan, zRan, radRan, velRanX,velRanY,velRanZ, colorRan, ran);
            balls.push_back(newBall);

        }
    

    }


    void randInitTerrain() {
        obst.reserve(16);
        mt19937 ran((random_device())());
        uniform_real_distribution<> xRan(-box.boxWidth,box.boxWidth);
        uniform_real_distribution<> yRan(-box.boxHeight, box.boxHeight);
        uniform_real_distribution<> zRan(-box.boxDepth, box.boxDepth);
        uniform_real_distribution<> radRan(0.2,0.25);
        uniform_real_distribution<> colorRan(0.2, 1.0);
        for (int i = 0; i < 4; i++){
            Terrain newObst;
            newObst.appearTerrainx(yRan, zRan, radRan, colorRan, ran, 1.0);
            obst.push_back(newObst);
        }
        for (int i = 0; i < 4; i++){
            Terrain newObst;
            newObst.appearTerrainx(yRan, zRan, radRan, colorRan, ran, -1.0);
            obst.push_back(newObst);
        }
        for (int i = 0; i < 4; i++){
            Terrain newObst;
            newObst.appearTerrainy(xRan, zRan, radRan,  colorRan, ran, 1.0);
            obst.push_back(newObst);
        }
        for (int i = 0; i < 4; i++){
            Terrain newObst;
            newObst.appearTerrainy(xRan, zRan, radRan,colorRan, ran, -1.0);
            obst.push_back(newObst);
        }
    }

};


GUI graphics;


struct Mouse
{
    int x;      /*  the x coordinate of the mouse cursor    */
    int y;      /*  the y coordinate of the mouse cursor    */
    int lmb;    /*  is the left button pressed?     */
    int mmb;    /*  is the middle button pressed?   */
    int rmb;    /*  is the right button pressed?    */


    int xpress; /*  stores the x-coord of when the first button press occurred  */
    int ypress; /*  stores the y-coord of when the first button press occurred  */
};

/*
 *  rename the structure from "struct Mouse" to just "Mouse"
 */
typedef struct Mouse Mouse;

/*
 *  Create a global mouse structure to hold the mouse information.
 */
Mouse TheMouse = {0,0,0,0,0};




int num  = 0;
int delay = 70;
void Init()
{
    glClearColor(0,0,0,0);
    graphics.initializeImage();
    graphics.randInitTerrain();
    graphics.randInitBalls();
    graphics.initializeButton();
    prevRed = balls[numb -1].red;
    prevGreen = balls[numb -1].green;
    prevBlue = balls[numb -1].blue;

    for( int i = 0; i < WATERSIZE; i++) 
        for(int j = 0; j < WATERSIZE; j++) {
            water[0][j][i] = 0;
            water[1][j][i] = 0;
        }
 
}

/*----------------------------------------------------------------------------------------
 *  This function will be used to draw the 3D scene
 */
// void Draw3D()
// {
//  gluLookAt(0,1,5,0,0,0,0,1,0);
//  glutSolidTeapot(1);
// }

/*----------------------------------------------------------------------------------------
 *  This function will be used to draw an overlay over the 3D scene.
 *  This will be used to draw our fonts, buttons etc......
 */
// void Draw2D()
// {
//  ButtonDraw(&MyButton);
// }

/*----------------------------------------------------------------------------------------
 *  This is the main display callback function. It sets up the drawing for
 *  The 3D scene first then calls the Draw3D() function. After that it switches to
 *  an orthographic projection and calls Draw2D().
 */

void Draw(void)
{
    /*
     *  Clear the background
     */
    glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );

    /*
     *  Enable lighting and the z-buffer
     */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    /*
     *  Set perspective viewing transformation
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,(winh==0)?(1):((float)winw/winh),0.2,10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
     *  Draw the 3D elements in the scene
     */
    for (int i = 0; i < numb; i++){
        graphics.showBall(balls[i]);
    }
       for (int i = 0; i < 16; i++){
        graphics.showTerrain(obst[i]);
    }

    glEnable(GL_TEXTURE_2D);
   
    graphics.showBox(box);
    glDisable(GL_TEXTURE_2D);

 

    /*
     *  Disable depth test and lighting for 2D elements
     */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    /*
     *  Set the orthographic viewing transformation
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45.0f, winw/winh, 0.1f, 100.0f);
    glOrtho(0,winw,winh,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
     *  Draw the 2D overlay
     */
    graphics.showButton(&increaseSpeed);
     graphics.showButton(&decreaseSpeed);
     graphics.showButton(&pauseButton);

    /*
     *  Bring the back buffer to the front and vice-versa.
     */
     glFlush();
    // glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
 *  This function is called when the window is resized. All this does is simply
 *  store the new width and height of the window which are then referenced by
 *  the draw function to set the correct viewing transforms
 */
void Resize(int w, int h)
{
    winw = w;
    winh = h;
cout<<winw<<"\t"<<winh<<"\n";
// Use the Projection Matrix

// Reset Matrix
    /*
     *  Allow drawing in full region of the screen
     */
    glViewport(0,0,w,h);
    
}

int ButtonClickTest(Button* b,int x,int y) 
{
    if( b) 
    {
        /*
         *  If clicked within button area, then return true
         */
        if( x > b->xPos     && 
            x < b->xPos+b->butWidth &&
            y > b->yPos     && 
            y < b->yPos+b->butHeight ) {
                return 1;
        }
    }

    /*
     *  otherwise false.
     */
    return 0;
}
void pausefunction(bool pause)
{
    if(pause)
    {
        for(int i = 0; i < numb; i++)
        {
            old_speed[i] = balls[i].velocity;
            balls[i].velocity.x = balls[i].velocity.y = balls[i].velocity.z = 0;


        }
    }
    else
    {
        for(int i = 0; i < numb; i++)
        {
            balls[i].velocity = old_speed[i];
            old_speed[i].x = old_speed[i].y = old_speed[i].z = 0;
        }
    }
}
void Pausebut(bool fPause){
 fPause = !fPause;
        if (fPause == true){
            char s[100] = "Play";
            pauseButton.setText(s);
        }
        else{
            char s[100] = "Pause";
            pauseButton.setText(s);
        }
        pausefunction(fPause);

}



void ButtonRelease(Button *b,int x,int y)
{
    if(b) 
    {
        /*
         *  If the mouse button was pressed within the button area
         *  as well as being released on the button.....
         */
        if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) && 
            ButtonClickTest(b,x,y) )
        {
            for(int i = 0; i < numb; i++)
        {
              balls[i].velocity.x /= 1.5f;
            balls[i].velocity.y /= 1.5f;
            balls[i].velocity.z /= 1.5f;


        }
          
                  }

        /*
         *  Set state back to zero.
         */
        b->isPressed=0;
    }
}
// void PauseButtonCallback()
// {
//     printf("I have been called\n");
// }
/*----------------------------------------------------------------------------------------
 *  \brief  This function draws the specified button.
 *  \param  b   -   a pointer to the button to check.
 *  \param  x   -   the x location of the mouse cursor.
 *  \param  y   -   the y location of the mouse cursor.
 */
void ButtonPress(Button *b,int x,int y)
{
    if(b)
    {
        /*
         *  if the mouse click was within the buttons client area, 
         *  set the state to true.
         */
        if( ButtonClickTest(b,x,y) )
        {
            b->isPressed=1;

        }
    }
}
void MouseButton(int button,int state,int x, int y)
{
    /*
     *  update the mouse position
     */
    TheMouse.x = x;
    TheMouse.y = y;

    /*
     *  has the button been pressed or released?
     */
    if (state == GLUT_DOWN)
    {
        /*
         *  This holds the location of the first mouse click
         */
        if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
            TheMouse.xpress = x;
            TheMouse.ypress = y;
            
        }

        /*
         *  Which button was pressed?
         */
        switch(button)
        {
        case GLUT_LEFT_BUTTON:
            TheMouse.lmb = 1;
            ButtonPress(&pauseButton,x,y);
            ButtonPress(&increaseSpeed,x,y);
            ButtonPress(&decreaseSpeed,x,y);
        case GLUT_MIDDLE_BUTTON:
            TheMouse.mmb = 1;
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.rmb = 1;
            break;
        }
        // cout<<"\n"<<"mouse\t"<< x <<"\t"<< y <<"\n";
    }
    else
    {
        /*
         *  Which button was released?
         */
        switch(button)
        {
        case GLUT_LEFT_BUTTON:
            TheMouse.lmb = 0;
             ButtonRelease(&pauseButton,x,y);
            ButtonRelease(&increaseSpeed,x,y);
            ButtonRelease(&decreaseSpeed,x,y);
            break;
        case GLUT_MIDDLE_BUTTON:
            TheMouse.mmb = 0;
            break;
        case GLUT_RIGHT_BUTTON:
            TheMouse.rmb = 0;
            break;
        }
    }

    /*
     *  Force a redraw of the screen. If we later want interactions with the mouse
     *  and the 3D scene, we will need to redraw the changes.
     */
    glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
    /*
     *  Calculate how much the mouse actually moved
     */
    int dx = x - TheMouse.x;
    int dy = y - TheMouse.y;

    /*
     *  update the mouse position
     */
    TheMouse.x = x;
    TheMouse.y = y;

    /*
     *  Force a redraw of the screen
     */
    glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *  \brief  This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 *  \param  x   -   the new x-coord of the mouse cursor.
 *  \param  y   -   the new y-coord of the mouse cursor.
 */
void MousePassiveMotion(int x, int y)
{
    /*
     *  Calculate how much the mouse actually moved
     */
    int dx = x - TheMouse.x;
    int dy = y - TheMouse.y;

    /*
     *  update the mouse position
     */
    TheMouse.x = x;
    TheMouse.y = y;


}



void keyboard(unsigned char c, int x, int y){
        
    
    if(curBall < numb){
        if(c == '+'){
            if(balls[curBall].velocity.x < maxVel && balls[curBall].velocity.x > -maxVel)
                balls[curBall].velocity.x *= 1.5f;
            else  
            balls[curBall].velocity.x = maxVel;
            if(balls[curBall].velocity.y < maxVel && balls[curBall].velocity.y > -maxVel)
                balls[curBall].velocity.y *= 1.5f;
            else  
            balls[curBall].velocity.y = maxVel;
            if(balls[curBall].velocity.z < maxVel && balls[curBall].velocity.z > -maxVel)
                balls[curBall].velocity.z *= 1.5f;
            else 
         balls[curBall].velocity.z = maxVel;
        }
        else if(c == '-'){
            balls[curBall].velocity.x /= 1.5f;
            balls[curBall].velocity.y /= 1.5f;
            balls[curBall].velocity.z /= 1.5f;
        }
        else if(c == 'd'){
            balls[curBall].red =  prevRed;
            balls[curBall].green = prevGreen;
            balls[curBall].blue = prevBlue;
            curBall = curBall + 1;
            if(curBall == numb)
                curBall = 0;
            prevRed = balls[curBall].red;
            prevGreen = balls[curBall].green;
            prevBlue = balls[curBall].blue;
            balls[curBall].red = balls[curBall].green = balls[curBall].blue = 1.0;
        }
        else if(c == 'a'){
            balls[curBall].red =  prevRed;
            balls[curBall].green = prevGreen;
            balls[curBall].blue = prevBlue;
            curBall = curBall - 1;
            if(curBall < 0)
                curBall = numb -1;
            prevRed = balls[curBall].red;
            prevGreen = balls[curBall].green;
            prevBlue = balls[curBall].blue;
            balls[curBall].red = balls[curBall].green = balls[curBall].blue = 1.0;
        }
        else if(c == 'w'){
            slices++;
            stacks++;
        }
        else if(c == 's'){
            if(slices>2 && stacks>2){
                slices--;
                stacks--;
            }
        }
        else if(c == 'f'){
            if(lon > 2){
                lon --;
            }
            cout<<lon<<" "<<lat<<endl;
        }
        else if(c == 'g'){
            if(lat > 2){
                lat --;
            }
            cout<<lon<<" "<<lat<<endl;
        }
        else if(c == 'h'){
            lon++;
            cout<<lon<<" "<<lat<<endl;
        }
        else if(c == 't'){
            lat++;
            cout<<lon<<" "<<lat<<endl;
        }
    }

    if (c == ' '){
        funcPause = !funcPause;
        if (funcPause == true){
            char s[100] = "Play";
            pauseButton.setText(s);
        }
        else{
            char s[100] = "Pause";
            pauseButton.setText(s);
        }
        pausefunction(funcPause);
    }

    glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

    float fraction = 0.3f;

    switch (key) {
        case GLUT_KEY_LEFT :
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT :
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP :
           if(z <= 0.0f && z>-5.1f){
                if(angle==0.0){
                    x += lx * fraction;
                    z += lz * fraction;    
                }
                else{
                    angle = 0.0f;
                    x = 0.0f;
                    //z=0.0f;
                    lz = -0.1f;
                    lx = 0.0f;
                }
            }
            break;
        case GLUT_KEY_DOWN :
            if(z <= -0.1f && angle==0.0){
                x -= lx * fraction;
                z -= lz * fraction;
            }
            break;
        case GLUT_KEY_F5 :
            angle = 0.0f;
            x = 0.0f;
            //z=0.0f;
            lz = -0.1f;
            lx = 0.0f;
            //Draw();
            //angle = 0.0f;

            //y = 0.0f;
            break;
    }
}


static void idle(void)
{
    if(!(++num %delay))
    {
        water[f][rand()%WATERSIZE][rand()%WATERSIZE] = -rand()%100;
        delay = rand()%100 + 50;
    }
    //cout<<balls[0].centre.z<<"\n";
    xRotated += 0.1;
    yRotated += 0.1;
    zRotated += 0.1;
     physics.ballCollisionDetection(balls);
     physics.TerrainCollisionDetection(balls,obst);
    for (int i = 0; i < numb; i++){
       
        physics.wallCollision(balls[i],box);
        
        balls[i].centre.x +=balls[i].velocity.x;
        balls[i].centre.y +=balls[i].velocity.y;
        balls[i].centre.z +=balls[i].velocity.z;
    }
    // if(TheMouse.lmb == 1)
    //     cout<<balls[0].centre.x<<"\t"<<balls[0].centre.y<<"\n";
   Draw();
}
/*----------------------------------------------------------------------------------------
 *
 */
int main(int argc,char **argv)
{   
    numb=atoi(argv[1]);
    // cin>>numb;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_SINGLE);
    glutInitWindowSize(winw,winh);
    glutInitWindowPosition(200,100);
    glutCreateWindow("N ball screensaver");
    old_speed.resize(numb);
    Init();
    glutDisplayFunc(Draw);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MousePassiveMotion);
    glutIdleFunc(idle);
    glutSpecialFunc(processSpecialKeys);


    glutMainLoop();
    return 0;
}