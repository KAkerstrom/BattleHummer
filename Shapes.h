#ifndef SHAPES
#define SHAPES

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include <math.h>
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

   Color(int r, int g, int b) : red((float)r/255), green((float)b/255), blue((float)g/255), alpha(0){}
   Color(float r, float g, float b) : red(r), green(b), blue(g), alpha(0){}

   Color(int r, int g, int b, int a) : red((float)r/255), green((float)b/255), blue((float)g/255), alpha((float)a/255){}
   Color(float r, float g, float b, float a) : red(r), green(b), blue(g), alpha(a){}

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
            rotation = Rotation(Point3d(0,1,0), 0.0f);
        }

        Shape(std::string _id, Point3d _center)
        {
            id = _id;
            center = _center;
            rotation = Rotation(Point3d(0,1,0), 0);
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
                glPushMatrix();
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
                glPopMatrix();
            }
        }

   protected:
      Color color;
      Point3d normal;
      std::vector<Point3d> points;
};

class Circle : public Shape
{
   public:
        Circle(Point3d _center, float _radius, Color _color)
            : Shape(_center)
        {
            radius = _radius;
            normal = Point3d(0,1,0);
            color = _color;
        };

        void SetColor(Color _color) { color = _color; }

        Color GetColor() { return color; }

        void Draw(bool _drawOnly = false)
        {
            if (!_drawOnly)
            {
                glPushMatrix();
                Transform();
                glBegin(GL_POLYGON);
            }
            glNormal3f(normal.X, normal.Y, normal.Z);
            glColor4f(color.red, color.blue, color.green, 1 - color.alpha);
            for (float i = 0; i < 50; i++)
            {
                float theta = 2.0f * 3.14159f * i / 50.0f;
                glVertex3f(radius * cosf(theta), radius * sinf(theta), 0);
            }
            if (!_drawOnly)
            {
                glEnd();
                glPopMatrix();
            }
        }

   protected:
      Color color;
      float radius;
      Point3d normal;
};

class Rect3d : public Shape
{
    public:
        Rect3d(Point3d _center, Point3d _size, Color _color, float _top_ratio = 1.0f)
            : Shape(_center)
        {
            Point3d s (_size.X/2, _size.Y/2, _size.Z/2);

            // top/bottom front/back right/left points
            Point3d bfl(-s.X,-s.Y, s.Z);
            Point3d bfr( s.X,-s.Y, s.Z);
            Point3d bbl(-s.X,-s.Y,-s.Z);
            Point3d bbr( s.X,-s.Y,-s.Z);
            Point3d tfl(-s.X, s.Y, s.Z*_top_ratio);
            Point3d tfr( s.X, s.Y, s.Z*_top_ratio);
            Point3d tbl(-s.X, s.Y,-s.Z*_top_ratio);
            Point3d tbr( s.X, s.Y,-s.Z*_top_ratio);
            
            std::vector<Point3d> p;
            p.push_back(bfr);
            p.push_back(bbr);
            p.push_back(bbl);
            p.push_back(bfl);
            rects.push_back (Rect2d (_color, Point3d( 0,-1, 0), std::vector<Point3d> (p))); // bottom

            p.clear();
            p.push_back(tfr);
            p.push_back(tbr);
            p.push_back(tbl);
            p.push_back(tfl);
            rects.push_back (Rect2d (_color, Point3d( 0, 1, 0), std::vector<Point3d> (p))); // top

            p.clear();
            p.push_back(tfr);
            p.push_back(bfr);
            p.push_back(bbr);
            p.push_back(tbr);
            rects.push_back (Rect2d (_color, Point3d( 1, 0, 0), std::vector<Point3d> (p))); // right

            p.clear();
            p.push_back(tfl);
            p.push_back(tbl);
            p.push_back(bbl);
            p.push_back(bfl);
            rects.push_back (Rect2d (_color, Point3d(-1, 0, 0), std::vector<Point3d> (p))); // left

            p.clear();
            p.push_back(bfl);
            p.push_back(bfr);
            p.push_back(tfr);
            p.push_back(tfl);
            rects.push_back (Rect2d (_color, Point3d( 0, 0, 1), std::vector<Point3d> (p))); // front

            p.clear();
            p.push_back(bbl);
            p.push_back(tbl);
            p.push_back(tbr);
            p.push_back(bbr);
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

class Sphere : public Shape
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
            Transform();

            glColor4f(color.red, color.blue, color.green, 1 - color.alpha);
            glutSolidSphere(radius, 50, 50);

            glPopMatrix();
        }

        void SetColor(Color _color) { color = _color; }

        float radius;
        Color color;
};

class Cylinder : public Shape
{
   public:
        Cylinder (Point3d _center, float _height, float _radius, Color _cylinderColor, Color _endColor) : Shape(_center)
        {
            height = _height;
            radius = _radius;
            cylinderColor = _cylinderColor;
            endColor = _endColor;
        };

        void Draw(bool _drawOnly = false)
        {
            glPushMatrix();
            Transform();

            glColor4f(cylinderColor.red, cylinderColor.blue, cylinderColor.green, 1 - cylinderColor.alpha);
            gluCylinder(gluNewQuadric(), radius, radius, height, 50, 50);
            Circle c1(Point3d(0, 0, 0), radius, endColor);
            Circle c2(Point3d(0, 0, height), radius, endColor);
            c1.Rotate(180);
            c1.Draw();
            c2.Draw();

            glPopMatrix();
        }

        void SetColor(Color _color) { color = _color; }

        float height;
        float radius;
        Color cylinderColor;
        Color endColor;
};

class Pyramid : public Shape
{
    public:
        Pyramid(Point3d _center, Point3d _size) : Shape(_center)
        {
            size = _size;
        };

        void Draw(bool _drawOnly = false)
        {
            glPushMatrix();
            Transform();

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

class Teapot : public Shape
{
   public:
        Teapot (Point3d _center, float _size, Color _color) : Shape(_center)
        {
            size = _size;
            color = _color;
        };

        void Draw(bool _drawOnly = false)
        {
            glPushMatrix();
            Transform();

            glColor4f(color.red, color.blue, color.green, 1 - color.alpha);
            glutSolidTeapot(size);

            glPopMatrix();
        }

        void SetColor(Color _color) { color = _color; }

        float size;
        Color color;
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