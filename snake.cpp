#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include<ctime>
#include<string>
#include <vector>
#include <GL/glut.h>


#define MAX_CHAR 128

using namespace std;

static int score = 0;

static int WIN_WIDTH = 500;
static int WIN_HEIGHT = 500;

static int  w = 20;
static double high[2] = { w, w };
static double low[2] = { -w, -w };


static double a = 1.0;


static int initialized = 0;
static int initialLength = 2;

static int sleeptime = 100;

static int power = 0;

enum direct { Left, Right, Up, Down };

class unit {
public:
	int ux;
	int uy;
	direct direction;
};

class bean {
public:
	int bx;
	int by;
};

class snake {
public:
	vector<unit> body;
};

static snake s;
static bean b;
static unit tail;
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void plotSolidRectangle(unit u, double r, double g, double b)
{
	glLineWidth(0.5);
	glColor3d(r, g, b);
	glBegin(GL_POLYGON);
	glVertex2d((double)u.ux, (double)u.uy);
	glVertex2d((double)u.ux + a, (double)u.uy);
	glVertex2d((double)u.ux + a, (double)u.uy + a);
	glVertex2d((double)u.ux, (double)u.uy + a);
	glEnd();
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void plotBean(bean be, double r, double g, double b)
{
	glLineWidth(0.5);
	glColor3d(r, g, b);
	glBegin(GL_POLYGON);
	glVertex2d((double)be.bx, (double)be.by);
	glVertex2d((double)be.bx + a, (double)be.by);
	glVertex2d((double)be.bx + a, (double)be.by + a);
	glVertex2d((double)be.bx, (double)be.by + a);
	glEnd();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void plotSnake(snake s) {
	for (int i = 0; i<s.body.size(); i++) {
		if (i == 0)
			plotSolidRectangle(s.body[i], 1.0, 0.0, 0.0);
		else
			plotSolidRectangle(s.body[i], 0.0, 0.0, 1.0);
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void plotBorder() {
	glLineWidth(1.0);
	glColor3d(1.0, 0.0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(-w + 1, -w);
	glVertex2d(-w + 1, w - 2);
	glVertex2d(w - 1, w - 2);
	glVertex2d(w - 1, -w);
	glEnd();
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void drawString(const char* str) //屏幕显示字体
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void move(void) {
	if (power == 0)
		return;
	if (s.body[0].ux>w - 2 || s.body[0].ux<-w + 1 || s.body[0].uy>w - 3 || s.body[0].uy<-w)
		return;

	for (int i = 1; i<s.body.size(); i++) {
		if (s.body[0].ux == s.body[i].ux && s.body[0].uy == s.body[i].uy) {
			return;
		}
	}


	if (abs(s.body[0].ux - b.bx) == 0 && abs(s.body[0].uy - b.by) == 0) {
		switch (s.body[s.body.size() - 1].direction) {
		case Left:
			tail.ux = s.body[s.body.size() - 1].ux + 1;
			tail.uy = s.body[s.body.size() - 1].uy;
			tail.direction = s.body[s.body.size() - 1].direction;
			s.body.push_back(tail);
			break;
		case Right:
			tail.ux = s.body[s.body.size() - 1].ux - 1;
			tail.uy = s.body[s.body.size() - 1].uy;
			tail.direction = s.body[s.body.size() - 1].direction;
			s.body.push_back(tail);
			break;
		case Up:
			tail.ux = s.body[s.body.size() - 1].ux;
			tail.uy = s.body[s.body.size() - 1].uy - 1;
			tail.direction = s.body[s.body.size() - 1].direction;
			s.body.push_back(tail);
			break;
		case Down:
			tail.ux = s.body[s.body.size() - 1].ux;
			tail.uy = s.body[s.body.size() - 1].uy + 1;
			tail.direction = s.body[s.body.size() - 1].direction;
			s.body.push_back(tail);
			break;
		}
		short flag = 0;
		while (flag == 0) {
			srand(time(0));
			b.bx = rand() % (2 * (w - 1)) - (w - 1);
			b.by = rand() % (2 * (w - 1)) - w;
			bool contact = false;
			for (int i = 0; i<s.body.size(); i++) {
				if (b.bx == s.body[i].ux && b.by == s.body[i].uy) {
					contact = true;
					break;
				}
			}
			if (contact == false)
				flag = 1;
		}
		score++;
	}


	switch (s.body[0].direction) {
	case Left:
		s.body[0].ux -= 1;
		break;
	case Right:
		s.body[0].ux += 1;
		break;
	case Up:
		s.body[0].uy += 1;
		break;
	case Down:
		s.body[0].uy -= 1;
		break;
	}

	//const int tmp = s.body.size();
	direct *dir = new direct[s.body.size()];
	dir[0] = s.body[0].direction;
	for (int i = 0; i<s.body.size() - 1; i++) {
		switch (s.body[i + 1].direction) {
		case Left:
			s.body[i + 1].ux -= 1;
			break;
		case Right:
			s.body[i + 1].ux += 1;
			break;
		case Up:
			s.body[i + 1].uy += 1;
			break;
		case Down:
			s.body[i + 1].uy -= 1;
			break;
		}
		dir[i + 1] = s.body[i + 1].direction;
	}
	for (int i = 0; i<s.body.size() - 1; i++)
		s.body[i + 1].direction = dir[i];

	delete[] dir;

	glutPostRedisplay();
	Sleep(sleeptime);

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void key_func(unsigned char k, int a, int b) {
	switch (k) {
	case ' ':
		power = 1 - power;
		break;
	case 27:
		exit(0);
		break;

	}
}
void SpecialKey(GLint key, GLint x, GLint y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		if (s.body[0].direction != Right)
			s.body[0].direction = Left;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		if (s.body[0].direction != Down)
			s.body[0].direction = Up;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		if (s.body[0].direction != Left)
			s.body[0].direction = Right;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if (s.body[0].direction != Up)
			s.body[0].direction = Down;
		glutPostRedisplay();
		break;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	plotSnake(s);

	plotBean(b, 0.0, 1.0, 0.5);

	plotBorder();

	glColor3f(0.0f, 0.0f, 0.0f);	 //设置字体颜色
	glRasterPos2i(-w + 17, w - 1);  //起始位置
	string str1 = "Score: ";
	string str2 = to_string(score);
	string str = str1 + str2;
	drawString(str.c_str());	 //输出的字符串

	//glFlush();
	glutSwapBuffers();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void initForOpenGLRendering(void)
{
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// specify the dimensions of the plot
	glOrtho(low[0], high[0], low[1], high[1], -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
//=============================================================================
void reshape(int width, int height)
{
	static float aspect_ratio = static_cast<float>(height) / width;
	if (height > (width*aspect_ratio)) {
		int offset_y = (height - width * aspect_ratio) / 2;
		glViewport(0, offset_y, width, width*aspect_ratio);
	}
	else {
		int offset_x = (width - height / aspect_ratio) / 2;
		glViewport(offset_x, 0, height / aspect_ratio, height);
	}

}

//=============================================================================
int main(int argc, char **argv)
{
	if (initialized == 0) {
		vector<unit> bo(initialLength);
		for (int i = 0; i<bo.size(); i++) {
			bo[i].ux = -int(w*0.4) - i;
			bo[i].uy = 0;
			bo[i].direction = Right;
		}
		s.body = bo;
		b.bx = int(w*0.2);
		b.by = 0;

		initialized = 1;

		cout << "游戏说明：" << endl;
		cout << "1、按空格开始，再按空格暂停" << endl;
		cout << "2、方向键控制方向" << endl;
		cout << "3、游戏结束后按“Esc”键退出" << endl;

	}
	glutInit(&argc, argv);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("贪吃蛇");
	initForOpenGLRendering();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(move);
	glutKeyboardFunc(key_func);
	glutSpecialFunc(SpecialKey);
	glutMainLoop();
	return EXIT_SUCCESS;
}
//=============================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
