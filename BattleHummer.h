#ifndef BATTLE_HUMMER
#define BATTLE_HUMMER

#include "Shapes.h"
#include "Object.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

enum FR {forward, reverse, neutral};
enum LR {left, right, straight};

class BattleHummer : public Object
{
    private:
        FR fr = neutral;
        LR lr = straight;

    public:
        std::string plateText = "PAWKK";
        Shape* antenna;
        Circle* brake1;
        Circle* brake2;

        Point3d originalCenter;
        Point3d* followPoint;
        Point3d* aheadPoint;
        bool rotR, rotL, throttle, brake;
        double moveSp, followDist, fDist = 1.5;
        double rotSp = 4.5f;
        double maxSpeed = 0.25;
        double minSpeed = -0.09;
        double accel = 0.03;

        BattleHummer(Point3d _center, float _size = 1.0f) : Object(_center)
        {
            originalCenter = _center;
            followPoint = new Point3d(0, 1.3, 0);
            aheadPoint = new Point3d(0, 1.3, 0);
            moveSp = 0;

            rotR = rotL = throttle = brake = false;

            Point3d bodySize(1.5 * _size, 1 * _size, 3 * _size);

            // Create rocket launchers
            Color endColor(200, 5, 5);
            Color cylinderColor(100, 5, 5);
            Cylinder *leftRL = new Cylinder
            (
                Point3d(0.95f * _size, 0.0f * _size, -1.0f * _size),
                0.55f * _size,
                0.2f * _size,
                cylinderColor,
                endColor
            );
            shapes.push_back(leftRL);
            Cylinder *rightRL = new Cylinder
            (
                Point3d(-0.95f * _size, 0.0f * _size, -1.0f * _size),
                0.55f * _size,
                0.2f * _size,
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
                Point3d(bodySize.X*0.9f, 0.8f*bodySize.Y, 0.5*bodySize.Z),
                Color(0.0f, 0.3f, 0.05f),
                0.5f
            );
            shapes.push_back(cab);

            // Create front window
            float fw_btmZ = -1.155f*_size;
            float fw_topZ = -0.95f*_size;
            float fw_topY = 1.1f*_size;
            float fw_btmY = 0.6f*_size;
            float fw_X = 0.5f*_size;
            std::vector<Point3d> p;
            p.push_back(Point3d(-fw_X,fw_topY,fw_topZ)); // top-left (from driver's perspective)
            p.push_back(Point3d( fw_X,fw_topY,fw_topZ)); // top-right
            p.push_back(Point3d( fw_X,fw_btmY,fw_btmZ)); // bottom-right
            p.push_back(Point3d(-fw_X,fw_btmY,fw_btmZ)); // bottom-left
            Rect2d* frontWindow = new Rect2d
            (
                Color(c_cyan, 0),
                Point3d(0,0,1),
                std::vector<Point3d>(p)
            );
            shapes.push_back(frontWindow);

            // Create Back window
            fw_btmZ = 0.155f*_size;
            fw_topZ = -0.05f*_size;
            p.clear();
            p.push_back(Point3d(-fw_X,fw_topY,fw_topZ)); // top-left (from driver's perspective)
            p.push_back(Point3d(-fw_X,fw_btmY,fw_btmZ)); // bottom-left
            p.push_back(Point3d( fw_X,fw_btmY,fw_btmZ)); // bottom-right
            p.push_back(Point3d( fw_X,fw_topY,fw_topZ)); // top-right
            Rect2d* backWindow = new Rect2d
            (
                Color(c_cyan, 0),
                Point3d(0,0,1),
                std::vector<Point3d>(p)
            );
            shapes.push_back(backWindow);

            fw_btmY = 0.65f*_size;
            fw_topY = 1.0f*_size;
            p.clear();
            p.push_back(Point3d(-0.7f*_size,fw_topY,-0.2f*_size));
            p.push_back(Point3d(-0.7f*_size,fw_topY,-0.75f*_size));
            p.push_back(Point3d(-0.7f*_size,fw_btmY,-0.85f*_size));
            p.push_back(Point3d(-0.7f*_size,fw_btmY,-0.1f*_size));
            Rect2d* leftWindow = new Rect2d
            (
                Color(c_cyan, 0),
                Point3d(0,0,1),
                std::vector<Point3d>(p)
            );
            shapes.push_back(leftWindow);

            p.clear();
            p.push_back(Point3d(0.7f*_size,fw_topY,-0.2f*_size));
            p.push_back(Point3d(0.7f*_size,fw_btmY,-0.1f*_size));
            p.push_back(Point3d(0.7f*_size,fw_btmY,-0.85f*_size));
            p.push_back(Point3d(0.7f*_size,fw_topY,-0.75f*_size));
            Rect2d* rightWindow = new Rect2d
            (
                Color(c_cyan, 0),
                Point3d(0,0,1),
                std::vector<Point3d>(p)
            );
            shapes.push_back(rightWindow);

            // Create headlights
            Circle* leftHeadlight = new Circle
            (
                Point3d(-0.4f*_size, 0.2f*_size, -1.55f*_size),
                0.15f*_size,
                Color(255, 255, 0)
            );
            leftHeadlight -> Rotate(180);
            shapes.push_back(leftHeadlight);

            Circle* rightHeadlight = new Circle
            (
                Point3d(0.4f*_size, 0.2f*_size, -1.55f*_size),
                0.15f*_size,
                Color(255, 255, 0)
            );
            rightHeadlight -> Rotate(180);
            shapes.push_back(rightHeadlight);

            // Create brakelights
            Circle* leftBrakelight = new Circle
            (
                Point3d(0.4f*_size, 0.2f*_size, 1.55f*_size),
                0.15f*_size,
                Color(100, 0, 0)
            );
            shapes.push_back(leftBrakelight);
            brake1 = leftBrakelight;

            Circle* rightBrakelight = new Circle
            (
                Point3d(-0.4f*_size, 0.2f*_size, 1.55f*_size),
                0.15f*_size,
                Color(100, 0, 0)
            );
            shapes.push_back(rightBrakelight);
            brake2 = rightBrakelight;

            // Create front grill
            p.clear();
            p.push_back(Point3d( 0.25f*_size, -0.1f*_size, -1.55f*_size));
            p.push_back(Point3d( 0.25f*_size, -0.35f*_size, -1.55f*_size));
            p.push_back(Point3d(-0.25f*_size, -0.35f*_size, -1.55f*_size));
            p.push_back(Point3d(-0.25f*_size, -0.1f*_size, -1.55f*_size));
            Rect2d* grillPlate = new Rect2d
            (
                Color(c_teal, 0),
                Point3d(0,0,1),
                std::vector<Point3d>(p)
            );
            shapes.push_back(grillPlate);

            Color barColor(120,50,168);
            for (int i = 0; i < 4; i++)
            {
                // Create grill bars
                Cylinder* bar = new Cylinder
                (
                    Point3d((-0.15f + i*0.1f)*_size, -0.15f*_size, -1.54f*_size),
                    0.2f*_size,
                    0.03f*_size,
                    barColor,
                    barColor
                );
                bar -> SetRotation(Rotation(Point3d(1,0,0), 0));
                bar -> Rotate(90);
                shapes.push_back(bar);
            }

            // Create license plate
            p.clear();
            p.push_back(Point3d( 0.25f*_size, -0.1f*_size, 1.55f*_size));
            p.push_back(Point3d(-0.25f*_size, -0.1f*_size, 1.55f*_size));
            p.push_back(Point3d(-0.25f*_size,-0.35f*_size, 1.55f*_size));
            p.push_back(Point3d( 0.25f*_size,-0.35f*_size, 1.55f*_size));
            Rect2d* license = new Rect2d
            (
                Color(c_teal, 0),
                Point3d(0,0,1),
                std::vector<Point3d>(p)
            );
            shapes.push_back(license);

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
            Teapot* teapot = new Teapot(Point3d(0,1*_size,1*_size), 0.2f*_size, Color(120,50,168));
            shapes.push_back(teapot);
            antenna = teapot;

            // Create body
            Rect3d *body = new Rect3d(Point3d(0,0,0), bodySize, Color(0.0f, 0.1f, 0.05f));
            shapes.push_back(body);
            
            // Create text
            Text* text = new Text
            (
                Point3d( -0.2f*_size, -0.25f*_size, 1.56f*_size),
                Color(50,50,50),
                1.0f*_size,
                plateText
            );
            shapes.push_back(text);
        };

        void ResetPos()
        {
            center = originalCenter;
        }

        void SetFDist(double Dist)
        {
            fDist = Dist;
        }

        void Drive(FR dir)
        {
            fr = dir;
        }

        void Steer(LR dir)
        {
            lr = dir;
        }

        void UpdatePosition()
        {
            antenna -> Rotate(2);

            Rotation rot = GetRotation();
            float deltaX = moveSp * sin(rot.deg * M_PI/180);
            float deltaZ = moveSp * cos(rot.deg * M_PI/180);
            float pot_x = center.X - deltaX;
            float pot_z = center.Z - deltaZ;

            // todo: move to rounded value on collision
            float blockCtoC = 6.0f;
            float blockSize = 1.9f;
            float x_col_dist = abs(pot_x - (blockCtoC * round(pot_x / blockCtoC)));
            float z_col_dist = abs(pot_z - (blockCtoC * round(pot_z / blockCtoC)));
            if (x_col_dist <= blockSize && z_col_dist <= blockSize)
                {deltaX = deltaZ = 0;}

            followDist = fDist + (moveSp * 1.6);

            followPoint->X = center.X - followDist * cos((-rot.deg - 90) * M_PI/180);
            followPoint->Z = center.Z - followDist * sin((-rot.deg - 90) * M_PI/180);
            aheadPoint->X  = center.X + followDist * cos((-rot.deg - 90) * M_PI/180);
            aheadPoint->Z  = center.Z + followDist * sin((-rot.deg - 90) * M_PI/180);
            Move(Point3d(-deltaX, 0, -deltaZ));
            Color brakeColor;
            double adjRotSp = rotSp;
            switch (fr)
            {
                case forward:
                    brakeColor = Color(100,0,0);
                    if(moveSp < maxSpeed)
                        moveSp += accel;

                    adjRotSp = rotSp;
                    break;
                case reverse:
                    brakeColor = Color(255,0,0);
                    if(moveSp > minSpeed)
                        moveSp -= accel;

                    adjRotSp = -rotSp;
                    break;
                case neutral:
                    brakeColor = Color(100,0,0);
                    if(moveSp < 0)
                        moveSp += accel;
                    if(moveSp > 0)
                        moveSp -= accel;
                    if (moveSp < 0 && moveSp > -accel)
                        moveSp = 0;
                    if (moveSp > 0 && moveSp < accel)
                        moveSp = 0;

                    adjRotSp = rotSp;
                    break;
                default:
                    brakeColor = Color(255,255,0);
                    // ¯\_(ツ)_/¯
                    break;
            }

            switch (lr)
            {
                case left:
                    Rotate(adjRotSp);
                    break;
                case right:
                    Rotate(-adjRotSp);
                    break;
                case neutral:
                    break;
                default:
                    brakeColor = Color(255,255,0);
                    // ¯\_(ツ)_/¯
                    break;
            }

            brake1 -> SetColor(brakeColor);
            brake2 -> SetColor(brakeColor);

        };
};

#endif