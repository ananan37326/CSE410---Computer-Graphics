#include <bits/stdc++.h>
#include <GL/glut.h>

#define pi (2 * acos(0.0))
#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3

using namespace std;

class Object;
class Vector3D;
class PointLight;
class SpotLight;

extern vector<Object *> objects;       // from main file
extern Vector3D eye;                   // from main file
extern int recursionLevel;             // from main file
extern vector<PointLight> pointLights; // from main file
extern vector<SpotLight> spotLights;   // from main file

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

    void normalize()
    {
        double length = sqrt(x * x + y * y + z * z);
        x /= length;
        y /= length;
        z /= length;
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

double dot(Vector3D a, Vector3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3D cross(Vector3D a, Vector3D b)
{
    return Vector3D(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double distance(Vector3D a, Vector3D b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

void printVector(Vector3D v)
{
    cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
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

    void clip()
    {
        if (r > 1)
            r = 1;
        if (g > 1)
            g = 1;
        if (b > 1)
            b = 1;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
    }
};

color operator+(color a, color b)
{
    return color(a.r + b.r, a.g + b.g, a.b + b.b);
}

color operator*(color a, double b)
{
    return color(a.r * b, a.g * b, a.b * b);
}

color operator*(color a, color b)
{
    return color(a.r * b.r, a.g * b.g, a.b * b.b);
}

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

    void draw()
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(light_color.r, light_color.g, light_color.b);
        glVertex3f(light_pos.x, light_pos.y, light_pos.z);
        glEnd();
        glPointSize(1);
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

    void draw()
    {
        glPointSize(7);
        glBegin(GL_POINTS);
        glColor3f(point_light.light_color.r, point_light.light_color.g, point_light.light_color.b);
        glVertex3f(point_light.light_pos.x, point_light.light_pos.y, point_light.light_pos.z);
        glEnd();
        glPointSize(1);
    }
};

class Ray
{
public:
    Vector3D start;
    Vector3D dir;

    Ray()
    {
        start = Vector3D(0, 0, 0);
        dir = Vector3D(0, 0, 0);
    }

    Ray(Vector3D start, Vector3D dir)
    {
        this->start = start;
        this->dir = dir;
        this->dir.normalize();
    }

    ~Ray()
    {
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
    }

    virtual double getIntersectingPoint(Ray ray, color &color, int level)
    {
        return -1.0;
    }

    virtual Vector3D getNormal(Vector3D point)
    {
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

    virtual double intersect(Ray ray, color &outputColor, int level)
    {
        double tMin = getIntersectingPoint(ray, outputColor, level);

        if (level == 0)
        {
            return tMin;
        }

        // cout << "here" << endl;

        Vector3D intersectionPoint = ray.start + ray.dir * tMin;
        Vector3D normal = getNormal(intersectionPoint);
        // if (distance(eye, this->reference_point) > this->length)
        // {
        //     normal = normal * -1.0;
        // }

        // ambient
        outputColor = this->c * this->coEfficients[0];
        outputColor.clip();

        if(dot(ray.dir, normal) > 0)
        {
            //cout << "here" << endl;
            normal = normal * -1.0;
        }

        // diffuse and specular
        for (int i = 0; i < pointLights.size(); i++)
        {
            Ray rayL(pointLights[i].light_pos, intersectionPoint - pointLights[i].light_pos);
            double t, tMinShadow = INFINITY;
            for (int j = 0; j < objects.size(); j++)
            {
                color blackDummy(0, 0, 0);
                t = objects[j]->intersect(rayL, blackDummy, 0);
                if (t > 0 && t < tMinShadow)
                {
                    tMinShadow = t;
                }
            }

            Vector3D whereIntersectionPoint = rayL.start + rayL.dir * tMinShadow;
            double tuningMargin = 0.00001;
            double distIncidentIntersection = distance(intersectionPoint, rayL.start) - tuningMargin;
            double distIncidentShadow = distance(whereIntersectionPoint, rayL.start);

            if (distIncidentIntersection > distIncidentShadow)
            {
                // cout << "herer" << endl;
                continue;
            }

            double lambertValue = max(dot((rayL.dir), normal*-1.0), 0.0);
            Ray rayR(intersectionPoint, rayL.dir - normal * 2.0 * dot((rayL.dir), normal));
            double phongValue = max(dot(rayL.dir, rayR.dir*-1.0), 0.0);
            // cout << "phongValue: " << phongValue << endl;
            // cout << "lambertValue: " << lambertValue << endl;

            outputColor = outputColor + (pointLights[i].light_color * this->c) * (this->coEfficients[1] * lambertValue);
            outputColor.clip();
            outputColor = outputColor + (pointLights[i].light_color * this->c) * (this->coEfficients[2] * pow(phongValue, this->shine));
            outputColor.clip();
        }

        for (int i = 0; i < spotLights.size(); i++)
        {
            Vector3D lightPos = spotLights[i].point_light.light_pos;
            Vector3D lightDir = spotLights[i].light_direction;
            Ray rayL(lightPos, intersectionPoint - lightPos);
            lightDir.normalize();
            rayL.dir.normalize();

            double beta = acos(dot(rayL.dir, lightDir)) * 180 / pi;
            if (beta > spotLights[i].cutoff_angle)
            {
                continue;
            }

            double intensity = pow(dot(rayL.dir, lightDir), 2);
            outputColor = outputColor * intensity;
            outputColor.clip();

            double t, tMinShadow = INFINITY;
            for (int j = 0; j < objects.size(); j++)
            {
                color blackDummy(0, 0, 0);
                t = objects[j]->intersect(rayL, blackDummy, 0);
                if (t > 0 && t < tMinShadow)
                {
                    tMinShadow = t;
                }
            }
            Vector3D whereIntersectionPoint = rayL.start + rayL.dir * tMinShadow;
            double tuningMargin = 0.000001;
            double distIncidentIntersection = distance(intersectionPoint, rayL.start) - tuningMargin;
            double distIncidentShadow = distance(whereIntersectionPoint, rayL.start);
            if (distIncidentIntersection > distIncidentShadow)
            {
                continue;
            }

            double lambertValue = max(dot((rayL.dir), normal), 0.0);
            Ray rayR(intersectionPoint, rayL.dir - normal * 2.0 * -lambertValue);
            double phongValue = max(dot(rayL.dir, rayR.dir), 0.0);
            // cout << "phongValue: " << phongValue << endl;
            // cout << "lambertValue: " << lambertValue << endl;

            outputColor = outputColor + (spotLights[i].point_light.light_color * this->c) * (this->coEfficients[1] * lambertValue);
            outputColor.clip();
            outputColor = outputColor + (spotLights[i].point_light.light_color * this->c) * (this->coEfficients[2] * pow(phongValue, this->shine));
            outputColor.clip();
        }

        // reflection
        if (level >= recursionLevel)
        {
            return tMin;
        }

        Vector3D reflectionDir = ray.dir - normal * 2.0 * dot(ray.dir, normal);
        Ray rayReflected(intersectionPoint + reflectionDir, reflectionDir);

        int nearestRef = -1;
        double tMinReflected = INFINITY;
        color dummyRef(0, 0, 0);
        for (int i = 0; i < objects.size(); i++)
        {
            double t = objects[i]->intersect(rayReflected, dummyRef, 0);
            if (t > 0 && t < tMinReflected)
            {
                tMinReflected = t;
                nearestRef = i;
            }
        }

        color reflectedColor(0, 0, 0);
        if (nearestRef != -1)
        {
            objects[nearestRef]->intersect(rayReflected, reflectedColor, level + 1);
            outputColor = outputColor + (reflectedColor * this->coEfficients[3]);
            outputColor.clip();
        }

        return tMin;
    }
};

class Floor : public Object
{
public:
    double tileWidth;
    double numTiles;

    Floor()
    {
        height = width = length = 0;
        reference_point = Vector3D(0, 0, 0);
        coEfficients[0] = coEfficients[1] = coEfficients[2] = coEfficients[3] = 0;
        shine = 0;
        tileWidth = 0;
        numTiles = 0;
    }

    Floor(double floorWidth, double floorHeight, double tileWidth)
    {
        this->height = floorHeight;
        this->width = floorWidth;
        this->length = 0;
        this->reference_point = Vector3D(-width / 2, -width / 2, 0);
        this->coEfficients[0] = this->coEfficients[1] = this->coEfficients[2] = this->coEfficients[3] = 0;
        this->shine = 0;
        this->tileWidth = tileWidth;
        this->numTiles = width / tileWidth;
    }

    void draw()
    {
        for (int i = 0; i < (int)numTiles; i++)
        {
            for (int j = 0; j < (int)numTiles; j++)
            {
                glColor3f((i + j) % 2, (i + j) % 2, (i + j) % 2);

                glBegin(GL_QUADS);
                {
                    glVertex3f(reference_point.x + i * tileWidth, reference_point.y + j * tileWidth, reference_point.z);
                    glVertex3f(reference_point.x + i * tileWidth, reference_point.y + (j + 1) * tileWidth, reference_point.z);
                    glVertex3f(reference_point.x + (i + 1) * tileWidth, reference_point.y + (j + 1) * tileWidth, reference_point.z);
                    glVertex3f(reference_point.x + (i + 1) * tileWidth, reference_point.y + j * tileWidth, reference_point.z);
                }
                glEnd();
            }
        }
    }

    double intersect(Ray ray, color &outputColor, int level)
    {
        Vector3D normal = Vector3D(0, 0, 1);
        
        if (dot(eye, normal) <= 0 || dot(ray.dir, normal) > 0)
        {
            //cout << "Floor::intersect: ray is not facing the floor" << endl;
            normal = normal * -1.0;
        }
        normal.normalize();

        double tMin = INFINITY;
        if (dot(ray.dir, normal) != 0)
        {
            tMin = - (dot(ray.start, normal) / dot(ray.dir, normal));
        }

        if (tMin > 0 && tMin < INFINITY)
        {
            Vector3D intersection = ray.start + ray.dir * tMin;
            double topLeft = -width / 2.0;
            double bottomRight = width / 2.0;

            if (!(intersection.x > topLeft && intersection.x < bottomRight && intersection.y > topLeft && intersection.y < bottomRight))
            {
                tMin = INFINITY;
            }
        }
        // cout << "tMin: " << tMin << endl;
        if (level == 0)
        {
            return tMin;
        }

        Vector3D intersectionPoint = ray.start + ray.dir * tMin;
        Vector3D distIntersectionReference = intersectionPoint - reference_point;
        int x = distIntersectionReference.x / tileWidth;
        int y = distIntersectionReference.y / tileWidth;

        color thisColor((x + y) % 2, (x + y) % 2, (x + y) % 2);

        // ambient
        outputColor = thisColor * this->coEfficients[0];

        

        // diffuse and specular
        for (int i = 0; i < pointLights.size(); i++)
        {
            Ray rayL(pointLights[i].light_pos, intersectionPoint - pointLights[i].light_pos);
            double t, tMinShadow = INFINITY;
            for (int j = 0; j < objects.size(); j++)
            {
                color blackDummy(0, 0, 0);
                t = objects[j]->intersect(rayL, blackDummy, 0);
                if (t > 0 && t < tMinShadow)
                {
                    tMinShadow = t;
                }
            }

            Vector3D whereIntersectionPoint = rayL.start + rayL.dir * tMinShadow;
            double tuningMargin = 0.000001;
            double distIncidentIntersection = distance(intersectionPoint, rayL.start) - tuningMargin;
            double distIncidentShadow = distance(whereIntersectionPoint, rayL.start);

            if (distIncidentIntersection > distIncidentShadow)
            {
                // cout << "herer" << endl;
                continue;
            }

            double lambertValue = max(dot((rayL.dir * -1.0), normal), 0.0);
            Ray rayR(intersectionPoint, rayL.dir - normal * 2.0 * dot((rayL.dir), normal));
            double phongValue = max(dot(rayL.dir*-1.0, rayR.dir), 0.0);
            // cout << "phongValue: " << phongValue << endl;
            // cout << "lambertValue: " << lambertValue << endl;

            outputColor = outputColor + (pointLights[i].light_color * thisColor) * (this->coEfficients[1] * lambertValue);
            outputColor = outputColor + (pointLights[i].light_color * thisColor) * (this->coEfficients[2] * pow(phongValue, this->shine));
        }

        for (int i = 0; i < spotLights.size(); i++)
        {
            Vector3D lightPos = spotLights[i].point_light.light_pos;
            Vector3D lightDir = spotLights[i].light_direction;
            Ray rayL(lightPos, intersectionPoint - lightPos);
            lightDir.normalize();
            rayL.dir.normalize();

            double beta = acos(dot(rayL.dir, lightDir)) * 180 / pi;
            if (beta > spotLights[i].cutoff_angle)
            {
                continue;
            }

            double intensity = pow(dot(rayL.dir, lightDir), 2);
            outputColor = outputColor * intensity;

            double t, tMinShadow = INFINITY;
            for (int j = 0; j < objects.size(); j++)
            {
                color blackDummy(0, 0, 0);
                t = objects[j]->intersect(rayL, blackDummy, 0);
                if (t > 0 && t < tMinShadow)
                {
                    tMinShadow = t;
                }
            }
            Vector3D whereIntersectionPoint = rayL.start + rayL.dir * tMinShadow;
            double tuningMargin = 0.000001;
            double distIncidentIntersection = distance(intersectionPoint, rayL.start) - tuningMargin;
            double distIncidentShadow = distance(whereIntersectionPoint, rayL.start);
            if (distIncidentIntersection > distIncidentShadow)
            {
                continue;
            }

            double lambertValue = max(dot((rayL.dir * -1.0), normal), 0.0);
            Ray rayR(intersectionPoint, rayL.dir - normal * 2.0 * dot((rayL.dir), normal));
            double phongValue = max(dot(rayL.dir*-1.0, rayR.dir), 0.0);
            // cout << "phongValue: " << phongValue << endl;
            // cout << "lambertValue: " << lambertValue << endl;

            outputColor = outputColor + (spotLights[i].point_light.light_color * thisColor) * (this->coEfficients[1] * lambertValue);
            outputColor = outputColor + (spotLights[i].point_light.light_color * thisColor) * (this->coEfficients[2] * pow(phongValue, this->shine));
        }

        // reflection
        if (level >= recursionLevel)
        {
            return tMin;
        }

        Vector3D reflectionDir = ray.dir - normal * 2.0 * dot(ray.dir, normal);
        Ray rayReflected(intersectionPoint + reflectionDir, reflectionDir);

        int nearestRef = -1;
        double tMinReflected = INFINITY;
        color dummyRef(0, 0, 0);
        for (int i = 0; i < objects.size(); i++)
        {
            double t = objects[i]->intersect(rayReflected, dummyRef, 0);
            if (t > 0 && t < tMinReflected)
            {
                tMinReflected = t;
                nearestRef = i;
            }
        }

        color reflectedColor(0, 0, 0);
        //cout << "nearestRef: " << nearestRef << endl;
        if (nearestRef != -1)
        {
            objects[nearestRef]->intersect(rayReflected, reflectedColor, level + 1);
        }

        outputColor = outputColor + (reflectedColor * this->coEfficients[3]);
        outputColor.clip();

        return tMin;
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
        int stacks = 50;
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
            glColor3f(c.r, c.g, c.b);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    // upper hemisphere
                    glVertex3f(points[i][j].x + this->reference_point.x, points[i][j].y + this->reference_point.y, points[i][j].z + this->reference_point.z);
                    glVertex3f(points[i][j + 1].x + this->reference_point.x, points[i][j + 1].y + this->reference_point.y, points[i][j + 1].z + this->reference_point.z);
                    glVertex3f(points[i + 1][j + 1].x + this->reference_point.x, points[i + 1][j + 1].y + this->reference_point.y, points[i + 1][j + 1].z + this->reference_point.z);
                    glVertex3f(points[i + 1][j].x + this->reference_point.x, points[i + 1][j].y + this->reference_point.y, points[i + 1][j].z + this->reference_point.z);
                    // lower hemisphere
                    glVertex3f(points[i][j].x + this->reference_point.x, points[i][j].y + this->reference_point.y, -points[i][j].z + this->reference_point.z);
                    glVertex3f(points[i][j + 1].x + this->reference_point.x, points[i][j + 1].y + this->reference_point.y, -points[i][j + 1].z + this->reference_point.z);
                    glVertex3f(points[i + 1][j + 1].x + this->reference_point.x, points[i + 1][j + 1].y + this->reference_point.y, -points[i + 1][j + 1].z + this->reference_point.z);
                    glVertex3f(points[i + 1][j].x + this->reference_point.x, points[i + 1][j].y + this->reference_point.y, -points[i + 1][j].z + this->reference_point.z);
                }
                glEnd();
            }
        }
    }

    Vector3D getNormal(Vector3D point)
    {
        Vector3D normal = point - this->reference_point;
        normal.normalize();
        return normal;
    }

    double getIntersectingPoint(Ray ray, color &outputColor, int level)
    {
        Vector3D Ro = ray.start - this->reference_point;
        Vector3D Rd = ray.dir;

        double tMin = -1.0;

        double a = 1.0;
        double b = 2.0 * dot(Ro, Rd);
        double c = dot(Ro, Ro) - this->length * this->length;
        double d = b * b - 4 * a * c;

        if (d < 0)
        {
            // cout << "No intersection" << endl;

            return tMin;
        }
        else if (d == 0)
        {
            tMin = -b / (2 * a);
        }
        else
        {
            double t1 = max((-b + sqrt(d)) / (2 * a), 0.0);
            double t2 = max((-b - sqrt(d)) / (2 * a), 0.0);
            if(t1>0)
            {
                tMin = t1;
            }
            else{
                tMin = t2;
            }
        }

        return tMin;
    }
};

class Triangle : public Object
{
public:
    Vector3D vertexA, vertexB, vertexC;

    Triangle()
    {
        this->vertexA = Vector3D(0, 0, 0);
        this->vertexB = Vector3D(0, 0, 0);
        this->vertexC = Vector3D(0, 0, 0);
    }

    Triangle(Vector3D a, Vector3D b, Vector3D c)
    {
        this->vertexA = a;
        this->vertexB = b;
        this->vertexC = c;
    }

    void draw()
    {
        glBegin(GL_TRIANGLES);
        {
            glColor3f(this->c.r, this->c.g, this->c.b);
            glVertex3f(this->vertexA.x, this->vertexA.y, this->vertexA.z);
            glVertex3f(this->vertexB.x, this->vertexB.y, this->vertexB.z);
            glVertex3f(this->vertexC.x, this->vertexC.y, this->vertexC.z);
        }
        glEnd();
    }

    Vector3D getNormal(Vector3D point)
    {
        Vector3D normal = cross(this->vertexB - this->vertexA, this->vertexC - this->vertexA);
        normal.normalize();
        return normal;
    }

    double getIntersectingPoint(Ray ray, color &outputColor, int level)
    {
        double tMin = -1.0;
        Vector3D Ro = ray.start;
        Vector3D Rd = ray.dir;

        double detVal = (vertexA.x - vertexB.x) * ((vertexA.y - vertexC.y) * Rd.z - (vertexA.z - vertexC.z) * Rd.y);
        detVal += (vertexA.y - vertexB.y) * ((vertexA.z - vertexC.z) * Rd.x - (vertexA.x - vertexC.x) * Rd.z);
        detVal += (vertexA.z - vertexB.z) * ((vertexA.x - vertexC.x) * Rd.y - (vertexA.y - vertexC.y) * Rd.x);

        double betaVal = (vertexA.x - Ro.x) * ((vertexA.y - vertexC.y) * Rd.z - (vertexA.z - vertexC.z) * Rd.y);
        betaVal += (vertexA.y - Ro.y) * ((vertexA.z - vertexC.z) * Rd.x - (vertexA.x - vertexC.x) * Rd.z);
        betaVal += (vertexA.z - Ro.z) * ((vertexA.x - vertexC.x) * Rd.y - (vertexA.y - vertexC.y) * Rd.x);
        betaVal /= detVal;

        double gammaVal = (vertexA.x - vertexB.x) * ((vertexA.y - Ro.y) * Rd.z - (vertexA.z - Ro.z) * Rd.y);
        gammaVal += (vertexA.y - vertexB.y) * ((vertexA.z - Ro.z) * Rd.x - (vertexA.x - Ro.x) * Rd.z);
        gammaVal += (vertexA.z - vertexB.z) * ((vertexA.x - Ro.x) * Rd.y - (vertexA.y - Ro.y) * Rd.x);
        gammaVal /= detVal;

        double tVal = (vertexA.x - vertexB.x) * ((vertexA.y - vertexC.y) * (vertexA.z - Ro.z) - (vertexA.z - vertexC.z) * (vertexA.y - Ro.y));
        tVal += (vertexA.y - vertexB.y) * ((vertexA.z - vertexC.z) * (vertexA.x - Ro.x) - (vertexA.x - vertexC.x) * (vertexA.z - Ro.z));
        tVal += (vertexA.z - vertexB.z) * ((vertexA.x - vertexC.x) * (vertexA.y - Ro.y) - (vertexA.y - vertexC.y) * (vertexA.x - Ro.x));
        tVal /= detVal;

        if (detVal != 0)
        {
            if (betaVal > 0 && gammaVal > 0 && (betaVal + gammaVal < 1) && tVal > 0)
            {
                tMin = tVal;
            }
        }

        return tMin;
    }
};

class GenQuad : public Object
{
public:
    double aVal, bVal, cVal, dVal, eVal, fVal, gVal, hVal, iVal, jVal;

    GenQuad()
    {
        aVal = 0;
        bVal = 0;
        cVal = 0;
        dVal = 0;
        eVal = 0;
        fVal = 0;
        gVal = 0;
        hVal = 0;
        iVal = 0;
        jVal = 0;
    }

    GenQuad(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j)
    {
        aVal = a;
        bVal = b;
        cVal = c;
        dVal = d;
        eVal = e;
        fVal = f;
        gVal = g;
        hVal = h;
        iVal = i;
        jVal = j;
    }

    Vector3D getNormal(Vector3D point)
    {
        Vector3D normal(0, 0, 0);
        normal.x = 2 * aVal * point.x + dVal * point.y + eVal * point.z + gVal;
        normal.y = 2 * bVal * point.y + dVal * point.x + fVal * point.z + hVal;
        normal.z = 2 * cVal * point.z + eVal * point.x + fVal * point.y + iVal;
        normal.normalize();
        return normal;
    }

    double getIntersectingPoint(Ray ray, color &outputColor, int level)
    {
        Vector3D Ro = ray.start;
        Vector3D Rd = ray.dir;
        double tMin = INFINITY;
        double t1 = -1, t2 = -1;

        double aQuad = aVal * Rd.x * Rd.x + bVal * Rd.y * Rd.y + cVal * Rd.z * Rd.z + dVal * Rd.x * Rd.y + eVal * Rd.x * Rd.z + fVal * Rd.y * Rd.z;
        double bQuad = 2 * aVal * Ro.x * Rd.x + 2 * bVal * Ro.y * Rd.y + 2 * cVal * Ro.z * Rd.z + dVal * (Ro.x * Rd.y + Ro.y * Rd.x) + eVal * (Ro.x * Rd.z + Ro.z * Rd.x) + fVal * (Ro.y * Rd.z + Ro.z * Rd.y) + gVal * Rd.x + hVal * Rd.y + iVal * Rd.z;
        double cQuad = aVal * Ro.x * Ro.x + bVal * Ro.y * Ro.y + cVal * Ro.z * Ro.z + dVal * Ro.x * Ro.y + eVal * Ro.x * Ro.z + fVal * Ro.y * Ro.z + gVal * Ro.x + hVal * Ro.y + iVal * Ro.z + jVal;

        double discriminant = bQuad * bQuad - 4 * aQuad * cQuad;

        if (aQuad == 0)
        {
            if (bQuad != 0)
            {
                t1 = -cQuad / bQuad;
            }
        }

        else
        {
            if (discriminant > 0)
            {
                t1 = (-bQuad - sqrt(discriminant)) / (2 * aQuad);
                t2 = (-bQuad + sqrt(discriminant)) / (2 * aQuad);
            }
            else if (discriminant == 0)
            {
                t1 = -bQuad / (2 * aQuad);
            }
        }

        if (t1 != -1)
        {
            if (t2 != -1)
            {
                Vector3D intersectionPoint1 = Ro + Rd * t1;
                Vector3D intersectionPoint2 = Ro + Rd * t2;

                if ((length != 0 && (intersectionPoint1.x < reference_point.x || intersectionPoint1.x > reference_point.x + length)) || (width != 0 && (intersectionPoint1.y < reference_point.y || intersectionPoint1.y > reference_point.y + width)) || (height != 0 && (intersectionPoint1.z < reference_point.z || intersectionPoint1.z > reference_point.z + height)))
                {
                    t1 = -1;
                }
                if ((length != 0 && (intersectionPoint2.x < reference_point.x || intersectionPoint2.x > reference_point.x + length)) || (width != 0 && (intersectionPoint2.y < reference_point.y || intersectionPoint2.y > reference_point.y + width)) || (height != 0 && (intersectionPoint2.z < reference_point.z || intersectionPoint2.z > reference_point.z + height)))
                {
                    t2 = -1;
                }
            }
            else
            {
                Vector3D intersectionPoint = Ro + Rd * t1;
                if ((length != 0 && (intersectionPoint.x < reference_point.x || intersectionPoint.x > reference_point.x + length)) || (width != 0 && (intersectionPoint.y < reference_point.y || intersectionPoint.y > reference_point.y + width)) || (height != 0 && (intersectionPoint.z < reference_point.z || intersectionPoint.z > reference_point.z + height)))
                {
                    t1 = -1;
                }
            }
        }
        if (t1 != -1 && t2 != -1)
        {
            tMin = min(t1, t2);
        }
        else if (t1 != -1)
        {
            tMin = t1;
        }
        else if (t2 != -1)
        {
            tMin = t2;
        }

        return tMin;
    }
};