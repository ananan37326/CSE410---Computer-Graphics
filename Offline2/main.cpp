#include<bits/stdc++.h>

using namespace std;

double eyeX, eyeY, eyeZ;
double lookX, lookY, lookZ;
double upX, upY, upZ;
double fov;
double aspectRatio;
double nearPlane, farPlane;

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