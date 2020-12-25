#include "GL\glut.h"

#include "BMPLoad.h"
BMPPic * PicWood;
BMPPic * PicKuang;
GLuint textPic[2];

void init()
{
	PicWood = ZXCLoadBMP("ľ��.bmp"); //house1016.BMP�����1016��ÿ��վ1016*3=3048���ֽڣ����ֽ����ÿ��Ա�1,2,4,8������������Alignment�������ĸ�ֵ������!!
	glGenTextures(2, textPic);
	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, PicWood->iwidth, PicWood->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, PicWood->pByte); // ͼƬ�洢���Կ���
	free(PicWood->pByte); // �ͷ�ͼƬ��ռ������ڴ�
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textPic[1]);
	PicKuang = ZXCLoadBMP("kuang.bmp"); //house1016.BMP�����1016��ÿ��ռ1016*3=3048���ֽڣ����ֽ����ÿ��Ա�1,2,4,8������������Alignment�������ĸ�ֵ������!!
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, PicKuang->iwidth, PicKuang->iheight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, PicKuang->pByte);// ͼƬ�洢���Կ���
	free(PicKuang->pByte);  // �ͷ�ͼƬ��ռ������ڴ�

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
	// ���ȿ�����ʾһ�¶�ȡ��BMPͼ���ļ����������ʾ��˵����ȡ�ļ�û������******************************************
	//glRasterPos3f(-40,-41,-100);
	//glPixelStoref(GL_UNPACK_ALIGNMENT,1);
	//	printf("%d %d %d %d %x",tgaPic->iWidth , tgaPic->iHeight, tgaPic->eFormat,tgaPic->iComponents,*(tgaPic->pBytes)));
	//glDrawPixels(PicWood->iwidth , PicWood->iheight, GL_BGR_EXT,GL_UNSIGNED_BYTE,PicWood->pByte);

	// ����������������ӳ�����**************************************************************
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(a, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glBegin(GL_QUADS); // ǰ��
	glTexCoord2f(0., 0.); glVertex3f(-1, -1, 1);
	glTexCoord2f(1., 0.); glVertex3f(1, -1, 1);
	glTexCoord2f(1., 1.); glVertex3f(1, 1, 1);
	glTexCoord2f(0., 1.); glVertex3f(-1, 1, 1);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(1, -1, -1);
	glTexCoord2f(1., 0.); glVertex3f(-1, -1, -1);
	glTexCoord2f(1., 1.); glVertex3f(-1, 1, -1);
	glTexCoord2f(0., 1.); glVertex3f(1, 1, -1);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(-1, -1, -1);
	glTexCoord2f(1., 0.); glVertex3f(-1, -1, 1);
	glTexCoord2f(1., 1.); glVertex3f(-1, 1, 1);
	glTexCoord2f(0., 1.); glVertex3f(-1, 1, -1);
	glEnd();
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(1, -1, 1);
	glTexCoord2f(1., 0.); glVertex3f(1, -1, -1);
	glTexCoord2f(1., 1.); glVertex3f(1, 1, -1);
	glTexCoord2f(0., 1.); glVertex3f(1, 1, 1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textPic[1]);
	glBegin(GL_QUADS); // ����
	glTexCoord2f(0., 0.); glVertex3f(-1, 1, 1);
	glTexCoord2f(1., 0.); glVertex3f(1, 1, 1);
	glTexCoord2f(1., 1.); glVertex3f(1, 1, -1);
	glTexCoord2f(0., 1.); glVertex3f(-1, 1, -1);
	glEnd();
	glBegin(GL_QUADS); // ����
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
	glutCreateWindow("����ӳ��");
	init(); // ��ȡ�ļ���������һ����glutCreateWindow����֮�󣡣�����������������
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(10, TimerFunction, 1);
	glutMainLoop();
}
