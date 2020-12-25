#include "GL\glut.h"

#include "BMPLoad.h"
BMPPic * PicWood;
BMPPic * PicKuang;
GLuint textPic[2];

void init()
{
	PicWood = ZXCLoadBMP("木材.bmp"); //house1016.BMP宽度是1016，每行站1016*3=3048个字节，该字节正好可以被1,2,4,8整除，所以在Alignment中设置哪个值都可以!!
	glGenTextures(2, textPic);
	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, PicWood->iwidth, PicWood->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, PicWood->pByte); // 图片存储到显卡中
	free(PicWood->pByte); // 释放图片所占计算机内存
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textPic[1]);
	PicKuang = ZXCLoadBMP("kuang.bmp"); //house1016.BMP宽度是1016，每行占1016*3=3048个字节，该字节正好可以被1,2,4,8整除，所以在Alignment中设置哪个值都可以!!
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, PicKuang->iwidth, PicKuang->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, PicKuang->pByte);// 图片存储到显卡中
	free(PicKuang->pByte);  // 释放图片所占计算机内存

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void RenderScene(void)
{
	static float a = 0.;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 首先可以显示一下读取的BMP图形文件，如果能显示，说明读取文件没有问题******************************************
	//glRasterPos3f(-40,-41,-100);
	//glPixelStoref(GL_UNPACK_ALIGNMENT,1);
	//	printf("%d %d %d %d %x",tgaPic->iWidth , tgaPic->iHeight, tgaPic->eFormat,tgaPic->iComponents,*(tgaPic->pBytes)));
	//glDrawPixels(PicWood->iwidth , PicWood->iheight, GL_BGR_EXT,GL_UNSIGNED_BYTE,PicWood->pByte);

	// 下面是正常的纹理映射操作**************************************************************
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(a, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glBegin(GL_QUADS); // 前面
	glTexCoord2f(0., 0.); glVertex3f(-1, -1, 1);
	glTexCoord2f(1., 0.); glVertex3f(1, -1, 1);
	glTexCoord2f(1., 1.); glVertex3f(1, 1, 1);
	glTexCoord2f(0., 1.); glVertex3f(-1, 1, 1);
	glEnd();
	glBegin(GL_QUADS); // 后面
	glTexCoord2f(0., 0.); glVertex3f(1, -1, -1);
	glTexCoord2f(1., 0.); glVertex3f(-1, -1, -1);
	glTexCoord2f(1., 1.); glVertex3f(-1, 1, -1);
	glTexCoord2f(0., 1.); glVertex3f(1, 1, -1);
	glEnd();
	glBegin(GL_QUADS); // 左面
	glTexCoord2f(0., 0.); glVertex3f(-1, -1, -1);
	glTexCoord2f(1., 0.); glVertex3f(-1, -1, 1);
	glTexCoord2f(1., 1.); glVertex3f(-1, 1, 1);
	glTexCoord2f(0., 1.); glVertex3f(-1, 1, -1);
	glEnd();
	glBegin(GL_QUADS); // 右面
	glTexCoord2f(0., 0.); glVertex3f(1, -1, 1);
	glTexCoord2f(1., 0.); glVertex3f(1, -1, -1);
	glTexCoord2f(1., 1.); glVertex3f(1, 1, -1);
	glTexCoord2f(0., 1.); glVertex3f(1, 1, 1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textPic[1]);
	glBegin(GL_QUADS); // 上面
	glTexCoord2f(0., 0.); glVertex3f(-1, 1, 1);
	glTexCoord2f(1., 0.); glVertex3f(1, 1, 1);
	glTexCoord2f(1., 1.); glVertex3f(1, 1, -1);
	glTexCoord2f(0., 1.); glVertex3f(-1, 1, -1);
	glEnd();
	glBegin(GL_QUADS); // 下面
	glTexCoord2f(0., 0.); glVertex3f(-1, -1, 1);
	glTexCoord2f(1., 0.); glVertex3f(-1, -1, -1);
	glTexCoord2f(1., 1.); glVertex3f(1, -1, -1);
	glTexCoord2f(0., 1.); glVertex3f(1, -1, 1);
	glEnd();

	glPopMatrix();
	a += 0.9;
	glutSwapBuffers();
}


void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}


void Reshape(int w, int h)
{
	GLfloat fAspect;
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("纹理映射");
	init(); // 读取文件生产纹理一定在glutCreateWindow（）之后！！！！！！！！！！
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}
