#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
#include <GL/glut.h>
#include "1705053_classes.hpp"

#define pi (2 * acos(0.0))
#define move_distance 2
#define rotate_angle 0.08



// Global Variables
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


double floorWidth = 200;
double floorHeight = 200;
double tileWidth = 20;
double tileHeight = 20;
double numTiles = floorWidth / tileWidth;

Vector3D origin(floorWidth/2,floorHeight/2, 0);

// Initial value for the vectors
Vector3D pos(100, 100, 0);
Vector3D u(0, 0, 1);
Vector3D r(-1 / sqrt(2), 1 / sqrt(2), 0);
Vector3D l(-1 / sqrt(2), -1 / sqrt(2), 0);



// MOVE OPERATIONS

void move_forward()
{
	// pos.x += move_distance * l.x;
	// pos.y += move_distance * l.y;
	// pos.z += move_distance * l.z;
	pos = pos + l * move_distance;
}

void move_backward()
{
	// pos.x -= move_distance * l.x;
	// pos.y -= move_distance * l.y;
	// pos.z -= move_distance * l.z;
	pos = pos - l * move_distance;
}

void move_right()
{
	// pos.x += move_distance * r.x;
	// pos.y += move_distance * r.y;
	// pos.z += move_distance * r.z;
	pos = pos + r * move_distance;
}

void move_left()
{
	// pos.x -= move_distance * r.x;
	// pos.y -= move_distance * r.y;
	// pos.z -= move_distance * r.z;
	pos = pos - r * move_distance;
}

void move_up()
{
	// pos.x += move_distance * u.x;
	// pos.y += move_distance * u.y;
	// pos.z += move_distance * u.z;
	pos = pos + u * move_distance;
}

void move_down()
{
	// pos.x -= move_distance * u.x;
	// pos.y -= move_distance * u.y;
	// pos.z -= move_distance * u.z;
	pos = pos - u * move_distance;
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


void drawFloor()
{
	for (int i = 0; i < numTiles; i++)
	{
		for (int j = 0; j < numTiles; j++)
		{
			glColor3f((i + j) % 2, (i + j) % 2, (i + j) % 2);

			glBegin(GL_QUADS);
			{
				glVertex3f(origin.x + i * tileWidth, origin.y + j * tileWidth, origin.z);
				glVertex3f(origin.x + i * tileWidth, origin.y + (j + 1) * tileWidth, origin.z);
				glVertex3f(origin.x + (i + 1) * tileWidth, origin.y + (j + 1) * tileWidth, origin.z);
				glVertex3f(origin.x + (i + 1) * tileWidth, origin.y + j * tileWidth, origin.z);
			}
			glEnd();
		}
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

	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

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
	drawFloor();

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
	gluPerspective(80, 1, 1, 1000.0);
	// field of view in the Y (vertically)
	// aspect ratio that determines the field of view in the X direction (horizontally)
	// near distance
	// far distance
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

	glutMainLoop(); // The main loop of OpenGL

	return 0;
}
