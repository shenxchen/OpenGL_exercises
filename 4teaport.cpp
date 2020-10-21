#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static float a1 = 0;	//ת���Ƕ�
static float a2 = 0;
static float a3 = 0;
static float a4 = 0;

static bool power = 1;	//������ͣ�Ϳ�ʼ

static float left_right = 0;	//��ͷ�ڿռ�����
static float up_down = 0;
static float forward_back = 0;
static float alpha = 0;
static float alpha2 = 0;

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 0.5, 1000);
}

void dis(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0+left_right + forward_back * sin(alpha2), 0.0+up_down, 3.0+forward_back*cos(alpha2), 0.0 + left_right + forward_back * sin(alpha2) +10*sin(alpha), 0.0 + up_down, 3.0 + forward_back*cos(alpha2) - 10.0*cos(alpha), 0.0f, 1.0f, 0.0f);

	// ���1
	glPushMatrix();

	glTranslated(-0.5, 0.5, 0);
	glRotated(a1, 0, 1, 0);
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.3);

	glPopMatrix();

	//���2
	glPushMatrix();

	glTranslated(0.5, 0.5, 0);
	glRotated(a2, 0, 1, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.3);

	glPopMatrix();

	//���3
	glPushMatrix();

	glTranslated(-0.5, -0.5, 0);
	glRotated(a3, 0, 1, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.3);

	glPopMatrix();

	//���4
	glPushMatrix();

	glTranslated(0.5, -0.5, 0);
	glRotated(a4, 0, 1, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 0, 0);
	glutWireTeapot(0.3);

	glPopMatrix();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glFlush();
}

void move() {
	if (power == 1) {
		a1 += 0.1;
		a2 += 0.3;
		a3 += 0.5;
		a4 += 0.7;
	}

	glutPostRedisplay();
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

	case 'a':
		left_right -= 0.1;
		glutPostRedisplay();
		break;

	case 'd':
		left_right += 0.1;
		glutPostRedisplay();
		break;

	case 'w':
		up_down+= 0.1;
		glutPostRedisplay();
		break;

	case 's':
		up_down -= 0.1;
		glutPostRedisplay();
		break;

	case 'r':
		alpha2 = alpha;
		forward_back -= 0.1;
		glutPostRedisplay();
		break;

	case 'f':
		alpha2 = alpha;
		forward_back += 0.1;
		glutPostRedisplay();
		break;
	}
}

void special_key(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		alpha -= 0.1;
		break;

	case GLUT_KEY_RIGHT:
		alpha += 0.1;
		break;
	}
}

int main(int argc, char **argv)
{
	cout << "���ո��������ͣ�Ϳ�ʼ��ת" << endl;
	cout << "����ECS���˳�" << endl;
	cout << "����w����s����a����d��������������" << endl;
	cout << "����r����f������ǰ������" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("��������ҵ3");

	glutReshapeFunc(reshape);
	glutDisplayFunc(dis);
	glutIdleFunc(move);
	glutKeyboardFunc(key_func);
	glutSpecialFunc(special_key);

	glutMainLoop();
	return 0;
}