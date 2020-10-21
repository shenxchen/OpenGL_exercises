#include "GL/glut.h"
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static float a = 0;	//转动角度

static bool power = 1;	//控制暂停和开始

void reshape(int width, int height)
{
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
	
	float ratio = 1.0 * width / height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// specify the dimensions of the plot
	//glOrtho(-1, 1, -1, 1, -1.0, 1.0);
	gluPerspective(45, ratio, 1, 10);
	//glFrustum(-0.5,0.5,-0.5,0.5,1,10);

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2, 2, 4.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);*/
	
}

void dis(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2, 2, 4.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	glPushMatrix();	//调回转动速度
	glRotated(a, 0, 1, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 0, 0);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	glPopMatrix();	//调回转动速度

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glFlush();
}

void move() {
	if (power == 1) {
		a += 0.05;
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
	glutCreateWindow("线框长方体盒子");

	glutReshapeFunc(reshape);
	glutDisplayFunc(dis);
	glutIdleFunc(move);
	glutKeyboardFunc(key_func);

	glutMainLoop();
	return 0;
}