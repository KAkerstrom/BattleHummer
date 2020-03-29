#ifndef SHAPES
#define SHAPES

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include <vector>
#include <string>

//Colours
#define c_black 0
#define c_white 1
#define c_red 2
#define c_green 3
#define c_blue 4
#define c_cyan 5
#define c_maroon 6
#define c_purple 7
#define c_teal 8
#define c_yellow 9

// Cube sides
#define side_top 0
#define side_bottom 1
#define side_right 2
#define side_left 3
#define side_front 4
#define side_back 5

struct Point2d
{
   Point2d() { X = Y = 0; };
   Point2d(float x, float y) : X(x), Y(y){}
   float X, Y;
};

struct Point3d
{
   Point3d() { X = Y = Z = 0; };
   Point3d(float x, float y, float z) : X(x), Y(y), Z(z){}
   float X, Y, Z;
};

struct Rotation
{
    Rotation()
    {
        vec = Point3d(0, 1, 0);
        deg = 0;
    };
    Rotation(Point3d _vec, float _deg) : vec(_vec), deg(deg){}
    Point3d vec;
    float deg;
};

struct Color
{
   Color()
   {
      red = 0;
      green = 0;
      blue = 0;
      alpha = 0;
   };

   Color(int preset, float a)
   {
      switch (preset)
      {
         case c_black:  red = 0.0; green = 0.0; blue = 0.0; break;
         case c_white:  red = 1.0; green = 1.0; blue = 1.0; break;
         case c_red:    red = 1.0; green = 0.0; blue = 0.0; break;
         case c_green:  red = 0.0; green = 0.5; blue = 0.0; break;
         case c_blue:   red = 0.0; green = 0.0; blue = 1.0; break;
         case c_yellow: red = 1.0; green = 1.0; blue = 0.0; break;
         case c_cyan:   red = 0.0; green = 1.0; blue = 1.0; break;
         case c_maroon: red = 0.5; green = 0.0; blue = 0.0; break;
         case c_purple: red = 0.5; green = 0.0; blue = 0.5; break;
         case c_teal:   red = 0.0; green = 0.5; blue = 0.5; break;
         default:       red = 0.0; green = 0.0; blue = 0.0; break;
      }
      alpha = a;
   }

   Color(int r, int g, int b) : red((float)r/255), green((float)g/255), blue((float)b/255), alpha(0){}
   Color(float r, float g, float b) : red(r), green(g), blue(b), alpha(0){}

   Color(int r, int g, int b, int a) : red((float)r/255), green((float)g/255), blue((float)b/255), alpha((float)a/255){}
   Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a){}

   float red;
   float green;
   float blue;
   float alpha;
};

class Shape
{
    public:
        Shape(Point3d _center)
        {
            id = "";
            center = _center;
            rotation = Rotation(Point3d(0,1,0), 0);
        }

        Shape(std::string _id, Point3d _center)
        {
            id = _id;
            center = _center;
        }

        void Move(Point3d _translation)
        {
            center.X += _translation.X;
            center.Y += _translation.Y;
            center.Z += _translation.Z;
        }

        void SetCenter(Point3d _newCenter)
        {
            center = _newCenter;
        }

        void Rotate(float _deg) { rotation.deg += _deg; }

        void SetRotation (Rotation _rot) { rotation = _rot; }

        Rotation GetRotation()     { return rotation; }
        Point3d  GetCenter()       { return center;  }

        virtual void Draw(bool _drawOnly = false) = 0;

    protected:
        void Transform()
        {
            glTranslatef(center.X, center.Y, center.Z);
            glRotatef(rotation.deg, rotation.vec.X, rotation.vec.Y, rotation.vec.Z);
        }

      std::string id;
      Rotation rotation;
      Color color;
      Point3d normal;
      Point3d center;
};

class Rect2d : public Shape
{
   public:
        Rect2d(Color _color, Point3d _normal, Point3d _center, std::vector<Point3d> _points)
            : Shape(_center)
        {
            normal = _normal;
            color = _color;
            points = _points;
        };

        Rect2d(Color _color, Point3d _normal, std::vector<Point3d> _points)
            : Shape(Point3d(0,0,0))
        {
            normal = _normal;
            color = _color;
            points = _points;
        };

        void SetColor(Color _color) { color = _color; }

        Color GetColor() { return color; }

        void Draw(bool _drawOnly = false)
        {
            if (!_drawOnly)
            {
                Transform();
                glBegin(GL_QUADS);
            }
            glNormal3f(normal.X, normal.Y, normal.Z);
            glColor4f(color.red, color.blue, color.green, 1 - color.alpha);
            for (int i = 0; i < 4; i++)
                glVertex3f(points[i].X, points[i].Y, points[i].Z);
            if (!_drawOnly)
            {
                glEnd();
                //glPopMatrix();
            }
        }

   protected:
      Color color;
      Point3d normal;
      std::vector<Point3d> points;
};

class Rect3d : public Shape
{
    public:
        Rect3d(Point3d _center, Point3d _size, Color _color)
            : Shape(_center)
        {
            Point3d s(_size.X/2, _size.Y/2, _size.Z/2);
            
            std::vector<Point3d> p;
            p.push_back(Point3d( s.X,-s.Y, s.Z));
            p.push_back(Point3d(-s.X,-s.Y, s.Z));
            p.push_back(Point3d(-s.X,-s.Y,-s.Z));
            p.push_back(Point3d( s.X,-s.Y,-s.Z));
            rects.push_back (Rect2d (_color, Point3d( 0,-1, 0), std::vector<Point3d> (p))); // top

            p.clear();
            p.push_back(Point3d( s.X, s.Y, s.Z));
            p.push_back(Point3d(-s.X, s.Y, s.Z));
            p.push_back(Point3d(-s.X, s.Y,-s.Z));
            p.push_back(Point3d( s.X, s.Y,-s.Z));
            rects.push_back (Rect2d (_color, Point3d( 0, 1, 0), std::vector<Point3d> (p))); // bottom

            p.clear();
            p.push_back(Point3d( s.X,-s.Y,-s.Z));
            p.push_back(Point3d( s.X, s.Y,-s.Z));
            p.push_back(Point3d( s.X, s.Y, s.Z));
            p.push_back(Point3d( s.X,-s.Y, s.Z));
            rects.push_back (Rect2d (_color, Point3d( 1, 0, 0), std::vector<Point3d> (p))); // right

            p.clear();
            p.push_back(Point3d(-s.X,-s.Y,-s.Z));
            p.push_back(Point3d(-s.X,-s.Y, s.Z));
            p.push_back(Point3d(-s.X, s.Y, s.Z));
            p.push_back(Point3d(-s.X, s.Y,-s.Z));
            rects.push_back (Rect2d (_color, Point3d(-1, 0, 0), std::vector<Point3d> (p))); // left

            p.clear();
            p.push_back(Point3d(-s.X,-s.Y, s.Z));
            p.push_back(Point3d( s.X,-s.Y, s.Z));
            p.push_back(Point3d( s.X, s.Y, s.Z));
            p.push_back(Point3d(-s.X, s.Y, s.Z));
            rects.push_back (Rect2d (_color, Point3d( 0, 0, 1), std::vector<Point3d> (p))); // front

            p.clear();
            p.push_back(Point3d(-s.X,-s.Y,-s.Z));
            p.push_back(Point3d( s.X,-s.Y,-s.Z));
            p.push_back(Point3d( s.X, s.Y,-s.Z));
            p.push_back(Point3d(-s.X, s.Y,-s.Z));
            rects.push_back (Rect2d (_color, Point3d( 0, 0,-1), std::vector<Point3d> (p))); // back
        };

        Rect3d(Point3d _center, std::vector<Rect2d> _rects)
            : Shape(_center)
        {
            rects = _rects;
        };

        void SetColor(int face, Color _color)
        {
            rects[face].SetColor(_color);
        }

        void SetColor(Color _color)
        {
            for (int i = 0; i < 6; i++)
                rects[i].SetColor(_color);
        }

        void Draw(bool _drawOnly = false)
        {
            if (!_drawOnly)
            {
                glPushMatrix();
                Transform();
                glBegin(GL_QUADS);
            }
            for (int i = 0; i < 6; i++)
                rects[i].Draw(true);
            if (!_drawOnly)
            {
                glEnd();
                glPopMatrix();
            }
        }

    protected:
        std::vector<Rect2d> rects;
};

class Sphere : Shape
{
   public:
        Sphere (Point3d _center, float _radius, Color _color) : Shape(_center)
        {
            radius = _radius;
            color = _color;
        };

        void Draw(bool _drawOnly = false)
        {
            glPushMatrix();
            // rotate after translating
            glRotatef(rotation.deg, rotation.vec.X, rotation.vec.Y, rotation.vec.Z);
            glTranslatef(center.X, center.Y, center.Z);

            glColor4f(color.red, color.blue, color.green, 1 - color.alpha);
            glutSolidSphere(radius, 50, 50);

            glPopMatrix();
        }

        void SetColor(Color _color) { color = _color; }

        float radius;
        Color color;
};

class Pyramid : Shape
{
    public:
        Pyramid(Point3d _center, Point3d _size) : Shape(_center)
        {
            size = _size;
        };

        void Draw(bool _drawOnly = false)
        {
            glPushMatrix();
            glTranslatef(center.X, center.Y, center.Z);
            glRotatef(rotation.deg, rotation.vec.X, rotation.vec.Y, rotation.vec.Z);
            //glTranslatef(-center.X, -center.Y, -center.Z);

            float topY = size.Y/2;
            Point3d front_left ( -size.X/2, -size.Y/2,  -size.Z/2);
            Point3d front_right(  size.X/2, -size.Y/2,  -size.Z/2);
            Point3d back_left  ( -size.X/2, -size.Y/2,   size.Z/2);
            Point3d back_right (  size.X/2, -size.Y/2,   size.Z/2);

            glBegin(GL_TRIANGLES);

            // Back face
            glNormal3f( 0.0f, 0.0f, -1.0f); // facing back
            glColor4f(colors[side_back].red, colors[side_back].blue, colors[side_back].green, 1 - colors[side_back].alpha);
            glVertex3f(0, topY, 0);
            glVertex3f(back_left.X,  back_left.Y,  back_left.Z);
            glVertex3f(back_right.X, back_right.Y, back_right.Z);

            // Right face
            glNormal3f( 1.0f, 0.0f, 0.0f);
            glColor4f(colors[side_right].red, colors[side_right].blue, colors[side_right].green, 1 - colors[side_right].alpha);
            glVertex3f(0, topY, 0);
            glVertex3f(back_right.X, back_right.Y, back_right.Z);
            glVertex3f(front_right.X, front_right.Y, front_right.Z);

            // Front face
            glNormal3f( 0.0f, 0.0f, 1.0f);
            glColor4f(colors[side_front].red, colors[side_front].blue, colors[side_front].green, 1 - colors[side_front].alpha);
            glVertex3f(0, topY, 0);
            glVertex3f(front_right.X, front_right.Y, front_right.Z);
            glVertex3f(front_left.X,  front_left.Y,  front_left.Z);

            // Left Face
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glColor4f(colors[side_left].red, colors[side_left].blue, colors[side_left].green, 1 - colors[side_left].alpha);
            glVertex3f(0, topY, 0);
            glVertex3f(front_left.X,  front_left.Y,  front_left.Z);
            glVertex3f(back_left.X,  back_left.Y,  back_left.Z);

            glEnd();

            // Bottom Face
            glBegin(GL_QUADS);
            glNormal3f(0.0f, -1.0f, .0f);
            glColor4f(colors[side_bottom].red, colors[side_bottom].blue, colors[side_bottom].green, 1 - colors[side_bottom].alpha);
            glVertex3f(front_left.X,  front_left.Y,  front_left.Z);
            glVertex3f(front_right.X, front_right.Y, front_right.Z);
            glVertex3f(back_right.X, back_right.Y, back_right.Z);
            glVertex3f(back_left.X,  back_left.Y,  back_left.Z);
            glEnd();

            glPopMatrix();
        }

        void SetColor(int _side, Color _color) { colors[_side] = _color; }

        Point3d size;
        Color colors[5];
};

class Text : public Shape
{
    public:
        Text(Point3d _center, Color _color, std::string _text)
            : Shape(_center)
        {
            color = _color;
            text = _text;
        };

        void Draw(bool _drawOnly = false)
        {
            char buf[256];
            glColor4f(color.red, color.green, color.green, 1 - color.alpha);
            sprintf(buf, "%s", text.c_str());
            //glRasterPos2i(2,2);
            glRasterPos2i(center.X, center.Y);
            for(int i = 0;i < text.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buf[i]);
        }

    protected:
        Color color;
        std::string text;
};

#endif