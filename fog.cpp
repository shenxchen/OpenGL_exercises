#include <GL/glut.h>

void myinit(void)
{
	GLfloat mat_ambient[] = { 1, 0, 0, 1.0 };
	GLfloat mat_diffuse[] = { 1, 0, 0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 0.0, 0, 1.00 };
	GLfloat mat_shininess[] = { 15.0 };
	GLfloat position[] = { 5.0, 5.0, 5.0, 1.0 };
	GLfloat fogColor[] = { 0.6, 0.6, 0.6, 0.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 启用雾化功能-----------------
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2  /*GL_EXP	GL_EXP2	GL_LINEAR*/);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.15f);
	glFogf(GL_FOG_START, 5.0);
	glFogf(GL_FOG_END, 200.0);
	glHint(GL_FOG_HINT, GL_NICEST);

	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(-2.0, -1.5, -5.0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -1.5, -7.0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, -1.5, -9.0);
	glutSolidTeapot(1);
	glPopMatrix();

	glFlush();
}

void myReshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

int main()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("雾化");
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
