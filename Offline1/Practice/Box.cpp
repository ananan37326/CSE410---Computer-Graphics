#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2 * acos(0.0))
#define move_distance 2
#define rotate_angle 0.08

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


double arm_length = 20; // arm length of the cube
double max_length = 25; // maximum length allowed
double total_slices = 24;
double total_stacks = 20;
double arm_angles[] = {0,0,0,0,0,0};
double translation_length = arm_length;

// Initial value for the vectors
point pos(100, 100, 0);
point u(0, 0, 1);
point r(-1 / sqrt(2), 1 / sqrt(2), 0);
point l(-1 / sqrt(2), -1 / sqrt(2), 0);

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
	glBegin(GL_QUADS);
	{
		// glVertex3f(0, 0, 0);
		// glVertex3f(0, a, 0);
		// glVertex3f(a, a, 0);
		// glVertex3f(a, 0, 0);
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
	}
	glEnd();
}

void drawBox()
{
    glPushMatrix();
	glColor3f(0,0,255);
	drawSquare(arm_length/2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(255,0,0);
	glTranslatef(0,arm_length/2,0);
	glRotatef(arm_angles[4],1,0,0);
	glTranslatef(0,arm_length/2,0);
	drawSquare(arm_length/2);
	

	glColor3f(0,0,255);
	glTranslatef(0,arm_length/2,0);
	glRotatef(arm_angles[3],1,0,0);
	glTranslatef(0,arm_length/2,0);
	drawSquare(arm_length/2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0,255,0);
	glTranslatef(0,-arm_length/2,0);
	glRotatef(-arm_angles[2],1,0,0);
	glTranslatef(0,-arm_length/2,0);
	drawSquare(arm_length/2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(127,228,0);
	glTranslatef(arm_length/2,0,0);
	glRotatef(-arm_angles[1],0,1,0);
	glTranslatef(arm_length/2,0,0);
	drawSquare(arm_length/2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(127,124,233);
	
	glTranslatef(-arm_length/2,0,0);
	glRotatef(arm_angles[0],0,1,0);
	glTranslatef(-arm_length/2,0,0);
	drawSquare(arm_length/2);
	glPopMatrix();
}





void keyboardListener(unsigned char key, int x, int y)
{
	switch (key)
	{

	case '1':
		arm_angles[0] = arm_angles[0] + 5;
		if (arm_angles[0]>90)
		{
			/* code */
			arm_angles[0] = 90;
		}
		//translation_length = arm_length*cos(arm_angles[0]*pi/180);
		
		break;
	case '2':
		arm_angles[0] = arm_angles[0] - 5;
		if (arm_angles[0]<0)
		{
			/* code */
			arm_angles[0] = 0;
		}
		break;
	case '3':
		arm_angles[1] = arm_angles[1] + 5;
		if (arm_angles[1]>90)
		{
			/* code */
			arm_angles[1] = 90;
		}
		break;
	case '4':
		arm_angles[1] = arm_angles[1] - 5;
		if (arm_angles[1]<0)
		{
			/* code */
			arm_angles[1] = 0;
		}
		break;
	case '5':
		arm_angles[2] = arm_angles[2] + 5;
		if (arm_angles[2]>90)
		{
			/* code */
			arm_angles[2] = 90;
		}
		break;
	case '6':
		arm_angles[2] = arm_angles[2] - 5;
		if (arm_angles[2]<0)
		{
			/* code */
			arm_angles[2] = 0;
		}
		break;
	case '7':
		arm_angles[3] = arm_angles[3] + 5;
		if (arm_angles[3]>90)
		{
			/* code */
			arm_angles[3] = 90;
		}
		break;
	case '8':
		arm_angles[3] = arm_angles[3] - 5;
		if (arm_angles[3]<0)
		{
			/* code */
			arm_angles[3] = 0;
		}
		break;
	case '9':
		arm_angles[4] = arm_angles[4] + 5;
		if (arm_angles[4]>90)
		{
			/* code */
			arm_angles[4] = 90;
		}
		break;
	case '0':
		arm_angles[4] = arm_angles[4] - 5;
		if (arm_angles[4]<0)
		{
			/* code */
			arm_angles[4] = 0;
		}
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
		move_up();
		break;
	case GLUT_KEY_PAGE_DOWN:
		move_down();
		break;

	case GLUT_KEY_INSERT:
		break;

	case GLUT_KEY_HOME:
		arm_length += 5;
		if (arm_length > max_length)
		{
			arm_length = max_length;
		}
		
		break;
	case GLUT_KEY_END:
		arm_length -= 5;
		if (arm_length < 0)
		{
			arm_length = 0;
		}
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
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	//gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

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
    drawBox();

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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color

	glutCreateWindow("1705053_Task1_and_Task2");

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
