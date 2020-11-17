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
0xC0 }; // 'h'�ַ�
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
0xC0,0xC0,0xC0,0xC0 }; // 'h'�ַ�

void init()
{
	glClearColor(0, 0, 0, 1);
	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //!!�ǳ���Ҫ��ɾ������!!
	//    glPixelStorei (GL_UNPACK_SWAP_BYTES, false);	//�Բ���BGR��ʽ����RGB��ɫ��ʽ��BMPͼ�����ã�������Ŀǰ����ʾûӰ��
	//    glPixelStorei (GL_UNPACK_LSB_FIRST,true);	//�ߵ�λͼ�����ݣ�11010 --> 01011,�ɿ�һ�����н��
	//    glPixelStorei (GL_UNPACK_SKIP_PIXELS, 3);	// ����ǰ���n���㲻����һ�����ݵ�����������Ҫ��Ӧ�޸�glBitmap()�����е�ͼ��߶�ֵ
	//    glPixelStorei (GL_UNPACK_SKIP_ROWS, 4);		// ������ǰ���n��ͼ��,����,Ҫ��Ӧ�޸�glBitmap()�����е�ͼ��߶�ֵ
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
