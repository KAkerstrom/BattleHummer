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
            Cylinder* leftRL = new Cylinder(Point3d( 1.3f, 0.0f, -1.0f), 0.5f, 0.3f, Color(23, 117, 48));
            shapes.push_back(leftRL);
            Cylinder* rightRL = new Cylinder(Point3d(-1.3f, 0.0f, -1.0f), 0.5f, 0.3f, Color(23, 117, 48));
            shapes.push_back(rightRL);

            // Create wheels
            Cylinder* rf_wheel = new Cylinder(Point3d(-1.1f, -0.7f, -1.0f), 0.3f, 0.3f, Color(23, 117, 48));
            rf_wheel -> Rotate(90);
            shapes.push_back(rf_wheel);
            Cylinder* lf_wheel = new Cylinder(Point3d(0.8f, -0.7f, -1.0f), 0.3f, 0.3f, Color(23, 117, 48));
            lf_wheel -> Rotate(90);
            shapes.push_back(lf_wheel);
            Cylinder* rb_wheel = new Cylinder(Point3d(-1.1f, -0.7f, 1.0f), 0.3f, 0.3f, Color(23, 117, 48));
            rb_wheel -> Rotate(90);
            shapes.push_back(rb_wheel);
            Cylinder* lb_wheel = new Cylinder(Point3d(0.8f, -0.7f, 1.0f), 0.3f, 0.3f, Color(23, 117, 48));
            lb_wheel -> Rotate(90);
            shapes.push_back(lb_wheel);
        };
};