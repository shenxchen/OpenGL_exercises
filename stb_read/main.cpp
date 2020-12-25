#include <GL/glut.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

#include "3DS.h"

GLfloat fscale = 0.1;  // 模型绘制时的缩放比例！！

char FILE_NAME[] = "indoor plant_02.3ds";
UINT g_Texture[20] = { 0 }; // 纹理ID，有的模型可能多个纹理

CLoad3DS g_Load3ds; // CLoad3DS类处理3D模型读入
t3DModel g_3DModel; // 模型信息结构体对象



void CreateTexture(UINT textureArray[], const char *strFileName, int textureID)
{
	int width, height, nrChannels;
	unsigned char *datap = stbi_load(strFileName, &width, &height, &nrChannels, 0);
	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);
	// 设置像素对齐格式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, datap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	free(datap);
}


void Load3DSModel()
{
	g_Load3ds.Import3DS(&g_3DModel, FILE_NAME);			// 将3ds文件装入到模型结构体中
														// 遍历所有的材质
	for (int i = 0; i < g_3DModel.numOfMaterials; i++)
	{	// 判断是否是一个文件名
		if (strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{  //  使用纹理文件名称来装入位图
			CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);
		}
		// 设置材质的纹理ID
		g_3DModel.pMaterials[i].texureId = i;
	}

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	// 开启深度检测功能；
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	//	glEnable(GL_DITHER);
	glCullFace(GL_BACK); // 剔除反面，否则模型会闪烁
	glEnable(GL_CULL_FACE);

}

void Draw3DModel()
{	// 遍历模型中所有的对象
	for (int i = 0; i < g_3DModel.numOfObjects; i++)
	{   // 如果对象的大小小于0，则退出
		if (g_3DModel.pObject.size() <= 0) break;
		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel.pObject[i];
		// 判断该对象是否有纹理映射
		if (pObject->bHasTexture) {
			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);
		}
		else {
			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		// 开始以g_ViewMode模式绘制
		glBegin(GL_TRIANGLES);
		// 遍历所有的面
		for (int j = 0; j < pObject->numOfFaces; j++)
		{	// 遍历三角形的所有点
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{	// 获得面对每个点的索引
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				// 给出法向量
				glNormal3f(pObject->pNormals[index].x, pObject->pNormals[index].y, pObject->pNormals[index].z);
				// 如果对象具有纹理
				if (pObject->bHasTexture) {
					// 确定是否有UVW纹理坐标
					if (pObject->pTexVerts)
						glTexCoord2f(pObject->pTexVerts[index].x, pObject->pTexVerts[index].y);
				}
				else {
					if (g_3DModel.pMaterials.size() && pObject->materialID >= 0)
					{
						BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				glVertex3f(pObject->pVerts[index].x, pObject->pVerts[index].y, pObject->pVerts[index].z);
			}
		}
		glEnd();// 绘制结束
	}
}

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 3, 0, 0, 0, 0.0f, 1.0f, 0.0f);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glTranslatef(0.0, 0.0, -3.6);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	/*  定义纹理 */
	// 注意GL_BGR_EXT，BMP采用的是蓝绿红存储
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, datap);

	/*  控制滤波 *//*
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	*/

	/*  说明映射方式*/
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/*  启动纹理映射 */
	//glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_SMOOTH /*GL_FLAT*/);
	/*
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
	glEnd(); 
	*/

	glScalef(fscale, fscale, fscale); // 设置模型大小比例
	Draw3DModel(); // 画3D模型

	glFlush();
}


void Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w / (GLfloat)h, 1.0, 30.0);
	
}


int main()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Texture");
	Load3DSModel();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}
