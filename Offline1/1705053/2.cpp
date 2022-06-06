#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
#include <GL/glut.h>

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

// Global Variables
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double wheel_radius = 20;
double wheel_height = 5;
double wheel_slices = 25;
double wheel_stacks = 25;
double wheel_movement = 2;
double angle_movement = 0;
double axis_angle = 0;

// Initial value for the vectors
point pos(150, 100, 0);
point center(0, 0, 0);

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

// WHEEL ROTATION OPERATIONS
void rotate_forward()
{
	center.x += wheel_movement * cos(angle_movement * pi / 180);
	center.y += wheel_movement * sin(angle_movement * pi / 180);
	axis_angle -= 5.0;
}

void rotate_backward()
{
	center.x -= wheel_movement * cos(angle_movement * pi / 180);
	center.y -= wheel_movement * sin(angle_movement * pi / 180);
	axis_angle += 5.0;
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

				// if (i == 0)
				// 	continue; // SKIP the MAIN axes

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

void drawRectangle(double x, double y, double z)
{
	glBegin(GL_QUADS);
	glColor3f(0.6,0.6,0.6);
	{
		glVertex3f(x, y, z);
		glVertex3f(x, -y, z);
		glVertex3f(-x, -y, -z);
		glVertex3f(-x, y, -z);
	}
	glEnd();
}

void drawCylinder(double radius, double height, int slices, int stacks)
{
	struct point points[100][100];
	int i, j;
	double h, r;
	// generate points
	for (i = 0; i <= stacks; i++)
	{
		h = height * sin(((double)i / (double)stacks) * (pi / 2));
		r = radius;
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
		// glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for (j = 0; j < slices; j++)
		{
			glColor3f((double)j/(double)slices, (double)j/(double)slices,(double)j/(double)slices);
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

void drawWheel()
{
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	glRotatef(angle_movement, 0, 0, 1);
	glTranslatef(0, 0, wheel_radius);
	glRotatef(90, 1, 0, 0);
	glRotatef(axis_angle, 0, 0, 1);
	drawCylinder(wheel_radius, wheel_height, wheel_slices, wheel_stacks);
	drawRectangle(0,wheel_radius,wheel_height);
	glRotatef(90, 0, 1, 0);
	glRotatef(-90,1,0,0);
	drawRectangle(wheel_height,wheel_radius,0);
	glPopMatrix();
}

void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{

	case '1':
		break;
	case '2':
		break;
	case '3':
		break;
	case '4':
		break;
	case '5':
		break;
	case '6':
		break;
	case 'w':
		rotate_forward();
		break;
	case 's':
		rotate_backward();
		break;
	case 'a':
		angle_movement += 2;
		break;
	case 'd':
		angle_movement -= 2;
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
		cameraHeight -= 2.0;
		// printf("down arrow is pressed\n");
		break;
	case GLUT_KEY_UP: // up arrow key
		cameraHeight += 2.0;
		// printf("up arrow is pressed\n");
		break;

	case GLUT_KEY_RIGHT:
		cameraAngle += 0.05;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= 0.05;
		break;

	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:

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
	gluLookAt(pos.x * cos(cameraAngle), pos.y * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);

	// gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

	// again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	/****************************
	/ Add your objects from here
	****************************/
	// add objects

	drawAxes();
	drawGrid();

	// draw the wheel with the rims
	drawWheel();

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
	drawgrid = 1;
	drawaxes = 0;
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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("1705053_Task3");

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
