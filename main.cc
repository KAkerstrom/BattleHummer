#define PROGRAM_TITLE "CPSC 3710 Project"
#define DISPLAY_INFO "CPSC 3710 Project"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <stdlib.h> // Useful for the following includes.
#include <stdio.h>
#include <string.h> // For spring operations.
#include <vector>
#include "Shapes.h"
#include "View.h"
#include "BattleHummer.h"

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

// Some global variables.
// Window IDs, window width and height.
int Window_ID;
int Window_Width = 600;
int Window_Height = 400;

// Cube position and rotation speed variables.
float X_Rot = 0.0f;
float Y_Rot = 30.0f;
float X_Speed = 0.0f;
float Y_Speed = 0.05f;

float Z_Max = -3.0f;
float Z_Min = -99.0f;
float Z_Speed = 0.05f;
float Z_Max_Speed = 0.005f;
float Z_Off = -5.0f;

float EyeX = -0.5f;
float EyeY = 1.0f;
float EyeZ = 2.0f;
float L_X = -0.5f;
float L_Y = 0.0f;
float L_Z = 0.0f;
//////////////////////////////////////////////////////////
// String rendering routine; leverages on GLUT routine. //
//////////////////////////////////////////////////////////
static void PrintString(void *font, char *str)
{
   int i, len = strlen(str);

   for (i = 0; i < len; i++)
      glutBitmapCharacter(font, *str++);
}

//Point3d cubeCenter (0,0,0);
Point3d lookPoint (L_X, L_Y, L_Z);
Point3d eye (EyeX, EyeY, EyeZ);
Point3d up (0, 1, 0);
int ViewDirection = 0;
int OutIn = 0;

//Ortho p (Point3d(0,0,0), Point3d(Window_Width/50, Window_Height/50, 500));
Perspective p(45.0f, (GLfloat)Window_Width / (GLfloat)Window_Height, 0.1f, 100.0f);
Camera c(eye, lookPoint, up);
Rect3d r(Point3d(0, 0, 0), Point3d(2, 2, 2), Color(250, 100, 10, 0));
View view3(Point2d(Window_Width * 0.1, 0), Point2d(1, 1), &p, &c);

Ortho p1(Point3d(0, 0, 0), Point3d(Window_Width / 50, Window_Height / 50, 500));
Text text(Point3d(0, 0, 0), Color(255, 0, 0), "Test Test Test");
View view1(Point2d(Window_Width * 0.1, 0), Point2d(1, 1), &p1, &c);

Rect3d world(Point3d(0, 0, 0), Point3d(100, 0.25, 100), Color(0, 0, 0, 0));

BattleHummer humm(Point3d(0, 0.5, 0), 0.3f);

/////////////////////////////////////////////////////////
// Routine which actually does the drawing             //
/////////////////////////////////////////////////////////
void CallBackRenderScene(void)
{
    Point3d hPoint = humm.GetCenter();

if (OutIn >0)
{
   Point3d fPoint(humm.followPoint->X, humm.followPoint->Y, humm.followPoint->Z);
   Point3d aPoint(humm.aheadPoint->X, humm.aheadPoint->Y, humm.aheadPoint->Z);
switch(OutIn)
{
case 1:
    humm.SetFDist(1.5);
    fPoint.X = fPoint.X - sin((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Z = fPoint.Z- cos((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Y = 2;
    break;
case 2:
    humm.SetFDist(1.5);
    fPoint.X = aPoint.X - sin((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Z = aPoint.Z- cos((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Y = 2;
    break;
case 3:
    humm.SetFDist(1.5);
    fPoint.X = (aPoint.X - sin((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Z = (aPoint.Z- cos((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Y = 2;
    break;
case 4:
    humm.SetFDist(1.5);
    fPoint.X = (fPoint.X - sin((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Z = (fPoint.Z- cos((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Y = 2;
    break;
case 5:
    humm.SetFDist(3.5);
    fPoint.X = fPoint.X - sin((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Z = fPoint.Z- cos((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Y = 4;
    break;
case 6:
    humm.SetFDist(3.5);
    fPoint.X = aPoint.X - sin((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Z = aPoint.Z- cos((humm.GetRotation().deg-90) * M_PI/180);
    fPoint.Y = 4;
    break;
case 7:
    humm.SetFDist(3.5);
    fPoint.X = (aPoint.X - sin((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Z = (aPoint.Z- cos((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Y = 4;
    break;
case 8:
    humm.SetFDist(3.5);
    fPoint.X = (fPoint.X - sin((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Z = (fPoint.Z- cos((humm.GetRotation().deg+90) * M_PI/180));
    fPoint.Y = 4;
    break;
}


c.SetEye(fPoint);
c.SetLookAt(hPoint);


}
else{
    Point3d fPoint(hPoint);
    fPoint.Y = fPoint.Y +0.2;
   //std::cout << "h: " << hPoint.X << ", " << hPoint.Y << ", " << hPoint.Z << std::endl;
   //std::cout << "f: " << fPoint.X << ", " << fPoint.Y << ", " << fPoint.Z << std::endl;

   c.SetEye(fPoint);
   hPoint.Y = hPoint.Y +0.2;
    switch(ViewDirection)
    {
    case 0:
        hPoint.X = hPoint.X - cos((humm.GetRotation().deg-90) * M_PI/180);
        hPoint.Z = hPoint.Z+ sin((humm.GetRotation().deg-90) * M_PI/180);
        c.SetLookAt(hPoint);
        break;
    case -1:
        hPoint.X = hPoint.X - sin((humm.GetRotation().deg+90) * M_PI/180);
        hPoint.Z = hPoint.Z- cos((humm.GetRotation().deg+90) * M_PI/180);
        c.SetLookAt(hPoint);
        break;
    case 1:
        hPoint.X = hPoint.X - sin((humm.GetRotation().deg-90) * M_PI/180);
        hPoint.Z = hPoint.Z- cos((humm.GetRotation().deg-90) * M_PI/180);
        c.SetLookAt(hPoint);
        break;
    default:
        hPoint = humm.GetCenter();
        break;
    }
}
   c.LookAt();


   view1.Draw();
   view3.Draw();

   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glutSwapBuffers();
}
void SpecialKey(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_F1:
    ViewDirection = 0;
    break;
   case GLUT_KEY_F2:
    ViewDirection = 1;
    break;
    case GLUT_KEY_F3:
    ViewDirection = -1;
    break;
    case GLUT_KEY_F4:
        OutIn =0;
    ViewDirection = 0;
    break;
   case GLUT_KEY_F5:
       OutIn =1;
    ViewDirection = 1;
    break;
case GLUT_KEY_F6:
        OutIn =2;
        break;
    case GLUT_KEY_F7:
        OutIn =3;
    break;
   case GLUT_KEY_F8:
       OutIn =4;
    break;
    case GLUT_KEY_F9:
        OutIn =5;
    break;
    case GLUT_KEY_F10:
        OutIn =6;
    break;
   case GLUT_KEY_F11:
       OutIn =7;
    break;
    case GLUT_KEY_F12:
        OutIn =8;
    break;
    }
}
// Only used for exiting the program
void KeyPressed(unsigned char key, int x, int y)
{
   switch (key)
   {
   //case 'a':
   //   eye.Z += Z_Speed;
   //   lookPoint.Z += Z_Speed;
   //   break;
   //
   //case 'z':
   //   eye.Z -= Z_Speed;
   //   lookPoint.Z -= Z_Speed;
   //   break;
   case 'w':
      humm.throttle = true;
      humm.brake = false;
      break;
   case 'a':
      humm.rotL = true;
      humm.rotR = false;
      break;
   case 's':
      humm.brake = true;
      humm.throttle = false;
      break;
   case 'd':
      humm.rotR = true;
      humm.rotL = false;
      break;
   case 27:
      printf("Escape key pressed. Exiting...\n");
      glutDestroyWindow(Window_ID);
      exit(1);
      break;
   default:
      printf("Unrecognized key pressed: %d\n", key);
   }
}

void KeyReleased(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 'w':
      humm.throttle = false;
      break;
   case 'a':
      humm.rotL = false;
      break;
   case 's':
      humm.brake = false;
      break;
   case 'd':
      humm.rotR = false;
      break;
   default:
      printf("Unrecognized key released: %d\n", key);
   }
}

///////////////////////////////////////////////////////////////
// Callback routine executed whenever the window is resized. //
//////////////////////////////////////////////////////////////
void CallBackResizeScene(int Width, int Height)
{
   // Let's not core dump, no matter what.
   if (Height == 0)
      Height = 1;

   view3.SetPosition(Point2d(Width * 0.1, 0));
   view3.SetSize(Point2d(Width * 0.9, Height * 0.9));

   view1.SetPosition(Point2d(Width * 0.1, Height * 0.9));
   view1.SetSize(Point2d(0, 0)); //Width*0.9, Height*0.1));

   Window_Width = Width;
   Window_Height = Height;
}

void MouseClick(int button, int state, int x, int y)
{
   if (state == GLUT_DOWN)
      switch (button)
      {
      case GLUT_RIGHT_BUTTON:
         view3.camera->eye.X = 5;
         view3.camera->eye.Y = 5;
         break;

      default:
         break;
      }
   else if (state == GLUT_UP)
   {
      view3.camera->eye.X = 0;
      view3.camera->eye.Y = 0;
   }
}

void MyInit(int Width, int Height)
{
    glEnable(GL_BLEND);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    CallBackResizeScene(Width,Height);
}

//BattleHummer test(Point3d(0,0.5,0), 0.3f);

void Timer(int id)
{
   humm.UpdatePosition();
   //test.Rotate(0.3f);
   glutTimerFunc(1, &Timer, 0);
}

int main(int argc, char **argv)
{
    printf("\n\n");
    printf("CPSC 3710 Group Project\n");
    printf("Group Members:\n");
    printf("\tKyle Akerstrom\n");
    printf("\tBrandon Kuss\n");
    printf("\tMark Wagner\n");
    printf("\tBrandon Pardy\n");
    printf("\tDerek Kuss\n");
    printf("\n");

    r.SetColor(side_back,   Color(c_yellow, 0.0f));
    r.SetColor(side_front,  Color(c_red,    0.2f));
    r.SetColor(side_left,   Color(c_cyan,   0.4f));
    r.SetColor(side_right,  Color(c_maroon, 0.0f));
    r.SetColor(side_top,    Color(c_purple, 0.6f));
    r.SetColor(side_bottom, Color(c_teal,   0.4f));
    view1.AddShape(&text);
    //view3.AddShape(&test);
   view3.AddShape(&world);
   view3.AddShape(&humm);
   //can obviously be abstracted out. Purely to test scale and appearance.
    for (int x = -50; x < 50; x+=3) {
        for (int y = -50; y < 50; y+=3){
            Rect3d* block = new Rect3d(Point3d(x, 0.1, y), Point3d(1.75,0.25,1.75), Color(75,75,75,0));
            view3.AddShape(block);
            Rect3d* middleLine = new Rect3d(Point3d(x + 1.5, 0.1, y), Point3d(0.1, 0.15, 0.75), Color(255,255,255,0));
            view3.AddShape(middleLine);
            //middleLine->Rotate(45);
            //view3.AddShape(middleLine);
            Rect3d* middleLine2 = new Rect3d(Point3d(x + 1.5, 0.1, y + 1.5), Point3d(0.1, 0.15, 0.75), Color(255, 255, 255, 0));
            view3.AddShape(middleLine2);
            Rect3d* edgeLine = new Rect3d(Point3d(x + 1.0, 0.1, y), Point3d(0.1, 0.15, 25), Color(255,255,255,0));
            view3.AddShape(edgeLine);
            Rect3d* edgeLine2 = new Rect3d(Point3d(x + 2.0, 0.1, y), Point3d(0.1, 0.15, 25), Color(255,255,255,0));
            view3.AddShape(edgeLine2);
            Rect3d* testBuilding = new Rect3d(Point3d(x + 0.25, 0.5, y - 0.15), Point3d(0.75, 1.5, 0.6), Color(125,125,0,0.4));
            view3.AddShape(testBuilding);
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(Window_Width, Window_Height);
    Window_ID = glutCreateWindow(PROGRAM_TITLE);

    glutSpecialFunc(&SpecialKey);
    glutDisplayFunc(&CallBackRenderScene);
    glutIdleFunc(&CallBackRenderScene);
    glutReshapeFunc(&CallBackResizeScene);
    glutMouseFunc(&MouseClick);
    glutTimerFunc(1, &Timer, 0);
    glutKeyboardFunc(&KeyPressed); // Only for exiting the program
    glutKeyboardUpFunc(&KeyReleased);

    MyInit(Window_Width, Window_Height);

    glutMainLoop();
    return 1;
}
