#include "gl\glut.h"

void * font[] = {
	GLUT_BITMAP_8_BY_13,
	GLUT_BITMAP_9_BY_15,
	GLUT_BITMAP_TIMES_ROMAN_10,
	GLUT_BITMAP_TIMES_ROMAN_24,
	GLUT_BITMAP_HELVETICA_10,
	GLUT_BITMAP_HELVETICA_12,
	GLUT_BITMAP_HELVETICA_18
};
const char *Text = "I love SJTU!";
const char *c;

void display()
{
	glLoadIdentity();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	
	glTranslatef(0, 0, -10);
	glColor3f(1, 1, 0);
	glRasterPos3f(-3, 3, -10);
	for (c = Text; *c != '\0'; c++)
		glutBitmapCharacter(font[1], *c);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 0, -10);
	glColor3f(1, 0, 0);
	glRasterPos3f(-3, -3, -10);
	for (c = Text; *c != '\0'; c++)
		glutBitmapCharacter(font[3], *c);

	glPopMatrix();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / h, 0.1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("GLUT charcter!!");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
