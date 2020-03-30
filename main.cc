#define PROGRAM_TITLE "CPSC 3710 Project"
#define DISPLAY_INFO  "CPSC 3710 Project"

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#endif

#include <stdlib.h>  // Useful for the following includes.
#include <stdio.h>
#include <string.h>  // For spring operations.
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
float X_Rot   = 0.0f;
float Y_Rot   = 30.0f;
float X_Speed = 0.0f;
float Y_Speed = 0.05f;

float Z_Max = -3.0f;
float Z_Min = -99.0f;
float Z_Speed = 0.05f;
float Z_Max_Speed = 0.005f;
float Z_Off   = -5.0f;

//////////////////////////////////////////////////////////
// String rendering routine; leverages on GLUT routine. //
//////////////////////////////////////////////////////////
static void PrintString(void *font, char *str)
{
   int i,len=strlen(str);

   for(i=0;i < len; i++)
      glutBitmapCharacter(font,*str++);
}

//Point3d cubeCenter (0,0,0);
Point3d lookPoint (0, 0, 0);
Point3d eye (0, 0, 5);
Point3d up (0, 1, 0);

Ortho p (Point3d(0,0,0), Point3d(Window_Width/50, Window_Height/50, 500));
Camera c (eye, lookPoint);
Rect3d r (Point3d(0,0,0), Point3d(2,2,2), Color(250, 100, 10, 0));
View view3 (Point2d(Window_Width*0.1,0), Point2d(1, 1), &p, &c);

Ortho p1 (Point3d(0,0,0), Point3d(Window_Width/50, Window_Height/50, 500));
Text text (Point3d(0,0,0), Color(255,0,0), "Test Test Test");
View view1 (Point2d(Window_Width*0.1,0), Point2d(1, 1), &p1, &c);

Rect3d world (Point3d(0,0,0), Point3d(100,0.25,100), Color(0,0,0,0));

/////////////////////////////////////////////////////////
// Routine which actually does the drawing             //
/////////////////////////////////////////////////////////
void CallBackRenderScene(void)
{
    view1.Draw();
    view3.Draw();
    
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glutSwapBuffers();
}

// Only used for exiting the program
void KeyPressed(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'a':
         eye.Z += Z_Speed;
         lookPoint.Z += Z_Speed;
         break;
      
      case 'z':
         eye.Z -= Z_Speed;
         lookPoint.Z -= Z_Speed;
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

///////////////////////////////////////////////////////////////
// Callback routine executed whenever the window is resized. //
//////////////////////////////////////////////////////////////
void CallBackResizeScene(int Width, int Height)
{
   // Let's not core dump, no matter what.
   if (Height == 0)
      Height = 1;

    view3.SetPosition(Point2d(Width*0.1, 0));
    view3.SetSize(Point2d(Width*0.9, Height*0.9));
    
    view1.SetPosition(Point2d(Width*0.1, Height*0.9));
    view1.SetSize(Point2d(0,0));//Width*0.9, Height*0.1));

   Window_Width  = Width;
   Window_Height = Height;
}

void MouseClick(int button, int state, int x, int y)
{
   if(state == GLUT_DOWN)
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

BattleHummer test(Point3d(0,0,0));

void Timer(int id)
{
    test.Rotate(0.3f);
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
    printf("\tDeangelo Kuss\n");
    printf("\n");

    r.SetColor(side_back,   Color(c_yellow, 0.0f));
    r.SetColor(side_front,  Color(c_red,    0.2f));
    r.SetColor(side_left,   Color(c_cyan,   0.4f));
    r.SetColor(side_right,  Color(c_maroon, 0.0f));
    r.SetColor(side_top,    Color(c_purple, 0.6f));
    r.SetColor(side_bottom, Color(c_teal,   0.4f));
    //view1.AddShape(&text);
    //view3.AddShape(&test);
   view3.AddShape(&world);
   //can obviously be abstracted out. Purely to test scale and appearance.
    for (int x = -50; x < 50; x+=3) {
        for (int y = -50; y < 50; y+=3){
            Rect3d* block = new Rect3d(Point3d(x, 0.1, y), Point3d(1.75,0.25,1.75), Color(75,75,75,0));
            view3.AddShape(block);
            Rect3d* middleLine = new Rect3d(Point3d(x + 1.5, 0.1, y), Point3d(0.1, 0.25, 0.75), Color(255,255,255,0));
            view3.AddShape(middleLine);
            //middleLine->Rotate(45);
            //view3.AddShape(middleLine);
            Rect3d* middleLine2 = new Rect3d(Point3d(x + 1.5, 0.1, y + 1.5), Point3d(0.1, 0.25, 0.75), Color(255, 255, 255, 0));
            view3.AddShape(middleLine2);
            Rect3d* edgeLine = new Rect3d(Point3d(x + 1.0, 0.1, y), Point3d(0.1, 0.25, 100), Color(255,255,255,0));
            view3.AddShape(edgeLine);
            Rect3d* edgeLine2 = new Rect3d(Point3d(x + 2.0, 0.1, y), Point3d(0.1, 0.25, 100), Color(255,255,255,0));
            view3.AddShape(edgeLine2);
            Rect3d* testBuilding = new Rect3d(Point3d(x + 0.25, 0.5, y - 0.15), Point3d(0.75, 1.5, 0.6), Color(125,125,0,0.4));
            view3.AddShape(testBuilding);
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(Window_Width, Window_Height);
    Window_ID = glutCreateWindow(PROGRAM_TITLE);

    glutDisplayFunc(&CallBackRenderScene);
    glutIdleFunc(&CallBackRenderScene);
    glutReshapeFunc(&CallBackResizeScene);
    glutMouseFunc(&MouseClick);
    glutTimerFunc(1, &Timer, 0);
    glutKeyboardFunc(&KeyPressed); // Only for exiting the program

    MyInit(Window_Width, Window_Height);

    glutMainLoop(); 
    return 1; 
}

