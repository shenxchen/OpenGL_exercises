#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static const int L = 10;
static const float h = 2;

static float a = 0.1;

static float an = 0;

static float ex = 0.0;
static float ey = -5.0;
static float ez = h;

static float cx = 0.0;
static float cy = 0.0;
static float cz = h;

static float l = 0.1;

static float t1;
static float t2;

void Block(float rx, float ry, float rz, float r, float g, float b) {
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0 + rx, 1 + ry, -1 + rz);
	glVertex3f(0 + rx, 0 + ry, -1 + rz);
	glVertex3f(0 + rx, 1 + ry, 0 + rz);
	glVertex3f(0 + rx, 0 + ry, 0 + rz);
	glVertex3f(1 + rx, 1 + ry, 0 + rz);
	glVertex3f(1 + rx, 0 + ry, 0 + rz);
	glVertex3f(1 + rx, 1 + ry, -1 + rz);
	glVertex3f(1 + rx, 0 + ry, -1 + rz);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(1 + rx, 0 + ry, 0 + rz);
	glVertex3f(0 + rx, 0 + ry, 0 + rz);
	glVertex3f(1 + rx, 0 + ry, -1 + rz);
	glVertex3f(0 + rx, 0 + ry, -1 + rz);
	glVertex3f(1 + rx, 1 + ry, -1 + rz);
	glVertex3f(0 + rx, 1 + ry, -1 + rz);
	glVertex3f(1 + rx, 1 + ry, 0 + rz);
	glVertex3f(0 + rx, 1 + ry, 0 + rz);
	glEnd();
}

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 0.1, 20);
}

float f(float x, float y)
{
	//return 0.1*sin(10*x)*sin(10*y)*pow(3,y);
	//return 0.3*pow(3, -y*y)*cos(x-y);
	//return 0.5*pow(3, -(x*x + y * y));
	return 0;
}

void dis(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ex, ey, ez, cx, cy, cz, 0.0f, 0.0f, 1.0f);
	//gluLookAt(0, -1, 1, 0, 0, 0.5, 0.0f, 0.0f, 1.0f);

	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0, 0, 0);

	for (int i = -L; i <L; i++) {
		for (int j = -L; j < L; j++) {
			if ((i + j) % 2 == 0) {
				Block(i, j, 0, 0, 0, 0);
			}
			else {
				Block(i, j, 0, 1, 0, 0);
			}
			
		}
		
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glFlush();
}


void key_func(unsigned char key, int x, int y) {
	switch (key)
	{

	case 27:
		exit(1);
		break;

	case 'a':
		t1 = -l * (cy - ey);
		t2 = l * (cx - ex);
		ex += t1;
		ey += t2;
		cx += t1;
		cy += t2;
		ez = f(ex, ey) + h;
		glutPostRedisplay();
		break;

	case 'd':
		t1 = l * (cy - ey);
		t2 = -l * (cx - ex);
		ex += t1;
		ey += t2;
		cx += t1;
		cy += t2;
		ez = f(ex, ey) + h;
		glutPostRedisplay();
		break;

	case 'w':
		t1 = l * (cx - ex);
		t2 = l * (cy - ey);
		ex += t1;
		ey += t2;
		cx += t1;
		cy += t2;
		ez = f(ex, ey) + h;
		glutPostRedisplay();
		break;

	case 's':
		t1 = l * (cx - ex);
		t2 = l * (cy - ey);
		ex -= t1;
		ey -= t2;
		cx -= t1;
		cy -= t2;
		ez = f(ex, ey) + h;
		glutPostRedisplay();
		break;
	}
}

void special_key(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		cz += 0.1;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		cz -= 0.1;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		t1 = ex + (cx - ex) * cos(a) - (cy - ey) * sin(a);
		t2 = ey + (cx - ex) * sin(a) + (cy - ey) * cos(a);
		cx = t1;
		cy = t2;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		t1 = ex + (cx - ex) * cos(a) + (cy - ey) * sin(a);
		t2 = ey + -(cx - ex) * sin(a) + (cy - ey) * cos(a);
		cx = t1;
		cy = t2;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("MC");

	glutReshapeFunc(reshape);
	glutDisplayFunc(dis);
	//glutIdleFunc(move);
	glutKeyboardFunc(key_func);
	glutSpecialFunc(special_key);

	glutMainLoop();
	return 0;
}