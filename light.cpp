#include "GL/glut.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static float a = 0;

static bool power = 1;

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 1000);

    
}

void disp(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glRotated(a, 0, 1, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glColor3f(1,0,0);
    glutSolidTeapot(0.5);


	glPopMatrix();

	glFlush();
}

void move() {
	if (power == 1) {
		a += 0.1;
		glutPostRedisplay();
	}
}

void key_func(unsigned char key, int x, int y) {
	switch (key)
	{
	case 32:
		power = 1 - power;
		break;

	case 27:
		exit(1);
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("1");

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

	glutReshapeFunc(reshape);
	glutDisplayFunc(disp);
	glutIdleFunc(move);
	glutKeyboardFunc(key_func);

	glutMainLoop();
	return 0;
}