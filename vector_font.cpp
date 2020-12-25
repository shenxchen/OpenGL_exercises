#include "string.h"
#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glut.h"

const char *Text = "OpenGL";
const char *c;

void display()
{
	static float angle = 0.;
	angle += 0.1;

	glLoadIdentity();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);


	glScalef(0.01, 0.01, 0.01);//控制字体的缩放
	glColor3f(0, 1, 0);
	glTranslatef(0, 0, -1000);
	glRotatef(angle, 1, 1, 1);

	for (c = Text; *c != '\0'; c++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c); //  GLUT_STROKE_MONO_ROMAN

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / h, 0.01, 10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void Idle()
{	glutPostRedisplay();
}

main()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("GLUT charcter!!");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutMainLoop();
}
