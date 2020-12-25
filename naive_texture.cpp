//#include <windows.h>
#include "GL\glut.h"

/* 创建纹理 */
#define ImageWidth 64
#define ImageHeight 64
GLubyte Image[ImageWidth][ImageHeight][3];
GLubyte ImageA[ImageWidth][ImageHeight][3];

GLuint texture[2];

void makeImage(void)
{
	int i, j, r, g, b;

	for (i = 0; i < ImageWidth; i++)
	{
		for (j = 0; j < ImageHeight; j++)
		{
			r = (i*j) % 255;
			g = (4 * i) % 255;
			b = (4 * j) % 255;

			Image[i][j][0] = (GLubyte)r;
			Image[i][j][1] = (GLubyte)g;
			Image[i][j][2] = (GLubyte)b;

			ImageA[i][j][0] = 255 - (GLubyte)r;
			ImageA[i][j][1] = 255 - (GLubyte)g;
			ImageA[i][j][2] = 255 - (GLubyte)b;
		}
	}
}


void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	makeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*  定义纹理11111111111111111 */
	glGenTextures(2, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &Image[0][0][0]);
	/*  纹理1的控制滤波 */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_REPEAT*/GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/); // 修改 试一下
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/*  定义纹理22222222222222222 */
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &ImageA[0][0][0]);
	/*  纹理2的控制滤波，每个纹理都要重新设置一下如下参数!!!!!!!!!!!!!!!!!!! */
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_REPEAT*/GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR /*GL_NEAREST*/); // 修改 试一下
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/*  说明映射方式*/
	//   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/*  启动纹理映射 */
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH /*GL_FLAT*/);
}



void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -4);

	glBindTexture(GL_TEXTURE_2D, texture[0]); // 选择第1幅图片，绘制物体**********************
											  /*  说明映射方式*/
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/*  设置纹理坐标和物体几何坐标:绘制了两个平面，一个是正对镜头平面，另一个是斜面 */
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);// 选择第2幅图片，绘制物体**********************
											 /*  说明映射方式*/
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();
	glFlush();
}

void Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Texture ");
	myinit();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	//	glutIdleFunc(Move);
	glutMainLoop();
}

