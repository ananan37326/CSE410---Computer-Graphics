#include<bits/stdc++.h>

using namespace std;

double eyeX, eyeY, eyeZ;
double lookX, lookY, lookZ;
double upX, upY, upZ;
double fov;
double aspectRatio;
double nearPlane, farPlane;

#define pi (2 * acos(0.0))

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
}

int main(){

    getSceneInfo();
    printParams();

    return 0;
}