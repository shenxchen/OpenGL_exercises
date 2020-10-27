#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static float R = 0.8;	//Բ���뾶
static float r = 0.1;	//�����뾶

static int N = 100;
static int M = 10;
static float del_theta = 2 * M_PI / N;
static float del_phi= 2 * M_PI / M;

static float a = 0;	//ת���Ƕ�
static float b = 0;

static bool power = 1;	//������ͣ�Ϳ�ʼ

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// specify the dimensions of the plot
	//glOrtho(-1, 1, -1, 1, -1.0, 1.0);
	gluPerspective(45, ratio, 1, 1000);

	/*
	static float aspect_ratio = static_cast<float>(height) / width;
	if (height > (width*aspect_ratio)) {
		int offset_y = (height - width * aspect_ratio) / 2;
		glViewport(0, offset_y, width, width*aspect_ratio);
	}
	else {
		int offset_x = (width - height / aspect_ratio) / 2;
		glViewport(offset_x, 0, height / aspect_ratio, height);
	}
	*/
}

void dis(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, sin(b), cos(b), 0.0f);

	glPushMatrix();	//����ת���ٶ�
	glRotated(a, 1, 0, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			float theta = del_theta * i;
			float phi = del_phi * j;
			float theta2 = del_theta * (i + 1);
			float phi2 = del_phi * (j + 1);

			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f((R - r * cos(phi))*cos(theta), (R - r * cos(phi))*sin(theta), r*sin(phi));
			glVertex3f((R - r * cos(phi2))*cos(theta), (R - r * cos(phi2))*sin(theta), r*sin(phi2));
			glVertex3f((R - r * cos(phi))*cos(theta2), (R - r * cos(phi))*sin(theta2), r*sin(phi));
			glVertex3f((R - r * cos(phi2))*cos(theta2), (R - r * cos(phi2))*sin(theta2), r*sin(phi2));
			glEnd();

		}
	}

	glPopMatrix();	//����ת���ٶ�

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

	case 'a':
		b -= 0.1;
		glutPostRedisplay();
		break;

	case 'd':
		b += 0.1;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char **argv)
{
	cout << "���ո��������ͣ�Ϳ�ʼ" << endl;
	cout << "����ECS���˳�" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("圆环");

	glutReshapeFunc(reshape);
	glutDisplayFunc(dis);
	glutIdleFunc(move);
	glutKeyboardFunc(key_func);

	glutMainLoop();
	return 0;
}
