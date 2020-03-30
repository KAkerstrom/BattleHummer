# ifndef PROJECTION
# define PROJECTION

#include "Shapes.h"

class Projection
{
    public:
        virtual void Project() = 0;
};

class Ortho : public Projection
{
    public:
        Ortho (Point3d _center, Point3d _size)
        {
            Point3d s (_size.X/2, _size.Y/2, _size.Z/2);
            left    = _center.X - s.X;
            right   = _center.X + s.X;
            bottom  = _center.Y - s.Y;
            top     = _center.Y + s.Y;
            Near    = _center.Z - s.Z;
            Far     = _center.Z + s.Z;
        };

        Ortho (float _left, float _right, float _bottom, float _top, float _near, float _far)
        {
            left = _left;
            right = _right;
            bottom = _bottom;
            top = _top;
            Near = _near;
            Far = _far;
        };

        void Project()
        {
            glOrtho (left, right, bottom, top, Near, Far);
        }

        float left, right, bottom, top, Near, Far;
};

class Perspective : public Projection
{
    public:
        Perspective (float _fovy, float _aspect, float _near, float _far)
        {
            fovy = _fovy;
            aspect = _aspect;
            Near = _near;
            Far = _far;
        };

        void Project()
        {
            gluPerspective (fovy, aspect, Near, Far);
        }

        float fovy, aspect, Near, Far;
};

# endif