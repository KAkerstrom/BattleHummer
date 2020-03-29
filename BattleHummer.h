#include "Shapes.h"
#include "Object.h"
#include <stdio.h>

class BattleHummer : public Object
{
    public:
        BattleHummer(Point3d _center) : Object(_center)
        {
            // Create body
            Rect3d* body = new Rect3d(Point3d(0,0,0), Point3d(2, 1, 3), Color(117, 50, 60));
            shapes.push_back(body);

            // Create rocket launchers
            Rotation rotRL(Point3d(1,0,0), 90);
            Cylinder* leftRL = new Cylinder(Point3d( 1.3f, 0.0f, -1.0f), 0.5f, 0.3f, Color(23, 117, 48));
            leftRL  -> SetRotation(rotRL);
            shapes.push_back(leftRL);
            Cylinder* rightRL = new Cylinder(Point3d(-1.3f, 0.0f, -1.0f), 0.5f, 0.3f, Color(23, 117, 48));
            rightRL -> SetRotation(rotRL);
            shapes.push_back(rightRL);
        };
};