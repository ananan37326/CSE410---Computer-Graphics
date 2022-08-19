#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
#include "1705053_classes.hpp"
#include "bitmap_image.hpp"

#define pi (2 * acos(0.0))
#define move_distance 2
#define rotate_angle 0.08

// Global Variables

// file variables
ifstream fin;
int imageCount = 10;

// camera and glu parameters
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double screenWidth = 600;
double screenHeight = 600;
double viewAngle = 80;

// necessary variables
int recursionLevel;
int imagePixel;
int numObjects;
int numPointLights;
int numSpotLights;

// Floor parameters
double floorWidth = 1000;
double floorHeight = 1000;
double tileWidth = 20;
double tileHeight = 20;
double numTiles = floorWidth / tileWidth;

// Vectors of objects
vector<Object *> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

Vector3D origin(-floorWidth / 2, -floorWidth / 2, 0);

// Initial value for the vectors
Vector3D eye(100, 100, 20);
Vector3D u(0, 0, 1);
Vector3D r(-1 / sqrt(2), 1 / sqrt(2), 0);
Vector3D l(-1 / sqrt(2), -1 / sqrt(2), 0);

// MOVE OPERATIONS

void move_forward()
{
	// pos.x += move_distance * l.x;
	// pos.y += move_distance * l.y;
	// pos.z += move_distance * l.z;
	eye = eye + l * move_distance;
}

void move_backward()
{
	// pos.x -= move_distance * l.x;
	// pos.y -= move_distance * l.y;
	// pos.z -= move_distance * l.z;
	eye = eye - l * move_distance;
}

void move_right()
{
	// pos.x += move_distance * r.x;
	// pos.y += move_distance * r.y;
	// pos.z += move_distance * r.z;
	eye = eye + r * move_distance;
}

void move_left()
{
	// pos.x -= move_distance * r.x;
	// pos.y -= move_distance * r.y;
	// pos.z -= move_distance * r.z;
	eye = eye - r * move_distance;
}

void move_up()
{
	// pos.x += move_distance * u.x;
	// pos.y += move_distance * u.y;
	// pos.z += move_distance * u.z;
	eye = eye + u * move_distance;
}

void move_down()
{
	// pos.x -= move_distance * u.x;
	// pos.y -= move_distance * u.y;
	// pos.z -= move_distance * u.z;
	eye = eye - u * move_distance;
}

// ROTATE OPERATIONS

void rotate_left()
{
	l = l * cos(rotate_angle) - r * sin(rotate_angle);
	r = r * cos(rotate_angle) + l * sin(rotate_angle);
}

void rotate_right()
{
	l = l * cos(-rotate_angle) - r * sin(-rotate_angle);
	r = r * cos(-rotate_angle) + l * sin(-rotate_angle);
}

void look_up()
{
	u = u * cos(rotate_angle) - l * sin(rotate_angle);
	l = l * cos(rotate_angle) + u * sin(rotate_angle);
}

void look_down()
{
	u = u * cos(-rotate_angle) - l * sin(-rotate_angle);
	l = l * cos(-rotate_angle) + u * sin(-rotate_angle);
}

void tilt_clockwise()
{
	r = r * cos(rotate_angle) - u * sin(rotate_angle);
	u = u * cos(rotate_angle) + r * sin(rotate_angle);
}

void tilt_anticlockwise()
{
	r = r * cos(-rotate_angle) - u * sin(-rotate_angle);
	u = u * cos(-rotate_angle) + r * sin(-rotate_angle);
}

// user defined functions
void capture()
{
	bitmap_image image(imagePixel, imagePixel);

	for (int i = 0; i < imagePixel; i++)
	{
		for (int j = 0; j < imagePixel; j++)
		{
			image.set_pixel(i, j, 0, 0, 0);
		}
	}

	double planeDistance = (screenHeight / 2.0) / tan((viewAngle*pi) / (2.0 * 180.0));
	Vector3D topLeft = eye + l * planeDistance - r * (screenWidth / 2.0) + u * (screenHeight / 2.0);
	double du = screenWidth / imagePixel;
	double dv = screenHeight / imagePixel;
	topLeft = topLeft + r * (0.5 * du) - u * (0.5 * dv);

	int nearest;
	double t, tMin;

	for (int i = 0; i < imagePixel; i++)
	{
		for (int j = 0; j < imagePixel; j++)
		{
			Vector3D currentPixel = topLeft + r * (i * du) - u * (j * dv);
			Ray rayCast(eye, currentPixel - eye);

			tMin = INFINITY;
			nearest = -1;
			color pixelColor(0, 0, 0);
			color dummyColor(0, 0, 0);

			for (int k = 0; k < objects.size(); k++)
			{
				t = objects[k]->intersect(rayCast, dummyColor, 0);
				if(t!=-1)
				{
					//cout << "inside capture, t: " << t << endl;
				}

				if (t > 0 && t < tMin)
				{
					tMin = t;
					nearest = k;
				}
			}
			if(nearest!=-1)
			{
				//cout << "i = " << i << " j = " << j << endl;
				//cout << "nearest = " << nearest << endl;
				tMin = objects[nearest]->intersect(rayCast, pixelColor, 1);
				pixelColor = pixelColor * 255.0;
				image.set_pixel(i, j, pixelColor.r, pixelColor.g, pixelColor.b);
			}
		}
	}
	imageCount++;
	image.save_image("image"+to_string(imageCount)+".bmp");
	cout << "image" + to_string(imageCount) + ".bmp" << " saved" << endl;
}

// DRAW OPERATIONS

void drawAxes()
{
	if (drawaxes == 1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f(100, 0, 0);
			glVertex3f(-100, 0, 0);

			glVertex3f(0, -100, 0);
			glVertex3f(0, 100, 0);

			glVertex3f(0, 0, 100);
			glVertex3f(0, 0, -100);
		}
		glEnd();
	}
}

void drawGrid()
{
	int i;
	if (drawgrid == 1)
	{
		glColor3f(0.6, 0.6, 0.6); // grey
		glBegin(GL_LINES);
		{
			for (i = -8; i <= 8; i++)
			{

				if (i == 0)
					continue; // SKIP the MAIN axes

				// lines parallel to Y-axis
				glVertex3f(i * 10, -90, 0);
				glVertex3f(i * 10, 90, 0);

				// lines parallel to X-axis
				glVertex3f(-90, i * 10, 0);
				glVertex3f(90, i * 10, 0);
			}
		}
		glEnd();
	}
}

void drawSquare(double a)
{
	glColor3f(0, 255, 0.0);
	glBegin(GL_QUADS);
	{
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
	}
	glEnd();
}

void drawObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw();
	}
}

void drawLights()
{
	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i].draw();
	}
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{

	case '1':
		rotate_left();
		break;
	case '2':
		rotate_right();
		break;
	case '3':
		look_up();
		break;
	case '4':
		look_down();
		break;
	case '5':
		tilt_clockwise();
		break;
	case '6':
		tilt_anticlockwise();
		break;
	case '0':
		capture();
		break;
	default:
		break;
	}
}

void specialKeyListener(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_DOWN: // down arrow key
		move_backward();
		// printf("down arrow is pressed\n");
		break;
	case GLUT_KEY_UP: // up arrow key
		move_forward();
		// printf("up arrow is pressed\n");
		break;

	case GLUT_KEY_RIGHT:
		move_right();
		break;
	case GLUT_KEY_LEFT:
		move_left();
		break;

	case GLUT_KEY_PAGE_UP:
		move_up();
		break;
	case GLUT_KEY_PAGE_DOWN:
		move_down();
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		break;

		break;
	case GLUT_KEY_END:
		break;

	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y)
{ // x, y is the x-y of the screen (2D)
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{ // 2 times?? in ONE click? -- solution is checking DOWN or UP
			drawaxes = 1 - drawaxes;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		//........
		break;

	case GLUT_MIDDLE_BUTTON:
		//........
		break;

	default:
		break;
	}
}

void display()
{

	// clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0); // color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	// load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	// initialize the matrix
	glLoadIdentity();

	// now give three info
	// 1. where is the camera (viewer)?
	// 2. where is the camera looking?
	// 3. Which direction is the camera's UP direction?

	// gluLookAt(100,100,100,	0,0,0,	0,0,1);
	// gluLookAt(pos.x * cos(cameraAngle), pos.y * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);

	// gluLookAt(pos.x * cos(cameraAngle), pos.y * sin(cameraAngle), cameraHeight, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

	gluLookAt(eye.x, eye.y, eye.z, eye.x + l.x, eye.y + l.y, eye.z + l.z, u.x, u.y, u.z);

	// again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	// add objects

	drawAxes();
	drawGrid();

	// Drawing the cube, sphere and cylinder
	// drawCube();
	// drawFullSphere();
	// drawFullCylinder();
	drawObjects();
	drawLights();

	// ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate()
{
	angle += 0.05;
	// codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init()
{
	// codes for initialization
	drawgrid = 0;
	drawaxes = 1;
	cameraHeight = 150.0;
	cameraAngle = 1.0;
	angle = 0;

	// clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	// load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	// initialize the matrix
	glLoadIdentity();

	// give PERSPECTIVE parameters
	gluPerspective(viewAngle, 1, 1, 1000.0);
	// field of view in the Y (vertically)
	// aspect ratio that determines the field of view in the X direction (horizontally)
	// near distance
	// far distance
}

// User defined functions
void loadData()
{
	fin.open("scene_test.txt");
	if (!fin.is_open())
	{
		cout << "Error opening file" << endl;
		exit(1);
	}

	fin >> recursionLevel;
	fin >> imagePixel;

	fin >> numObjects;

	// adding floor to objects
	Floor *floor = new Floor(floorWidth, floorHeight, tileWidth);
	floor->setCoefficients(0.4, 0.2, 0.3, 0.3);
	floor->setShine(5);
	//objects.push_back(floor);

	for (int i = 0; i < numObjects; i++)
	{
		Object *currentObject;
		string objectType;
		fin >> objectType;

		if (objectType == "sphere")
		{
			cout << "Sphere" << endl;
			Vector3D center;
			color sphereColor;
			double radius;
			double ambient, diffuse, specular, reflection;
			int shine;

			fin >> center.x >> center.y >> center.z;
			fin >> radius;
			currentObject = new Sphere(center, radius);

			fin >> sphereColor.r >> sphereColor.g >> sphereColor.b;
			currentObject->setColor(sphereColor);

			fin >> ambient >> diffuse >> specular >> reflection;
			currentObject->setCoefficients(ambient, diffuse, specular, reflection);

			fin >> shine;
			currentObject->setShine(shine);

			objects.push_back(currentObject);
		}
		else if(objectType=="triangle")
		{
			Vector3D v1, v2, v3;
			color triangleColor;
			double ambient, diffuse, specular, reflection;
			int shine;

			fin >> v1.x >> v1.y >> v1.z;
			fin >> v2.x >> v2.y >> v2.z;
			fin >> v3.x >> v3.y >> v3.z;
			currentObject = new Triangle(v1, v2, v3);

			fin >> triangleColor.r >> triangleColor.g >> triangleColor.b;
			currentObject->setColor(triangleColor);

			fin >> ambient >> diffuse >> specular >> reflection;
			currentObject->setCoefficients(ambient, diffuse, specular, reflection);

			fin >> shine;
			currentObject->setShine(shine);
			objects.push_back(currentObject);
		}

		else if (objectType=="general")
		{
			double a, b, c, d, e, f, g, h, i, j;
			double length, width, height;
			Vector3D cubeRefPoint;
			color quadColor;
			double ambient, diffuse, specular, reflection;
			int shine;

			fin >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;
			currentObject = new GenQuad(a, b, c, d, e, f, g, h, i, j);

			fin >> cubeRefPoint.x >> cubeRefPoint.y >> cubeRefPoint.z;
			currentObject->reference_point = cubeRefPoint;

			fin >> length >> width >> height;
			currentObject->length = length;
			currentObject->width = width;
			currentObject->height = height;

			fin >> quadColor.r >> quadColor.g >> quadColor.b;
			currentObject->setColor(quadColor);

			fin >> ambient >> diffuse >> specular >> reflection;
			currentObject->setCoefficients(ambient, diffuse, specular, reflection);

			fin >> shine;
			currentObject->setShine(shine);

			objects.push_back(currentObject);
		}
	}

	objects.push_back(floor);

	fin >> numPointLights;
	cout << "Number of point lights: " << numPointLights << endl;
	for (int i = 0; i < numPointLights; i++)
	{
		Vector3D position;
		color lightColor;
		fin >> position.x >> position.y >> position.z;
		fin >> lightColor.r >> lightColor.g >> lightColor.b;
		PointLight currentLight(position, lightColor);
		pointLights.push_back(currentLight);
	}
}

void printObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		cout << objects[i]->reference_point.x << " " << objects[i]->reference_point.y << " " << objects[i]->reference_point.z << endl;
		cout << objects[i]->c.r << " " << objects[i]->c.g << " " << objects[i]->c.b << endl;
		cout << objects[i]->coEfficients[0] << " " << objects[i]->coEfficients[1] << " " << objects[i]->coEfficients[2] << " " << objects[i]->coEfficients[3] << endl;
		cout << objects[i]->shine << endl;
		cout << objects[i]->height << endl;
		cout << objects[i]->width << endl;
		cout << objects[i]->length << endl;
		cout << endl;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("Task3");

	init();

	glEnable(GL_DEPTH_TEST); // enable Depth Testing

	glutDisplayFunc(display); // display callback function
	glutIdleFunc(animate);	  // what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	loadData();
	// printObjects();

	glutMainLoop(); // The main loop of OpenGL

	return 0;
}
