#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include "Shapes.h"
#include <vector>
#include <string>

class Object : public Shape
{
    public:
        Object(Point3d _center) : Shape(_center){};

        void Draw(bool _drawOnly = false)
        {

            if (!_drawOnly)
            {
                glPushMatrix();
                Transform();
            }
            for (int i = 0; i < shapes.size(); i++)
                shapes[i] -> Draw();
            if (!_drawOnly)
                glPopMatrix();
        }

    protected:
        std::vector<Shape*> shapes;
};