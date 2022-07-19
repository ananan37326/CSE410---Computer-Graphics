#include<bits/stdc++.h>

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
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                mat[i][j] = 0;
    }

    
};

double eyeX, eyeY, eyeZ;
double lookX, lookY, lookZ;
double upX, upY, upZ;
double fov;
double aspectRatio;
double nearPlane, farPlane;
point eye, look, up;


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

double dot(point a, point b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

point cross(point a, point b)
{
    return point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void modelingTransformation()
{

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


void getSceneInfo(){
    ifstream fin("scene.txt");
    if (!fin) {
        cout << "Error opening file" << endl;
        exit(1);
    }

    fin >> eyeX >> eyeY >> eyeZ;
    fin >> lookX >> lookY >> lookZ;
    fin >> upX >> upY >> upZ;
    fin >> fov;
    fin >> aspectRatio;
    fin >> nearPlane >> farPlane;
    initializegluLookAtParams();



    fin.close();
}

void printParams(){
    cout << "Eye: " << eyeX << " " << eyeY << " " << eyeZ << endl;
    cout << "Look: " << lookX << " " << lookY << " " << lookZ << endl;
    cout << "Up: " << upX << " " << upY << " " << upZ << endl;
    cout << "FOV: " << fov << endl;
    cout << "Aspect Ratio: " << aspectRatio << endl;
    cout << "Near Plane: " << nearPlane << endl;
    cout << "Far Plane: " << farPlane << endl;
    cout << "Eye: " << eye.x << " " << eye.y << " " << eye.z << endl;
    cout << "Look: " << look.x << " " << look.y << " " << look.z << endl;
    cout << "Up: " << up.x << " " << up.y << " " << up.z << endl; 
}


int main(){

    getSceneInfo();
    printParams();

    return 0;
}