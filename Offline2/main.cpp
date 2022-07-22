#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <stack>

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
    int nRows=4, nCols=4;
    double **mat;
    matrix()
    {
        mat = new double*[nRows];
        for (int i = 0; i < nRows; i++)
        {
            mat[i] = new double[nCols];
        }
        for (int i = 0; i < nRows; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                mat[i][j] = 0;
            }
        }
    }

    matrix(int nRows, int nCols)
    {
        this->nRows = nRows;
        this->nCols = nCols;
        mat = new double*[nRows];
        for (int i = 0; i < nRows; i++)
        {
            mat[i] = new double[nCols];
        }
        for (int i = 0; i < nRows; i++)
        {
            for (int j = 0; j < nCols; j++)
            {
                mat[i][j] = 0;
            }
        }
    }

};

struct triangle
{
    point a,b,c;

    triangle()
    {
        a = b = c = point(0,0,0);    
    }

    triangle(point a, point b, point c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
};

ifstream fin;
ofstream stage1, stage2, stage3, z_buffer;

stack<matrix> S;
stack<stack<matrix>> states;
matrix viewMatrix(4,4), projectionMatrix(4,4);

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
    point first = x * cos(angle * pi / 180.0);
    point second = cross(a, x) * sin(angle * pi / 180.0);
    point third = a * dot(a, x) * (1 - cos(angle * pi / 180.0));
    return x * cos(angle * pi / 180.0) + a * (dot(a, x) * (1 - cos(angle * pi / 180.0))) + cross(a, x) * sin(angle * pi / 180.0);
}

point scaleW(matrix m)
{
    double w = m.mat[m.nRows - 1][m.nCols - 1];
    //cout << "W: " << w << endl;
    return point(m.mat[0][0] / w, m.mat[1][0] / w, m.mat[2][0] / w);
}


// MATRIX METHODS
matrix matmul(matrix a, matrix b)
{
    matrix c(a.nRows, b.nCols);
    for (int i = 0; i < a.nRows; i++)
        for (int j = 0; j < b.nCols; j++)
            for (int k = 0; k < a.nCols; k++)
                c.mat[i][j] += a.mat[i][k] * b.mat[k][j];
    return c;
}

matrix pointToMatrix(point a)
{
    matrix c(4,1);
    c.mat[0][0] = a.x;
    c.mat[1][0] = a.y;
    c.mat[2][0] = a.z;
    c.mat[3][0] = 1;
    return c;

}

matrix getIdentityMatrix()
{
    matrix c(4,4);
    for (int i = 0; i < c.nRows; i++)
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

matrix getScaleMatrix(double sx, double sy, double sz)
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
    //cout << "a: " << a.x << " " << a.y << " " << a.z << endl;

    matrix c = getIdentityMatrix();

    point c1 = rodrigues(i, a, angle);
    point c2 = rodrigues(j, a, angle);
    point c3 = rodrigues(k, a, angle);

    // cout << "c1: " << c1.x << " " << c1.y << " " << c1.z << endl;
    // cout << "c2: " << c2.x << " " << c2.y << " " << c2.z << endl;
    // cout << "c3: " << c3.x << " " << c3.y << " " << c3.z << endl;

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

matrix getViewMatrix()
{
    point l = normalize(look - eye);
    point r = normalize(cross(l, up));
    point u = normalize(cross(r, l));

    matrix T = getTranslationMatrix(-eye.x, -eye.y, -eye.z);
    matrix R = getIdentityMatrix();
    R.mat[0][0] = r.x;
    R.mat[0][1] = r.y;
    R.mat[0][2] = r.z;
    R.mat[1][0] = u.x;
    R.mat[1][1] = u.y;
    R.mat[1][2] = u.z;
    R.mat[2][0] = -l.x;
    R.mat[2][1] = -l.y;
    R.mat[2][2] = -l.z;

    return matmul(R, T);
}

matrix getProjectionMatrix()
{
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY  * pi / (180.0*2));
    double r = near * tan(fovX *  pi / (180.0*2));
    

    matrix c = getIdentityMatrix();

    c.mat[0][0] = near / r;
    c.mat[1][1] = near / t;
    c.mat[2][2] = -(far + near) / (far - near);
    c.mat[2][3] = -(2 * far * near) / (far - near);
    c.mat[3][2] = -1;
    c.mat[3][3] = 0;


    return c;
}

void printTriangle(triangle t)
{
    cout << "Triangle: " << endl;
    cout << "A: " << t.a.x << " " << t.a.y << " " << t.a.z << endl;
    cout << "B: " << t.b.x << " " << t.b.y << " " << t.b.z << endl;
    cout << "C: " << t.c.x << " " << t.c.y << " " << t.c.z << endl;
}

void printMatrix(matrix m)
{
    cout << fixed << setprecision(7);
    for (int i = 0; i < m.nRows; i++)
    {
        for (int j = 0; j < m.nCols; j++)
        {
            cout << m.mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void executeStages()
{
    string command;
    matrix c = getIdentityMatrix();
    S.push(c);
    while(true)
    {
        fin >> command;

        if (command=="triangle")
        {
            point a, b, c;
            fin >> a.x >> a.y >> a.z;
            fin >> b.x >> b.y >> b.z;
            fin >> c.x >> c.y >> c.z;
            triangle t(a, b, c);
            
            
            // Transform the triangle for stage1
            t.a = scaleW(matmul(S.top(), pointToMatrix(t.a)));
            //cout << "A: " << t.a.x << " " << t.a.y << " " << t.a.z << endl;
            t.b = scaleW(matmul(S.top(), pointToMatrix(t.b)));
            t.c = scaleW(matmul(S.top(), pointToMatrix(t.c)));

            stage1 << t.a.x << " " << t.a.y << " " << t.a.z << endl;
            stage1 << t.b.x << " " << t.b.y << " " << t.b.z << endl;
            stage1 << t.c.x << " " << t.c.y << " " << t.c.z << endl;
            stage1 << endl;

            // Stage2
            t.a = scaleW(matmul(viewMatrix, pointToMatrix(t.a)));
            t.b = scaleW(matmul(viewMatrix, pointToMatrix(t.b)));
            t.c = scaleW(matmul(viewMatrix, pointToMatrix(t.c)));

            stage2 << t.a.x << " " << t.a.y << " " << t.a.z << endl;
            stage2 << t.b.x << " " << t.b.y << " " << t.b.z << endl;
            stage2 << t.c.x << " " << t.c.y << " " << t.c.z << endl;
            stage2 << endl;

            cout << fixed << setprecision(7);
            cout << "A: " << t.a.x << " " << t.a.y << " " << t.a.z << endl;
            cout << "B: " << t.b.x << " " << t.b.y << " " << t.b.z << endl;
            cout << "C: " << t.c.x << " " << t.c.y << " " << t.c.z << endl;

            // Stage3
            t.a = scaleW(matmul(projectionMatrix, pointToMatrix(t.a)));
            t.b = scaleW(matmul(projectionMatrix, pointToMatrix(t.b)));
            t.c = scaleW(matmul(projectionMatrix, pointToMatrix(t.c)));

            stage3 << t.a.x << " " << t.a.y << " " << t.a.z << endl;
            stage3 << t.b.x << " " << t.b.y << " " << t.b.z << endl;
            stage3 << t.c.x << " " << t.c.y << " " << t.c.z << endl;
            stage3 << endl;

        }
        else if (command=="translate")
        {
            double tx, ty, tz;
            fin >> tx >> ty >> tz;
            S.push(matmul(S.top(),getTranslationMatrix(tx, ty, tz)));

        }
        else if (command=="scale")
        {
            double sx, sy, sz;
            fin >> sx >> sy >> sz;
            S.push(matmul(S.top(),getScaleMatrix(sx, sy, sz)));
        }
        else if (command=="rotate")
        {
            double angle, ax, ay, az;
            fin >> angle >> ax >> ay >> az;
            matrix rot = getRotationMatrix(angle, ax, ay, az);
            S.push(matmul(S.top(), rot));

        }
        else if (command=="push")
        {
            states.push(S);
        }
        else if (command=="pop")
        {
            S = states.top();
            states.pop();

        }
        else if (command=="end")
        {
            break;
        }

    }
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
    viewMatrix = getViewMatrix();
    projectionMatrix = getProjectionMatrix();
    printMatrix(projectionMatrix);
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

    stage1.open("stage1.txt");
    stage2.open("stage2.txt");
    stage3.open("stage3.txt");
    z_buffer.open("z-buffer.txt");

    stage1 << fixed << setprecision(7);
    stage2 << fixed << setprecision(7);
    stage3 << fixed << setprecision(7);
    z_buffer << fixed << setprecision(7);

    getSceneInfo();
    // printParams();
    executeStages();

    fin.close();
    stage1.close();
    stage2.close();
    stage3.close();
    z_buffer.close();
    return 0;
}