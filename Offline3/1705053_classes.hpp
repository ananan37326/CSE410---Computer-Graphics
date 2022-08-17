#include <bits/stdc++.h>

#define pi (2 * acos(0.0))

using namespace std;

// struct for denoting a point in 3D space
struct Vector3D
{
    double x, y, z;

    Vector3D()
    {
        x = y = z = 0;
    }

    Vector3D(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

// POINT ARITHMETICS
Vector3D operator+(Vector3D a, Vector3D b)
{
	return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3D operator-(Vector3D a, Vector3D b)
{
	return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3D operator*(Vector3D a, double b)
{
	return Vector3D(a.x * b, a.y * b, a.z * b);
}

// struct for defining a color in RGB
struct color
{
    double r, g, b;

    color()
    {
        r = g = b = 0;
    }

    color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class PointLight
{
    public:
    Vector3D light_pos;
    color light_color;

    PointLight()
    {
        light_pos = Vector3D(0, 0, 0);
        light_color = color(0, 0, 0);
    }
    
    PointLight(Vector3D light_pos, color light_color)
    {
        this->light_pos = light_pos;
        this->light_color = light_color;
    }
};

class SpotLight
{
    public:
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle;

    SpotLight()
    {
        point_light = PointLight(Vector3D(0, 0, 0), color(0, 0, 0));
        light_direction = Vector3D(0, 0, 0);
        cutoff_angle = 0;
    }

    SpotLight(PointLight point_light, Vector3D light_direction, double cutoff_angle)
    {
        this->point_light = point_light;
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
    }
};

class Object
{
public:
    Vector3D reference_point;
    double height;
    double width;
    double length;
    color c;
    double coEfficients[4]; // ambient, diffuse, specular, reflection
    int shine;              // exponent term for specular reflection

    Object()
    {
        reference_point = Vector3D(0, 0, 0);
        height = width = length = 0;
        coEfficients[0] = coEfficients[1] = coEfficients[2] = coEfficients[3] = 0;
        shine = 0;
    }

    Object(double height, double width, double length)
    {
        this->height = height;
        this->width = width;
        this->length = length;
        reference_point = Vector3D(0, 0, 0);
        coEfficients[0] = coEfficients[1] = coEfficients[2] = coEfficients[3] = 0;
        shine = 0;
    }

    void setReferencePoint(Vector3D reference_point)
    {
        this->reference_point = reference_point;
    }

    virtual void draw()
    {
        cout << "Drawing an object" << endl;
    }

    void setColor(color c)
    {
        this->c = c;
    }

    void setShine(int shine)
    {
        this->shine = shine;
    }

    void setCoefficients(double ambient, double diffuse, double specular, double reflection)
    {
        coEfficients[0] = ambient;
        coEfficients[1] = diffuse;
        coEfficients[2] = specular;
        coEfficients[3] = reflection;
    }
};

class Sphere : public Object
{
public:
    Sphere(Vector3D center, double radius)
    {
        this->reference_point = center;
        this->length = radius;
    }

    void draw()
    {
        int slices = 24;
        int stacks = 20;
        Vector3D points[100][100];
        int i, j;
        double h, r;
        // generate points
        for (i = 0; i <= stacks; i++)
        {
            h = this->length * sin(((double)i / (double)stacks) * (pi / 2));
            r = this->length * cos(((double)i / (double)stacks) * (pi / 2));
            for (j = 0; j <= slices; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
                points[i][j].z = h;
            }
        }
        // draw quads using generated points
        for (i = 0; i < stacks; i++)
        {
            glColor3f((double)i / (double)stacks, (double)i / (double)stacks, (double)i / (double)stacks);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    // upper hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                    // lower hemisphere
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                glEnd();
            }
        }
    }
};