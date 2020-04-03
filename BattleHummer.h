#include "Shapes.h"
#include "Object.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

class BattleHummer : public Object
{
    public:
        Point3d * followPoint;
        bool rotR, rotL, throttle, brake;
        double moveSp, rotSp, followDist;
        double maxSpeed = 0.28;
        double accel = 0.05;

        BattleHummer(Point3d _center, float _size = 1.0f) : Object(_center)
        {
            followPoint = new Point3d(0, 1.3, 0);
            moveSp = 0;
            rotSp = 2.5;

            rotR = rotL = throttle = brake = false;

            // Create body
            Rect3d *body = new Rect3d(Point3d(0, 0, 0), Point3d(2 * _size, 1 * _size, 3 * _size), Color(117, 50, 60));
            shapes.push_back(body);

            // Create rocket launchers
            Cylinder *leftRL = new Cylinder(Point3d(1.3f * _size, 0.0f * _size, -1.0f * _size), 0.5f * _size, 0.3f * _size, Color(23, 117, 48));
            shapes.push_back(leftRL);
            Cylinder *rightRL = new Cylinder(Point3d(-1.3f * _size, 0.0f * _size, -1.0f * _size), 0.5f * _size, 0.3f * _size, Color(23, 117, 48));
            shapes.push_back(rightRL);

            // Create wheels
            Cylinder *rf_wheel = new Cylinder(Point3d(-1.1f * _size, -0.7f * _size, -1.0f * _size), 0.3f * _size, 0.3f * _size, Color(23, 117, 48));
            rf_wheel->Rotate(90);
            shapes.push_back(rf_wheel);
            Cylinder *lf_wheel = new Cylinder(Point3d(0.8f * _size, -0.7f * _size, -1.0f * _size), 0.3f * _size, 0.3f * _size, Color(23, 117, 48));
            lf_wheel->Rotate(90);
            shapes.push_back(lf_wheel);
            Cylinder *rb_wheel = new Cylinder(Point3d(-1.1f * _size, -0.7f * _size, 1.0f * _size), 0.3f * _size, 0.3f * _size, Color(23, 117, 48));
            rb_wheel->Rotate(90);
            shapes.push_back(rb_wheel);
            Cylinder *lb_wheel = new Cylinder(Point3d(0.8f * _size, -0.7f * _size, 1.0f * _size), 0.3f * _size, 0.3f * _size, Color(23, 117, 48));
            lb_wheel->Rotate(90);
            shapes.push_back(lb_wheel);
        };

        void UpdatePosition()
        {
            Rotation rot = GetRotation();
            float z = moveSp * cos(rot.deg * M_PI/180);
            float x = moveSp * sin(rot.deg * M_PI/180);

            followDist = 1.5 + (moveSp * 1.6);

            followPoint->X = center.X - followDist * cos((-rot.deg - 90) * M_PI/180);
            followPoint->Z = center.Z - followDist * sin((-rot.deg - 90) * M_PI/180);

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
