#include "GL/glut.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat diffuse[] = { 1,1,1,1 };
	GLfloat specular[] = { 1,1,1,1 };
	GLfloat position[] = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);

	float m_diffuse[] = { 1,1,0,1 };
	float m_specular[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	
	glPushMatrix();

    glutSolidSphere(0.5,50,50);

	glPopMatrix();

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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("π‚’’¡∑œ∞1");

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutReshapeFunc(reshape);
	glutDisplayFunc(disp);
	glutKeyboardFunc(key_func);

	glutMainLoop();
	return 0;
}