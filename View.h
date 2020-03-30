# ifndef VIEW
# define VIEW

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include <vector>
#include "Shapes.h"
#include "Projection.h"

class Camera
{
    public:
        Camera(Point3d _eye, Point3d _lookAt)
        {
            eye = _eye;
            lookAt = _lookAt;
            up = Point3d(0, 1, 0);
        }

        Camera(Point3d _eye, Point3d _lookAt, Point3d _up)
        {
            eye = _eye;
            lookAt = _lookAt;
            up = _up;
        }

        void MoveLookAt(Point3d _move)
        {
            lookAt.X += _move.X;
            lookAt.Y += _move.Y;
            lookAt.Z += _move.Z;
        }

        void MoveEye(Point3d _move)
        {
            eye.X += _move.X;
            eye.Y += _move.Y;
            eye.Z += _move.Z;
        }

        void SetLookAt(Point3d _lookAt) { lookAt = _lookAt; }
        void SetEye(Point3d _eye) { eye = _eye; }
        void SetUp(Point3d _up) { up = _up; }

        void LookAt()
        {
            gluLookAt (eye.X, eye.Y, eye.Z,
                lookAt.X, lookAt.Y, lookAt.Z,
                up.X, up.Y, up.Z);
        }

        Point3d eye;
        Point3d lookAt;
        Point3d up;
};

class View
{
    public:
        View(Point2d _btm_left, Point2d _size, Projection* _projection, Camera* _camera)
        {
            btm_left = _btm_left;
            size = _size;
            projection = _projection;
            camera = _camera;
        }

        void AddShape(Shape* _shape)
        {
            shapes.push_back(_shape);
        }

        void SetPosition(Point2d _position)
        {
            btm_left = _position;
        }

        void SetSize(Point2d _size)
        {
            size = _size;
        }

        bool PointInViewport(Point2d _point)
        {
            return (_point.X > btm_left.X
            && _point.X < btm_left.X + size.X
            && _point.Y > btm_left.Y
            && _point.Y < btm_left.Y + size.Y);
        }

        void Draw()
        {
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glViewport(btm_left.X, btm_left.Y, size.X, size.Y);
            if (projection != 0)
                projection->Project();

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            if (camera != 0)
                camera->LookAt();
            for (int i = 0; i < shapes.size(); i++)
            {
                Shape* s = shapes[i];
                s -> Draw();
            }

            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
        }

    //protected:
        Point2d btm_left;
        Point2d size;
        Projection* projection;
        Camera* camera;
        std::vector<Shape*> shapes;
};

# endif