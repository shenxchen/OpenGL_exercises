#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static float a = 0;	//转动角度

static bool power = 1;	//控制暂停和开始

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 1000);
}

void DrawSphere(GLdouble radius, GLint slices, GLint stacks, bool bSolid)
{
	float R = radius;
	float del_theta = M_PI / stacks;
	float del_phi = 2 * M_PI / slices;

	if (bSolid == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glColor3f(1, 0, 0);
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			float theta = del_theta * i;
			float theta2 = del_theta * (i + 1);
			float phi = del_phi * j;
			float phi2 = del_phi * (j + 1);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(R*sin(theta)*cos(phi), R*sin(theta)*sin(phi), R*cos(theta));
			glVertex3f(R*sin(theta2)*cos(phi), R*sin(theta2)*sin(phi), R*cos(theta2));
			glVertex3f(R*sin(theta)*cos(phi2), R*sin(theta)*sin(phi2), R*cos(theta));
			glVertex3f(R*sin(theta2)*cos(phi2), R*sin(theta2)*sin(phi2), R*cos(theta2));
			glEnd();
		}
	}
}

void disp(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, -2.0, 4.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glRotated(a, 0, 0, 1);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	DrawSphere(0.8, 20, 20, 0);

	glPopMatrix();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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
	cout << "按空格键控制暂停和开始" << endl;
	cout << "按‘ECS’退出" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("第二章作业");

	glutReshapeFunc(reshape);
	glutDisplayFunc(disp);
	glutIdleFunc(move);
	glutKeyboardFunc(key_func);

	glutMainLoop();
	return 0;
}