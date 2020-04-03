#include "Shapes.h"
#include "Object.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

class BattleHummer : public Object
{
    public:
        Shape* antenna;
        Point3d * followPoint;
        Point3d * aheadPoint;
        bool rotR, rotL, throttle, brake;
        double moveSp, rotSp, followDist, fDist = 1.5;
        double maxSpeed = 0.18;
        double accel = 0.03;
    
        BattleHummer(Point3d _center, float _size = 1.0f) : Object(_center)
        {
            followPoint = new Point3d(0, 1.3, 0);
            aheadPoint = new Point3d(0, 1.3, 0);
            moveSp = 0;
            rotSp = 2.5;

            rotR = rotL = throttle = brake = false;
    
            Point3d bodySize(2 * _size, 1 * _size, 3 * _size);
    
            // Create rocket launchers
            Color cylinderColor(50, 5, 100);
            Color endColor(100, 5, 5);
            Cylinder *leftRL = new Cylinder
            (
                Point3d(1.3f * _size, 0.0f * _size, -1.0f * _size),
                0.5f * _size,
                0.3f * _size,
                cylinderColor,
                endColor
            );
            shapes.push_back(leftRL);
            Cylinder *rightRL = new Cylinder
            (
                Point3d(-1.3f * _size, 0.0f * _size, -1.0f * _size),
                0.5f * _size,
                0.3f * _size,
                cylinderColor,
                endColor
            );
            shapes.push_back(rightRL);

            // Create wheels
            Color wheelColor(0.1f, 0.1f, 0.1f);
            Color hubColor(0.2f, 0.2f, 0.2f);
            float wheelSize = 0.3f * _size;
            Cylinder *rf_wheel = new Cylinder
            (
                Point3d(-1.1f * _size, -0.7f * _size, -1.0f * _size),
                wheelSize, wheelSize, wheelColor, hubColor
            );
            rf_wheel->Rotate(90);
            shapes.push_back(rf_wheel);
            Cylinder *lf_wheel = new Cylinder
            (
                Point3d(0.8f * _size, -0.7f * _size, -1.0f * _size),
                wheelSize, wheelSize, wheelColor, hubColor
            );
            lf_wheel->Rotate(90);
            shapes.push_back(lf_wheel);
            Cylinder *rb_wheel = new Cylinder
            (
                Point3d(-1.1f * _size, -0.7f * _size, 1.0f * _size),
                wheelSize, wheelSize, wheelColor, hubColor
            );
            rb_wheel->Rotate(90);
            shapes.push_back(rb_wheel);
            Cylinder *lb_wheel = new Cylinder
            (
                Point3d(0.8f * _size, -0.7f * _size, 1.0f * _size),
                wheelSize, wheelSize, wheelColor, hubColor
            );
            lb_wheel->Rotate(90);
            shapes.push_back(lb_wheel);

            // Create cab
            Rect3d* cab = new Rect3d
            (
                Point3d(0.0f, 0.8f*_size, -0.5f*_size),
                Point3d(bodySize.X*0.95f, 0.8f*bodySize.Y, 0.5*bodySize.Z),
                Color(0.0f, 0.8f*_size, 0.3f*_size),
                0.5f
            );
            shapes.push_back(cab);

            // Create antenna
            Cylinder *wire = new Cylinder
            (
                Point3d(0, 0.85f*_size, 1*_size),
                0.8f * _size,
                0.05f * _size,
                Color(0,0,0),
                Color(0,0,0)
            );
            wire -> SetRotation(Rotation(Point3d(1,0,0), 90));
            wire->Rotate(90);
            shapes.push_back(wire);

            // Create antenna topper
            Teapot* teapot = new Teapot(Point3d(0,1*_size,1*_size), 0.2f*_size, Color(0.5f,0.5f,0.5f));
            shapes.push_back(teapot);
            antenna = teapot;

            // Create body
            Rect3d *body = new Rect3d(Point3d(0,0,0), bodySize, Color(117, 50, 60));
            shapes.push_back(body);
        };
        void SetFDist(double Dist)
        {
            fDist= Dist;
        }
        void UpdatePosition()
        {
            antenna -> Rotate(2);

            Rotation rot = GetRotation();
            float z = moveSp * cos(rot.deg * M_PI/180);
            float x = moveSp * sin(rot.deg * M_PI/180);

            followDist = fDist + (moveSp * 1.6);

            followPoint->X = center.X - followDist * cos((-rot.deg - 90) * M_PI/180);
            followPoint->Z = center.Z - followDist * sin((-rot.deg - 90) * M_PI/180);
            aheadPoint->X =center.X + followDist * cos((-rot.deg - 90) * M_PI/180);
            aheadPoint->Z = center.Z + followDist * sin((-rot.deg - 90) * M_PI/180);
            Move(Point3d(-x, 0, -z));

            if(throttle)
            {
                if(moveSp < maxSpeed)
                    moveSp += accel;
            }
            else
            {
                if(moveSp > 0)
                    moveSp -= accel;
            }

            if(brake)
            {
                if(moveSp > 0)
                    moveSp -= accel * 2;
            }

            if(rotL)
            {
                Rotate(rotSp);
            }
            if(rotR)
            {
                Rotate(-rotSp);
            }

            if(moveSp <= 0)
                    moveSp = 0;

            if(moveSp >= maxSpeed)
                    moveSp = maxSpeed;

        };

        ~BattleHummer()
        {
            delete followPoint;
        }
};
