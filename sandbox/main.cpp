#include "GL\glut.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

GLuint textPic[10];	//纹理编号

//需要导入的纹理图片文件名称
char FileName1[] = "ni.jpg";
char FileName2[] = "grass.jpg";
char FileName3[] = "cloud.png";
char FileName4[] = "stone.jpg";
char FileName5[] = "wood.bmp";

//储存几张图片的宽、高和通道
int width[10];
int height[10];
int nrChannels[10];

//导入纹理图片，存于指针
unsigned char *data1 = stbi_load(FileName1, &width[0], &height[0], &nrChannels[0], 0);
unsigned char *data2 = stbi_load(FileName2, &width[1], &height[1], &nrChannels[1], 0);
unsigned char *data3 = stbi_load(FileName3, &width[2], &height[2], &nrChannels[2], 0);
unsigned char *data4 = stbi_load(FileName4, &width[3], &height[3], &nrChannels[3], 0);
unsigned char *data5 = stbi_load(FileName5, &width[4], &height[4], &nrChannels[4], 0);

static const int L = 60;	//世界大小
static const int undergroud = 5;	//地下深度
static const float h = 1.5;	//人眼高度

//视角旋转参数
static float a = 0.1;	
static float l = 0.05;

//眼睛位置
static float ex = L / 2.0;
static float ey = L / 2.0;
static float ez = h;

//视点位置
static float cx = L / 2.0;
static float cy = L / 2.0 + 5;
static float cz = h;

//记录格点位置
int *loc = new int[3];

enum material
{
	EMPTY,
	GROUND,
	STONE,
	WOOD,
	WATER
};	//方块材料类型

class Block {
public:
	//位置
	int x = 0;
	int y = 0;
	int z = 0;
	//类型
	material type=EMPTY;
};	//定义方块类

Block ***all_blocks = new Block**[L];	//所有方块数组

//初始化方块数组（每个方块的位置和类型）
void init_blocks(Block ***all_blocks) {
	for (int i = 0; i <L; i++) {
		all_blocks[i] = new Block*[L];
		for (int j = 0; j < L; j++) {
			all_blocks[i][j] = new Block[20];
		}
	}
	
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			for (int k = 0; k < 20; k++) {
				all_blocks[i][j][k].x = i;
				all_blocks[i][j][k].y = j;
				all_blocks[i][j][k].z = k - undergroud;
			}
		}
	}
	
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			for (int k = undergroud; k > -0.5; k--) {
				all_blocks[i][j][k].type = GROUND;
			}
		}
	}
	
	for (int i = 23; i < 33; i++) {
		for (int j = 40; j < 50; j++) {
			all_blocks[i][j][1+undergroud].type = STONE;
		}
	}
	for (int k = 2; k < 5; k++) {
		all_blocks[27][45][k + undergroud].type = STONE;
		
	}

	for (int k = 1; k < 3; k++) {
		for (int i = 14; i < 21; i++) {
			all_blocks[i][23][k + undergroud].type = WOOD;
		}
		for (int i = 14; i < 21; i++) {
			all_blocks[i][33][k + undergroud].type = WOOD;
		}
		for (int j = 23; j < 33; j++) {
			all_blocks[14][j][k + undergroud].type = WOOD;
		}
		for (int j = 23; j < 33; j++) {
			all_blocks[20][j][k + undergroud].type = WOOD;
		}
		all_blocks[20][27][k + undergroud].type = EMPTY;
		all_blocks[20][28][k + undergroud].type = EMPTY;
	}

	for (int i = 35; i < 44; i++) {
		for (int j = 35; j < 44; j++) {
			all_blocks[i][j][undergroud].type = WATER;
		}
	}

	for (int i = 25; i < 31; i++) {
		for (int j = 12; j < 17; j++) {
			for (int k = undergroud; k > 2; k--) {
				all_blocks[i][j][k].type = EMPTY;
			}
		}
	}

	for (int i = 35; i < 55; i++) {
		for (int j = 7; j < 27; j++) {
			all_blocks[i][j][undergroud + 1].type = GROUND;
		}
	}

	for (int i = 37; i < 55; i++) {
		for (int j = 7; j < 25; j++) {
			all_blocks[i][j][undergroud + 2].type = GROUND;
		}
	}

	for (int i = 38; i < 55; i++) {
		for (int j = 7; j < 24; j++) {
			all_blocks[i][j][undergroud + 3].type = GROUND;
		}
	}

	for (int i = 40; i < 55; i++) {
		for (int j = 7; j < 22; j++) {
			all_blocks[i][j][undergroud + 4].type = GROUND;
		}
	}

	for (int i = 41; i < 55; i++) {
		for (int j = 7; j < 21; j++) {
			all_blocks[i][j][undergroud + 5].type = GROUND;
		}
	}

	for (int i = 43; i < 55; i++) {
		for (int j = 7; j < 19; j++) {
			all_blocks[i][j][undergroud + 6].type = GROUND;
		}
	}

	for (int i = 47; i < 55; i++) {
		for (int j = 7; j < 14; j++) {
			all_blocks[i][j][undergroud + 7].type = GROUND;
		}
	}

}

//画单个方块
void draw(Block block) {
	int i = block.x;
	int j = block.y;
	int k = block.z;
	switch (block.type)
	{
	case GROUND:
		glBindTexture(GL_TEXTURE_2D, textPic[0]);
		glBegin(GL_QUADS); // 前面
		glColor4f(1, 1, 1, 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glEnd();
		glBegin(GL_QUADS); // hou
		glColor4f(1, 1, 1, 1);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 1, k + 0);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 0, j + 1, k - 1);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // zuo
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, -1, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(0, -1, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(0, -1, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 0, j + 0, k - 1);
		glNormal3f(0, -1, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // you
		glColor4f(1, 1, 1, 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 1, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 1, k - 1);
		glEnd();
		glBegin(GL_QUADS); // xia
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k - 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 1, k - 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, textPic[1]);
		glBegin(GL_QUADS); // shang
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 1, k + 0);
		glEnd();
		break;

	case STONE:
		glBindTexture(GL_TEXTURE_2D, textPic[3]);
		glBegin(GL_QUADS); // 前面
		glColor4f(1, 1, 1, 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glEnd();
		glBegin(GL_QUADS); // hou
		glColor4f(1, 1, 1, 1);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 1, k + 0);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 0, j + 1, k - 1);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // zuo
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, -1, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(0, -1, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(0, -1, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 0, j + 0, k - 1);
		glNormal3f(0, -1, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // you
		glColor4f(1, 1, 1, 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 1, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(1, 0, 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 1, k - 1);
		glEnd();
		glBegin(GL_QUADS); // xia
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k - 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 1, k - 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // shang
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 1, k + 0);
		glEnd();
		break;

	case WOOD:
		glBindTexture(GL_TEXTURE_2D, textPic[4]);
		glBegin(GL_QUADS); // 前面
		glColor4f(1, 1, 1, 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 1, j + 1, k + 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glEnd();
		glBegin(GL_QUADS); // hou
		glColor4f(1, 1, 1, 1);
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 1, k + 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 0, j + 1, k - 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // zuo
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, -1, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 0, j + 0, k - 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // you
		glColor4f(1, 1, 1, 1);
		glNormal3f(1, 0, 0);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 1, k + 0);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 1, k + 0);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 1, k - 1);
		glEnd();
		glBegin(GL_QUADS); // xia
		glColor4f(1, 1, 1, 1);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k - 1);
		glTexCoord2f(1., 0.); glVertex3f(i + 0, j + 1, k - 1);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k - 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 1, j + 0, k - 1);
		glEnd();
		glBegin(GL_QUADS); // shang
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0., 0.); glVertex3f(i + 0, j + 0, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(1., 0.); glVertex3f(i + 1, j + 0, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(1., 1.); glVertex3f(i + 1, j + 1, k + 0);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0., 1.); glVertex3f(i + 0, j + 1, k + 0);
		glNormal3f(0, 0, 1);
		glEnd();
		break;

	case WATER:
		
		glBegin(GL_QUADS); // 前面
		glColor3f(0, 0.5, 1);
		glNormal3f(1, 0, 0);
		glVertex3f(i + 1, j + 1, k + 0);
		glVertex3f(i + 1, j + 0, k + 0);
		glVertex3f(i + 1, j + 0, k - 1);
		glVertex3f(i + 1, j + 1, k - 1);
		 // hou
		glNormal3f(-1, 0, 0);
		glVertex3f(i + 0, j + 0, k + 0);
		glVertex3f(i + 0, j + 1, k + 0);
		glVertex3f(i + 0, j + 1, k - 1);
		glVertex3f(i + 0, j + 0, k - 1);
		 // zuo
		glNormal3f(0, -1, 0);
		glVertex3f(i + 1, j + 0, k + 0);
		glVertex3f(i + 0, j + 0, k + 0);
		glVertex3f(i + 0, j + 0, k - 1);
		glVertex3f(i + 1, j + 0, k - 1);
		 // you
		glNormal3f(1, 0, 0);
		glVertex3f(i + 0, j + 1, k + 0);
		glVertex3f(i + 1, j + 1, k + 0);
		glVertex3f(i + 1, j + 1, k - 1);
		glVertex3f(i + 0, j + 1, k - 1);
		 // xia
		glNormal3f(0, 0, -1);
		glVertex3f(i + 0, j + 0, k - 1);
		glVertex3f(i + 0, j + 1, k - 1);
		glVertex3f(i + 1, j + 1, k - 1);
		glVertex3f(i + 1, j + 0, k - 1);
		 // shang
		glNormal3f(0, 0, 1);
		glVertex3f(i + 0, j + 0, k + 0);
		glVertex3f(i + 1, j + 0, k + 0);
		glVertex3f(i + 1, j + 1, k + 0);
		glVertex3f(i + 0, j + 1, k + 0);
		glEnd();
		glPopMatrix();
		break;
	}
	

}

//画出所有方块
void draw_all(Block ***all_blocks) {
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			for (int k = 0; k < 20; k++) {
				draw(all_blocks[i][j][k]);
			}
		}
	}
}

//画云
void draw_cloud(float x, float y, float z) {
	glBindTexture(GL_TEXTURE_2D, textPic[2]);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS); 
	glNormal3f(0, 0, -1);
	glTexCoord2f(0., 0.); glVertex3f(x + 0, y + 0, z + 0);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1., 0.); glVertex3f(x + 10, y + 0, z + 0);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1., 1.); glVertex3f(x + 10, y + 10, z + 0);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0., 1.); glVertex3f(x + 0, y + 10, z + 0);
	glEnd();
}

//使用显示列表
void CreateDisplayLists()
{
	//List 1 画出所有方块
	if (!glIsList((GLuint)1)) //判断编号1是否已经被用作显示列表
	{
		glNewList(1, GL_COMPILE); // 创建显示列表

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		draw_all(all_blocks);

		glEndList();
	}

	//List 2 画云
	if (!glIsList((GLuint)2)) //判断编号2是否已经被用作显示列表
	{
		glNewList(2, GL_COMPILE);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		srand(unsigned(time(0)));
		
		for (int i = 0; i < 200; i++) {
			int r1 = (rand() % (2 * L)) - L / 2;
			int r2 = (rand() % (2 * L)) - L / 2;
			int r3 = 10 + rand() % 3;
			draw_cloud(r1, r2, r3);
		}
		
		glEndList();
	}
}

//初始化纹理映射
void init_tex()
{
	glGenTextures(5,textPic);

	glBindTexture(GL_TEXTURE_2D, textPic[0]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data1); // 图片存储到显卡中
	free(data1); // 释放图片所占计算机内存
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textPic[1]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data2);// 图片存储到显卡中
	free(data2);  // 释放图片所占计算机内存
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textPic[2]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width[2], height[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, data3);// 图片存储到显卡中
	free(data3);  // 释放图片所占计算机内存
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glBindTexture(GL_TEXTURE_2D, textPic[3]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[3], height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, data4);// 图片存储到显卡中
	free(data4);  // 释放图片所占计算机内存
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textPic[4]);
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[4], height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, data5);// 图片存储到显卡中
	free(data5);  // 释放图片所占计算机内存
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
}

void disp(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ex, ey, ez, cx, cy, cz, 0.0f, 0.0f, 1.0f);

	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);	//背景天空为蓝色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//------------------------光照--------------------------------
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat diffuse[] = { 1,1,1,1 };
	GLfloat specular[] = { 1,1,1,1 };
	GLfloat position0[] = { 30,30,7,1 };
	GLfloat lmodel_ambient[] = { 1,1,1,1 };
	float m_diffuse[] = { 1,1,1,1 };
	float m_specular[] = { 0,0,0,1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	
	//------------背面剔除、深度检测、融合、反走样----------------
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	//------------------召唤显示列表，画出场景--------------------
	glCallList(2);
	glCallList(1);
	
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	float ratio = 1.0 * width / height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 0.1, 50);
}

//输入眼睛精确位置，输出所在格点位置
void which_block(float ex,float ey,float ez, int *loc) {
	int i = floor(ex);
	int j = floor(ey);
	int k = ceil(ez);
	loc[0] = i;
	loc[1] = j;
	loc[2] = k + undergroud;
}

//=======================在场景中跳跃=========================
float g = 9; //重力加速度
float dt = 0.1; //离散时间间隔

int jump_flag = 0; //是否跳起的判据
float v = 4;	//起跳初速度
float v0 = 0;	//不起跳，初速度为零
void jump() {
	if (jump_flag == 1) {
		float ez_next = ez + v * dt;

		which_block(ex, ey, ez_next-h, loc);
		int i = loc[0]; int j = loc[1]; int k = loc[2];
		if (all_blocks[i][j][k].type == EMPTY || all_blocks[i][j][k].type == WATER) {
			cz += (ez_next - ez);
			ez = ez_next;
			glutPostRedisplay();
			v -= g * dt;
		}
		else {
			float ez_n = floor(ez) + 0.5;

			cz += (ez_n - ez);
			ez = ez_n;
			glutPostRedisplay();
			jump_flag = 0;
			v = 4;
		}
	}
	else {
		float ez_next = ez + (v0 - g * dt) *dt;

		which_block(ex, ey, ez_next - h, loc);
		int i = loc[0]; int j = loc[1]; int k = loc[2];
		if (all_blocks[i][j][k].type == EMPTY || all_blocks[i][j][k].type == WATER) {
			cz += (ez_next - ez);
			ez = ez_next;
			glutPostRedisplay();
			v0 -= g * dt;
		}
		else {
			float ez_n = floor(ez) + 0.5;

			cz += (ez_n - ez);
			ez = ez_n;
			glutPostRedisplay();
			v0 = 0;
		}
	}
}

//================键盘控制位置移动和视野方向==================
void key_func(unsigned char key, int x, int y) {
	float t1, t2;
	switch (key)
	{
		float ex_next, ey_next;
		int i, j, k;
		bool b1, b2;

	case ' ':
		jump_flag = 1;
		break;

	case 27:
		exit(1);
		break;

	case 'a':
	case 'A':
		t1 = -l * (cy - ey);
		t2 = l * (cx - ex);
		ex_next = ex + 2 * t1;
		ey_next = ey + 2 * t2;
		which_block(ex_next, ey_next, ez, loc);
		i = loc[0]; j = loc[1]; k = loc[2];
		if (i >= 0 && i < L && j >= 0 && j < L && k >= 0 && k < L) {
			b1 = all_blocks[i][j][k].type == EMPTY || all_blocks[i][j][k].type == WATER;
			b2 = all_blocks[i][j][k - 1].type == EMPTY || all_blocks[i][j][k-1].type == WATER;
			if (b1 && b2) {
				ex += t1;
				ey += t2;
				cx += t1;
				cy += t2;
				glutPostRedisplay();
			}
		}
		break;

	case 'd':
	case 'D':
		t1 = l * (cy - ey);
		t2 = -l * (cx - ex);
		ex_next = ex + 2 * t1;
		ey_next = ey + 2 * t2;
		which_block(ex_next, ey_next, ez, loc);
		i = loc[0]; j = loc[1]; k = loc[2];
		if (i >= 0 && i < L && j >= 0 && j < L && k >= 0 && k < L) {
			b1 = all_blocks[i][j][k].type == EMPTY || all_blocks[i][j][k].type == WATER;
			b2 = all_blocks[i][j][k - 1].type == EMPTY || all_blocks[i][j][k-1].type == WATER;
			if (b1 && b2) {
				ex += t1;
				ey += t2;
				cx += t1;
				cy += t2;
				glutPostRedisplay();
			}
		}
		break;

	case 'w':
	case 'W':
		t1 = l * (cx - ex);
		t2 = l * (cy - ey);
		ex_next = ex + 2 * t1;
		ey_next = ey + 2 * t2;
		which_block(ex_next, ey_next, ez, loc);
		i = loc[0]; j = loc[1]; k = loc[2];
		if (i >= 0 && i < L && j >= 0 && j < L && k >= 0 && k < L) {
			b1 = all_blocks[i][j][k].type == EMPTY || all_blocks[i][j][k].type == WATER;
			b2 = all_blocks[i][j][k - 1].type == EMPTY || all_blocks[i][j][k-1].type == WATER;
			if (b1 && b2) {
				ex += t1;
				ey += t2;
				cx += t1;
				cy += t2;
				glutPostRedisplay();
			}
		}
		break;

	case 's':
	case 'S':
		t1 = -l * (cx - ex);
		t2 = -l * (cy - ey);
		ex_next = ex + 2 * t1;
		ey_next = ey + 2 * t2;
		which_block(ex_next, ey_next, ez, loc);
		i = loc[0]; j = loc[1]; k = loc[2];
		if (i >= 0 && i < L && j >= 0 && j < L && k >= 0 && k < L) {
			b1 = all_blocks[i][j][k].type == EMPTY || all_blocks[i][j][k].type == WATER;
			b2 = all_blocks[i][j][k - 1].type == EMPTY || all_blocks[i][j][k-1].type == WATER;
			if (b1 && b2) {
				ex += t1;
				ey += t2;
				cx += t1;
				cy += t2;
				glutPostRedisplay();
			}
		}
		break;
	}
}

void special_key(int key, int x, int y)
{
	float t1, t2;
	switch (key)
	{
	case GLUT_KEY_UP:
		cz += 0.2;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		cz -= 0.2;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		t1 = ex + (cx - ex) * cos(a) - (cy - ey) * sin(a);
		t2 = ey + (cx - ex) * sin(a) + (cy - ey) * cos(a);
		cx = t1;
		cy = t2;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		t1 = ex + (cx - ex) * cos(a) + (cy - ey) * sin(a);
		t2 = ey + -(cx - ex) * sin(a) + (cy - ey) * cos(a);
		cx = t1;
		cy = t2;
		glutPostRedisplay();
		break;
	}
}

static bool left_button_is_pressed;
static float press_x, press_y, mouse_x, mouse_y;
static float angle_h = 0, angle_v = 0;
static float a_h = 0, a_v = 0;
void mouse_func(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button_is_pressed = true;
		press_x = x;
		press_y = y;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button_is_pressed = false;
		a_h += angle_h;
		a_v += angle_v;
	}
}

void motion_func(int x, int y)
{
	if (left_button_is_pressed == true) {
		mouse_x = x;
		mouse_y = y;
		angle_h = -(mouse_x - press_x) / 500.0;
		angle_v = (mouse_y - press_y) / 500.0;

		cx = ex + sin(a_h + angle_h);
		cy = ey + 5 * cos(a_h + angle_h);
		cz = ez + a_v + angle_v;
		
		glutPostRedisplay();
	}
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("sandbox");

	init_blocks(all_blocks);
	init_tex(); // 读取文件生产纹理一定在glutCreateWindow（）之后
	CreateDisplayLists();
	glutReshapeFunc(reshape);
	glutDisplayFunc(disp);
	glutKeyboardFunc(key_func);
	glutSpecialFunc(special_key);
	glutIdleFunc(jump);
	//glutMouseFunc(mouse_func);
	//glutMotionFunc(motion_func);
	
	glutMainLoop();

	return 0;
}
