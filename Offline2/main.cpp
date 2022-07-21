#include <bits/stdc++.h>

#define pi (2 * acos(0.0))

using namespace std;

// Struct for representing a point in 3D space
struct point
{
    double x, y, z;

    point()
    {
        x = y = z = 0;
    }

    point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct matrix
{
    double mat[4][4];
    matrix()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] = 0;
    }

    double getW()
    {
        return mat[3][3];
    }
};

ifstream fin;
stack<matrix> S;

// gluLookAt variables
point eye, look, up;
double eyeX, eyeY, eyeZ;
double lookX, lookY, lookZ;
double upX, upY, upZ;

// gluPerspective variables
double fovY;
double aspectRatio;
double near, far;

// Screen variables
double screenWidth, screenHeight;
double left, right, top, bottom, front, rear;

// zBuffer variables
double dx, dy;

// POINT ARITHMETICS
point operator+(point a, point b)
{
    return point(a.x + b.x, a.y + b.y, a.z + b.z);
}

point operator-(point a, point b)
{
    return point(a.x - b.x, a.y - b.y, a.z - b.z);
}

point operator*(point a, double b)
{
    return point(a.x * b, a.y * b, a.z * b);
}

point normalize(point a)
{
    double len = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    return point(a.x / len, a.y / len, a.z / len);
}

point scale(point a, double w)
{
    return point(a.x / w, a.y / w, a.z / w);
}

// VECTOR ARITHMETICS
double dot(point a, point b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

point cross(point a, point b)
{
    return point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

point rodrigues(point x, point a, double angle)
{
    return x * cos(angle * pi / 180) + a * dot(a, x) * (1 - cos(angle * pi / 180)) + cross(a, x) * sin(angle * pi / 180);
}

// MATRIX METHODS
matrix matmul(matrix a, matrix b)
{
    matrix c;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                c.mat[i][j] += a.mat[i][k] * b.mat[k][j];
    return c;
}

matrix getIdentityMatrix()
{
    matrix c;
    for (int i = 0; i < 4; i++)
    {
        c.mat[i][i] = 1;
    }
    return c;
}

matrix getTranslationMatrix(double tx, double ty, double tz)
{
    matrix c = getIdentityMatrix();
    c.mat[0][3] = tx;
    c.mat[1][3] = ty;
    c.mat[2][3] = tz;
    return c;
}

matrix getScalingMatrix(double sx, double sy, double sz)
{
    matrix c = getIdentityMatrix();
    c.mat[0][0] = sx;
    c.mat[1][1] = sy;
    c.mat[2][2] = sz;
    return c;
}

matrix getRotationMatrix(double angle, double ax, double ay, double az)
{
    point i(1, 0, 0);
    point j(0, 1, 0);
    point k(0, 0, 1);
    point a = normalize(point(ax, ay, az));

    matrix c = getIdentityMatrix();

    point c1 = rodrigues(i, a, angle);
    point c2 = rodrigues(j, a, angle);
    point c3 = rodrigues(k, a, angle);

    c.mat[0][0] = c1.x;
    c.mat[0][1] = c2.x;
    c.mat[0][2] = c3.x;
    c.mat[1][0] = c1.y;
    c.mat[1][1] = c2.y;
    c.mat[1][2] = c3.y;
    c.mat[2][0] = c1.z;
    c.mat[2][1] = c2.z;
    c.mat[2][2] = c3.z;

    return c;
}

matrix projectionMatrix()
{
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY * 0.5 * pi / 180);
    double r = near * tan(fovX * 0.5 * pi / 180);

    matrix c = getIdentityMatrix();

    c.mat[0][0] = near / r;
    c.mat[1][1] = near / t;
    c.mat[2][2] = -(far + near) / (far - near);
    c.mat[2][3] = -(2 * far * near) / (far - near);
    c.mat[3][2] = -1;

    return c;
}

void modelingTransformation()
{
    string command;
    matrix c = getIdentityMatrix();
    S.push(c);
    while(true)
    {
        fin >> command;

        if (command=="triangle")
        {

        }
        else if (command=="translate")
        {

        }
        else if (command=="scale")
        {

        }
        else if (command=="rotate")
        {

        }
        else if (command=="push")
        {

        }
        else if (command=="pop")
        {

        }
        else if (command=="end")
        {
            break;
        }

    }
}

void viewTransformation()
{
}

void initializegluLookAtParams()
{
    eye = point(eyeX, eyeY, eyeZ);
    look = point(lookX, lookY, lookZ);
    up = point(upX, upY, upZ);
}

void getSceneInfo()
{
    fin >> eyeX >> eyeY >> eyeZ;
    fin >> lookX >> lookY >> lookZ;
    fin >> upX >> upY >> upZ;
    fin >> fovY;
    fin >> aspectRatio;
    fin >> near >> far;
    initializegluLookAtParams();
}

void printParams()
{
    cout << "Eye: " << eyeX << " " << eyeY << " " << eyeZ << endl;
    cout << "Look: " << lookX << " " << lookY << " " << lookZ << endl;
    cout << "Up: " << upX << " " << upY << " " << upZ << endl;
    cout << "FOV: " << fovY << endl;
    cout << "Aspect Ratio: " << aspectRatio << endl;
    cout << "Near Plane: " << near << endl;
    cout << "Far Plane: " << far << endl;
    cout << "Eye: " << eye.x << " " << eye.y << " " << eye.z << endl;
    cout << "Look: " << look.x << " " << look.y << " " << look.z << endl;
    cout << "Up: " << up.x << " " << up.y << " " << up.z << endl;
}

int main()
{
    fin.open("scene.txt");
    if (!fin)
    {
        cout << "Error opening file" << endl;
        exit(1);
    }

    getSceneInfo();
    // printParams();
    modelingTransformation();

    fin.close();
    return 0;
}