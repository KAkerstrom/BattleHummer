#include "Shapes.h"
#include "Object.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

class Buildings : public Object
{
    public:
        double rotSp = 2.0f;
        double maxSpeed = 0.18;
        double accel = 0.03;
        double B_X,B_Z;
        int Decision=0;
        Buildings(Point3d _center, Point3d _size,int Quadrant) : Object(_center)
        {
            Decision = rand()%4;

            switch(Quadrant){
                case 0:
                   B_X = 0.95;B_Z=0.65;
                   break;
                case 1:
                   B_X = 0.95;B_Z=-0.65;
                   break;
                case 2:
                   B_X = -0.95;B_Z=0.15;
                   break;
                case 3:
                   B_X =-0.95;B_Z=0.65;
                   break;
            }
            switch(Decision){
                case 0:
                    {
                    Rect3d *foundation = new Rect3d(Point3d(B_X,0.2,B_Z), _size, Color(125, 125, 0, 0.4));
                    shapes.push_back(foundation);
                    _center.Y = _center.Y;
                    Point3d _size(0.8,0.45,0.25);
                    Rect3d *WindowsLR = new Rect3d(Point3d(B_X,0.2,B_Z), _size,Color(0,0,125,0.8));
                    Point3d _size2(0.25,0.35,0.8);
                    Rect3d *WindowsFB = new Rect3d(Point3d(B_X,0.2,B_Z), _size2,Color(0,0,125,0.8));
                    shapes.push_back(WindowsLR);
                    shapes.push_back(WindowsFB);
                    break;
                    }
                case 1:
                   {

                    Rect3d *top = new Rect3d(Point3d(B_X,0.6,B_Z), Point3d(0.62,0.4,0.62), Color(125, 15, 40, 0.4));
                    shapes.push_back(top);
                    Cylinder *foundation = new Cylinder(Point3d(B_X,0.4,B_Z),0.9f,0.3f,Color(125,75,15,0.6),Color(125,75,15,0.6));
                    foundation -> SetRotation(Rotation(Point3d(1,0,0), 0));
                    foundation -> Rotate(90);
                    shapes.push_back(foundation);
                    break;
                   }
                case 2:
                   {

                    Cylinder *top = new Cylinder(Point3d(B_X+0.4,0.7,B_Z),0.01f,0.25f,Color(125,75,15,0.6),Color(125,75,15,0.6));
                    top -> SetRotation(Rotation(Point3d(1,1,0), 0));
                    top -> Rotate(45);
                    shapes.push_back(top);
                    Sphere *Foundation = new Sphere(Point3d(B_X,0.2,B_Z),0.5,Color(111,45,45,0.8));
                    shapes.push_back(Foundation);
                    break;
                   }
                default:
                    Rect3d *foundation = new Rect3d(Point3d(B_X,0.2,B_Z), Point3d(0,0,0), Color(125, 125, 0, 0.4));
                    shapes.push_back(foundation);
                    break;
            }


        };
         void UpdatePosition()
        {
            if(Decision>0){
                shapes[0]->Rotate(2);
            }
        }
};
