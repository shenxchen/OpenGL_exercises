#include "GL/glut.h"
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static bool left_button_is_pressed = false;
static float press_x=0;
static float press_y=0;
static float mouse_x=0;
static float mouse_y=0;

static float a_h = 0;
static float a_v = 0;
static float angle_h = 0;
static float angle_v = 0;

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 0.1, 20);
}

void dis(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4, 1.5, 2, 0, 0, 0, 0.0f, 0.0f, 1.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glRotatef(a_h+angle_h, 0, 0, 1);
	glRotatef(a_v + angle_v, 0, 1, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5.0);
	glColor3f(0, 0.5, 1);
	glutSolidCube(2);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBegin(GL_TRIANGLES);
	glColor4f(1, 0.5, 0, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, -1, 1);

	glColor4f(0, 0.5, 1, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, 1);

	glColor4f(0.5, 0, 1, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, 1);

	glColor4f(1, 0, 0, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, 1, -1);

	glColor4f(0.5, 1, 0, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, -1);
	
	glEnd();

	glBegin(GL_QUADS);
	glColor4f(1, 1, 0, 0.7);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, -1, 1);
	glEnd();

	glFlush();
}


void key_func(unsigned char key, int x, int y) {
	switch (key)
	{

	case 27:
		exit(1);
		break;
	}
}

void mouse_func(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button_is_pressed = true;
		press_x = x;
		press_y = y;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button_is_pressed = false;
		a_h += angle_h;
		a_v += angle_v;
	}
}

void motion_func(int x, int y)
{
	if (left_button_is_pressed == true) {
		mouse_x = x;
		mouse_y = y;
		angle_h = (mouse_x - press_x) / 2.0;
		angle_v = (mouse_y - press_y) / 2.0;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Phase Diagram");

	glutReshapeFunc(reshape);
	glutDisplayFunc(dis);
	glutKeyboardFunc(key_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutMainLoop();
	return 0;
}