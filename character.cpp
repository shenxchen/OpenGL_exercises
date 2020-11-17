#include <GL/glut.h>

GLubyte Font1[] =
{ 0xC3,
0xC3,
0xC3,
0xC3,
0xC3,
0xFF,
0xFF,
0xC0,
0xC0,
0xC0,
0xC0,
0xC0 }; // 'h'字符
GLubyte Font0[] =
{ 0xC3,0xC3,0xC3,0xC3,
0xC3,0xC3,0xC3,0xC3,
0xC3,0xC3,0xC3,0xC3,
0xC3,0xC3,0xC3,0xC3,
0xC3,0xC3,0xC3,0xC3,
0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,
0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xC0 }; // 'h'字符

void init()
{
	glClearColor(0, 0, 0, 1);
	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //!!非常重要，删除试试!!
	//    glPixelStorei (GL_UNPACK_SWAP_BYTES, false);	//对采用BGR格式而非RGB颜色格式的BMP图形有用；该例句目前对显示没影响
	//    glPixelStorei (GL_UNPACK_LSB_FIRST,true);	//颠倒位图的数据，11010 --> 01011,可看一下运行结果
	//    glPixelStorei (GL_UNPACK_SKIP_PIXELS, 3);	// 忽略前面的n个点不画（一行数据的整数倍），要相应修改glBitmap()函数中的图像高度值
	//    glPixelStorei (GL_UNPACK_SKIP_ROWS, 4);		// 忽略最前面的n行图像,不画,要相应修改glBitmap()函数中的图像高度值
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1, 0, 0); 
	glRasterPos3f(-40, -40, -90); 
	glBitmap(32, 12, 0, 0, 8, 24, Font0);
	glBitmap(8, 12, 0, 0, 0, 0, Font0);

	glColor3f(1, 1, 0);
	glRasterPos3f(-45, 40, -100); 
	glBitmap(8, 12, 0, 0, 8, 12, Font0);

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50, 50, -50, 50, -150, 150);
}

void main()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("bitmap font");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
