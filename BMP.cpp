#include "gl\glut.h"


#include "BMPLoad.h"

BMPPic * bmpPic;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRasterPos3f(-100, -70, -400);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	printf("%d %d", bmpPic->iwidth, bmpPic->iheight);
	glDrawPixels(bmpPic->iwidth, bmpPic->iheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmpPic->pByte);

	glFlush();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main()
{
	bmpPic = ZXCLoadBMP("pic.bmp");
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Read BMP Picture");
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
}