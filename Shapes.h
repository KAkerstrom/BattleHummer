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
        Shape(Point3d _origin, Point3d _offset)
        {
            id = "";
            offset = _offset;
            rot_origin = Rotation(Point3d(0,1,0), 0);
            rot_offset = Rotation(Point3d(0,1,0), 0);
        }

        Shape(std::string _id, Point3d _origin, Point3d _offset)
        {
            id = _id;
            offset = _offset;
        }

        void MoveOrigin(Point3d _translation)
        {
            origin.X += _translation.X;
            origin.Y += _translation.Y;
            origin.Z += _translation.Z;
        }

        void SetOrigin(Point3d _newOrigin)
        {
            origin = _newOrigin;
        }

        void MoveOffset(Point3d _translation)
        {
            offset.X += _translation.X;
            offset.Y += _translation.Y;
            offset.Z += _translation.Z;
        }

        void SetOffset(Point3d _newOffset)
        {
            offset = _newOffset;
        }

        void Rotate(float _deg) { rot_origin.deg += _deg; }

        void SetRotOrigin (Rotation _rot) { rot_origin = _rot; }
        void SetRotOffset (Rotation _rot) { rot_offset = _rot; }

        Rotation GetOriginRot()    { return rot_origin; }
        Rotation GetOffsetRot()    { return rot_offset; }
        Point3d GetOrigin()        { return origin;  }
        Point3d GetOffset()        { return offset;  }

        virtual void Draw(bool _drawOnly = false) = 0;

    protected:
        void Transform()
        {
            glTranslatef(origin.X, origin.Y, origin.Z);
            glRotatef(rot_origin.deg, rot_origin.vec.X, rot_origin.vec.Y, rot_origin.vec.Z);
            glTranslatef(offset.X, offset.Y, offset.Z);
            glRotatef(rot_offset.deg, rot_offset.vec.X, rot_offset.vec.Y, rot_offset.vec.Z);
        }

      std::string id;
      Rotation rot_origin;
      Rotation rot_offset;
      Color color;
      Point3d normal;
      Point3d origin;
      Point3d offset; // "center" relative to origin
};

class Rect2d : public Shape
{
   public:
        Rect2d(Color _color, Point3d _normal, Point3d _origin, Point3d _offset, std::vector<Point3d> _points)
            : Shape(_origin, _offset)
        {
            normal = _normal;
            color = _color;
            points = _points;
        };

        Rect2d(Color _color, Point3d _normal, std::vector<Point3d> _points)
            : Shape(Point3d(), Point3d())
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
        Rect3d(Point3d _origin, Point3d _offset, Point3d _size, Color _color)
            : Shape(_origin, _offset)
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

        Rect3d(Point3d _origin, Point3d _offset, std::vector<Rect2d> _rects)
            : Shape(_origin, _offset)
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

class Text : public Shape
{
    public:
        Text(Point3d _origin, Point3d _offset, Color _color, std::string _text)
            : Shape(_origin, _offset)
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
            glRasterPos2i(origin.X, origin.Y);
            for(int i = 0;i < text.size(); i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, buf[i]);
        }

    protected:
        Color color;
        std::string text;
};

#endif