#include "GL/glut.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

static int WIN_WIDTH = 600;
static int WIN_HEIGHT = 600;

static bool flag = 1;

float radius = 0.5;
int stacks = 500;
int slices = 500;

float **nx = new float*[stacks];
float **ny = new float*[stacks];
float **nz = new float*[stacks];
float ***Point = new float**[stacks + 1];

void GetNormalize(float *nx, float* ny, float* nz, float x1, float  y1, float  z1, float x2, float  y2, float  z2, float x3, float  y3, float  z3) {
	float u1, u2, u3, v1, v2, v3; // 定义两个向量的分量
	u1 = x2 - x1; 	u2 = y2 - y1; 	u3 = z2 - z1;
	v1 = x3 - x1; 	v2 = y3 - y1; 	v3 = z3 - z1;
	// 计算两个向量
	*nx = u2 * v3 - v2 * u3; 	*ny = u3 * v1 - v3 * u1;
	*nz = u1 * v2 - u2 * v1;
	float d = sqrt(pow(*nx, 2.) + pow(*ny, 2.) + pow(*nz, 2.)); // 取模
	*nx = *nx / d; // 法相向量归一化成单位向量 x,y,z
	*ny = *ny / d;
	*nz = *nz / d;
}

void init() {
	float R = radius;
	float del_theta = M_PI / stacks;
	float del_phi = 2 * M_PI / slices;
	
	for (int i = 0; i < stacks; i++) {
		nx[i] = new float[slices];
		ny[i] = new float[slices];
		nz[i] = new float[slices];
	}
	for (int i = 0; i <= stacks; i++) {
		Point[i] = new float*[slices + 1];
		for (int j = 0; j <= slices; j++) {
			Point[i][j] = new float[3];
		}
	}

	for (int i = 0; i <= stacks; i++) {
		for (int j = 0; j <= slices; j++) {
			float theta = del_theta * i;
			float phi = del_phi * j;

			Point[i][j][0] = R * sin(theta)*cos(phi);
			Point[i][j][1] = R * sin(theta)*sin(phi);
			Point[i][j][2] = R * cos(theta);
		}
	}

	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			GetNormalize(&nx[i][j], &ny[i][j], &nz[i][j],
				Point[i][j][0], Point[i][j][1], Point[i][j][2],
				Point[i + 1][j][0], Point[i + 1][j][1], Point[i + 1][j][2],
				Point[i][j + 1][0], Point[i][j + 1][1], Point[i][j + 1][2]
			);
		}
	}
}


void DrawSphere(GLfloat radius, GLint slices, GLint stacks)
{
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(nx[i][j], ny[i][j], nz[i][j]);
			glVertex3f(Point[i][j][0], Point[i][j][1], Point[i][j][2]);
			glVertex3f(Point[i + 1][j][0], Point[i + 1][j][1], Point[i + 1][j][2]);
			glVertex3f(Point[i][j + 1][0], Point[i][j + 1][1], Point[i][j + 1][2]);
			glVertex3f(Point[i+1][j + 1][0], Point[i+1][j + 1][1], Point[i+1][j + 1][2]);
			glEnd();
		}
	}
}

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

	float m_diffuse[] = { 1,1,1,1 };
	float m_specular[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);

	glPushMatrix();

	glRotated(108, 0, 1, 0);
	DrawSphere(radius, slices, stacks);

	glPopMatrix();

	glFlush();
}

void key_func(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		for (int i = 0; i < stacks; i++) {
			delete[] nx[i], ny[i], nz[i];
		}
		delete nx, ny, nz;
		for (int i = 0; i <= stacks; i++) {
			for (int j = 0; j <= slices; j++) {
				delete[] Point[i][j];
			}
		}
		for (int i = 0; i <= stacks; i++) {
			delete[] Point[i];
		}
		delete[] Point;
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
	glutCreateWindow("光照练习2（自绘法线）");

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	if (flag == 1) {
		flag = 0;
		init();
	}
	glutReshapeFunc(reshape);
	glutDisplayFunc(disp);
	glutKeyboardFunc(key_func);

	glutMainLoop();
	return 0;
}