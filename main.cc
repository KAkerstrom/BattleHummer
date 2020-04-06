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
#include "Buildings.h"

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

// Some global variables.
// Window IDs, window width and height.
int Window_ID;
int Window_Width = 600;
int Window_Height = 400;
std::vector<Buildings> Rotatepieces;
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
Point3d lookPoint(L_X, L_Y, L_Z);
Point3d eye(EyeX, EyeY, EyeZ);
Point3d up(0, 1, 0);
int ViewDirection = 0;
int OutIn = 0;
bool paused = false;

//Ortho p (Point3d(0,0,0), Point3d(Window_Width/50, Window_Height/50, 500));
Perspective p(45.0f, (GLfloat)Window_Width / (GLfloat)Window_Height, 0.1f, 30.0f);
Camera c(eye, lookPoint, up);
Rect3d r(Point3d(0, 0, 0), Point3d(2, 2, 2), Color(250, 100, 10, 0));
View gameView(Point2d(Window_Width * 0.1, 0), Point2d(1, 1), &p, &c);

//Ortho p1(Point3d(0, 0, 0), Point3d(Window_Width / 50, Window_Height / 50, 500));
//Rect3d world(Point3d(0, 0, 0), Point3d(200, 0.25, 200), Color(0, 0, 0, 0));

BattleHummer humm(Point3d(-3.0f, 0.5, 1), 0.3f);
//BattleHummer test(Point3d(0, 0.5f, -2.0f), 0.3f);

void CallBackRenderScene(void)
{
    Point3d hPoint = humm.GetCenter();

    if (OutIn > 0)
    {
        Point3d fPoint(humm.followPoint->X, humm.followPoint->Y, humm.followPoint->Z);
        Point3d aPoint(humm.aheadPoint->X, humm.aheadPoint->Y, humm.aheadPoint->Z);
        switch (OutIn)
        {
            case 1:
                humm.SetFDist(1.5);
                fPoint.X = fPoint.X - sin((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Z = fPoint.Z - cos((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Y = 2;
                break;
            case 2:
                humm.SetFDist(1.5);
                fPoint.X = aPoint.X - sin((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Z = aPoint.Z - cos((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Y = 2;
                break;
            case 3:
                humm.SetFDist(1.5);
                fPoint.X = (aPoint.X - sin((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Z = (aPoint.Z - cos((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Y = 2;
                break;
            case 4:
                humm.SetFDist(1.5);
                fPoint.X = (fPoint.X - sin((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Z = (fPoint.Z - cos((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Y = 2;
                break;
            case 5:
                humm.SetFDist(3.5);
                fPoint.X = fPoint.X - sin((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Z = fPoint.Z - cos((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Y = 4;
                break;
            case 6:
                humm.SetFDist(3.5);
                fPoint.X = aPoint.X - sin((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Z = aPoint.Z - cos((humm.GetRotation().deg - 90) * M_PI / 180);
                fPoint.Y = 4;
                break;
            case 7:
                humm.SetFDist(3.5);
                fPoint.X = (aPoint.X - sin((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Z = (aPoint.Z - cos((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Y = 4;
                break;
            case 8:
                humm.SetFDist(3.5);
                fPoint.X = (fPoint.X - sin((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Z = (fPoint.Z - cos((humm.GetRotation().deg + 90) * M_PI / 180));
                fPoint.Y = 4;
                break;
        }

        c.SetEye(fPoint);
        c.SetLookAt(hPoint);
    }
    else
    {
        Point3d fPoint(hPoint);
        fPoint.Y = fPoint.Y + 0.2;

        c.SetEye(fPoint);
        hPoint.Y = hPoint.Y + 0.2;
        switch (ViewDirection)
        {
            case 0:
                hPoint.X = hPoint.X - cos((humm.GetRotation().deg - 90) * M_PI / 180);
                hPoint.Z = hPoint.Z + sin((humm.GetRotation().deg - 90) * M_PI / 180);
                c.SetLookAt(hPoint);
                break;
            case -1:
                hPoint.X = hPoint.X - sin((humm.GetRotation().deg + 90) * M_PI / 180);
                hPoint.Z = hPoint.Z - cos((humm.GetRotation().deg + 90) * M_PI / 180);
                c.SetLookAt(hPoint);
                break;
            case 1:
                hPoint.X = hPoint.X - sin((humm.GetRotation().deg - 90) * M_PI / 180);
                hPoint.Z = hPoint.Z - cos((humm.GetRotation().deg - 90) * M_PI / 180);
                c.SetLookAt(hPoint);
                break;
            default:
                hPoint = humm.GetCenter();
                break;
        }
    }
    c.LookAt();

    gameView.SetPosition(Point2d(0, 0));
    gameView.SetSize(Point2d(Window_Width, Window_Height));
    p.aspect = (float)(Window_Width / Window_Height);
    gameView.Draw(40);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glutSwapBuffers();
}

void SpecialKey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:  ViewDirection =  0; break;
        case GLUT_KEY_F2:  ViewDirection =  1; break;
        case GLUT_KEY_F3:  ViewDirection = -1; break;
        case GLUT_KEY_F4:  OutIn = ViewDirection = 0; break;
        case GLUT_KEY_F5:  OutIn = ViewDirection = 1; break;
        case GLUT_KEY_F6:  OutIn = 2; break;
        case GLUT_KEY_F7:  OutIn = 3; break;
        case GLUT_KEY_F8:  OutIn = 4; break;
        case GLUT_KEY_F9:  OutIn = 5; break;
        case GLUT_KEY_F10: OutIn = 6; break;
        case GLUT_KEY_F11: OutIn = 7; break;
        case GLUT_KEY_F12: OutIn = 8; break;
    }
}

void KeyPressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'A':
        case 'a':
            humm.Drive(forward);
            break;
        case 'Q':
        case 'q':
            humm.Steer(left);
            break;
        case 'Z':
        case 'z':
            humm.Drive(reverse);
            break;
        case 'W':
        case 'w':
            humm.Steer(right);
            break;
        case 'R':
        case 'r':
            humm.ResetPos();
            break;
        case 'P':
        case 'p':
            paused = !paused;
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
        case 'A':
        case 'a':
        case 'Z':
        case 'z':
            humm.Drive(neutral);
            break;
        case 'Q':
        case 'q':
        case 'W':
        case 'w':
            humm.Steer(straight);
            break;
        default:
            printf("Unrecognized key released: %d\n", key);
    }
}

void CallBackResizeScene(int Width, int Height)
{
    // Let's not core dump, no matter what.
    if (Height == 0)
        Height = 1;

    if (Width < Height)
    {
        Width = Height;
        glutReshapeWindow(Width, Height);
    }

    Window_Width = Width;
    Window_Height = Height;
}

void MouseClick(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
        switch (button)
        {
        case GLUT_RIGHT_BUTTON:
            gameView.camera->eye.X = 5;
            gameView.camera->eye.Y = 5;
            break;

        default:
            break;
        }
    else if (state == GLUT_UP)
    {
        gameView.camera->eye.X = 0;
        gameView.camera->eye.Y = 0;
    }
}

void MyInit(int Width, int Height)
{
    glEnable(GL_BLEND);
    glClearColor(0.6f, 0.6f, 1.0f, 0.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    CallBackResizeScene(Width, Height);
}

void Timer(int id)
{
    Point3d hc = humm.GetCenter();
    if (hc.X < -104 || hc.X > 104
        || hc.Z < -104 || hc.Z > 104
        || hc.Y < 0)
            humm.Move(Point3d(0,-0.1f,0));

    if (hc.Y < -2)
        humm.ResetPos();

    if (!paused)
    {
        for (auto &Buildings : Rotatepieces)
            Buildings.UpdatePosition();
        humm.UpdatePosition();
        //test.Rotate(1);
    }

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
    printf("\tDevon Kuss\n");
    printf("\n");

    //test.Rotate(270);
    //gameView.AddShape(&test);
    gameView.AddShape(&humm);

    int worldScaling = 2;
    for (int x = -102; x <= 102; x += 6)
        for (int z = -102; z <= 102; z += 6)
        {
            Point3d blockSize(1.75f * worldScaling, 0.25f * worldScaling, 1.75f * worldScaling);
            Rect3d *block = new Rect3d(Point3d(x, 0.0, z), blockSize, Color(75, 75, 75, 0));
            gameView.AddShape(block);

            for (int quads = 0;quads < 3; quads++)
            {
                Buildings *BlockBuilding = new Buildings(Point3d(x, 0.5, z), Point3d(0.75, 2.5, 0.75),quads);
                gameView.AddShape(BlockBuilding);
                Rotatepieces.push_back(*BlockBuilding);
            }

            // draw white outline
            Rect3d* roadLine1 = new Rect3d
            (
                Point3d(x, 0.07f*worldScaling, z),
                Point3d(blockSize.X*1.2f, 0,blockSize.Z*1.2f),
                Color(255,255,255)
            );
            gameView.AddShape(roadLine1);
            // draw road
            Rect3d* roadLine = new Rect3d
            (
                Point3d(x, 0.065f*worldScaling, z),
                Point3d(blockSize.X*2.0f,0 ,blockSize.Z*2.0f),
                Color(5,5,5)
            );
            gameView.AddShape(roadLine);

            // Draw dotted lines
            Rect3d* roadLine2 = new Rect3d
            (
                Point3d(x + 1.5f*worldScaling, 0.07f*worldScaling, z - 0.75f*worldScaling),
                Point3d(0.1f*worldScaling, 0,0.6f*worldScaling),
                Color(255,255,255)
            );
            gameView.AddShape(roadLine2);

            Rect3d* roadLine3 = new Rect3d
            (
                Point3d(x + 1.5f*worldScaling, 0.07f*worldScaling, z + 0.75f*worldScaling),
                Point3d(0.1f*worldScaling, 0,0.6f*worldScaling),
                Color(255,255,255)
            );
            gameView.AddShape(roadLine3);

            Rect3d* roadLine4 = new Rect3d
            (
                Point3d(x - 0.75f*worldScaling, 0.07f*worldScaling, z + 1.5f*worldScaling),
                Point3d(0.6f*worldScaling,0, 0.1f*worldScaling),
                Color(255,255,255)
            );
            gameView.AddShape(roadLine4);

            Rect3d* roadLine5 = new Rect3d
            (
                Point3d(x + 0.75f*worldScaling, 0.07f*worldScaling, z + 1.5f*worldScaling),
                Point3d(0.6f*worldScaling, 0,0.1f*worldScaling),
                Color(255,255,255)
            );
            gameView.AddShape(roadLine5);


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
