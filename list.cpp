#include <gl\glut.h>

void CreateDisplayLists()
{
	//List 1 定义模型视图矩阵
	if (!glIsList((GLuint)1)) //判断编号1是否已经被用作显示列表
	{
		glNewList(1, GL_COMPILE); // 创建显示列表
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75, 1, 2, 50);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEndList();
	}

	//List 2 定义立方体实体
	if (!glIsList((GLuint)2)) //判断编号2是否已经被用作显示列表
	{
		glNewList(2, GL_COMPILE);

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 2, 0);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glEnd();

		glEndList();

	}
}

void myDisplay(void)
{
	glEnable(GL_DEPTH_TEST);   //深度缓存
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);

	glTranslatef(-10, 0, 0);
	glCallList(2); 
	glTranslatef(5, 0, 0);
	glCallList(2);
	glTranslatef(5, 0, 0);
	glCallList(2);
	glTranslatef(5, 0, 0);
	glCallList(2);
	glTranslatef(5, 0, 0);
	glCallList(2);

	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glCallList(1);
}

int main()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("显示列表");
	CreateDisplayLists();  //创建显示列表
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(myReshape);
	glutMainLoop();
	glDeleteLists(1, 2);
}