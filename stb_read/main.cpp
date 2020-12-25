#include <GL/glut.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

#include "3DS.h"

GLfloat fscale = 0.1;  // ģ�ͻ���ʱ�����ű�������

char FILE_NAME[] = "indoor plant_02.3ds";
UINT g_Texture[20] = { 0 }; // ����ID���е�ģ�Ϳ��ܶ������

CLoad3DS g_Load3ds; // CLoad3DS�ദ��3Dģ�Ͷ���
t3DModel g_3DModel; // ģ����Ϣ�ṹ�����



void CreateTexture(UINT textureArray[], const char *strFileName, int textureID)
{
	int width, height, nrChannels;
	unsigned char *datap = stbi_load(strFileName, &width, &height, &nrChannels, 0);
	// ��������
	glGenTextures(1, &textureArray[textureID]);
	// �������ض����ʽ
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, datap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	free(datap);
}


void Load3DSModel()
{
	g_Load3ds.Import3DS(&g_3DModel, FILE_NAME);			// ��3ds�ļ�װ�뵽ģ�ͽṹ����
														// �������еĲ���
	for (int i = 0; i < g_3DModel.numOfMaterials; i++)
	{	// �ж��Ƿ���һ���ļ���
		if (strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{  //  ʹ�������ļ�������װ��λͼ
			CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);
		}
		// ���ò��ʵ�����ID
		g_3DModel.pMaterials[i].texureId = i;
	}

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	// ������ȼ�⹦�ܣ�
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	//	glEnable(GL_DITHER);
	glCullFace(GL_BACK); // �޳����棬����ģ�ͻ���˸
	glEnable(GL_CULL_FACE);

}

void Draw3DModel()
{	// ����ģ�������еĶ���
	for (int i = 0; i < g_3DModel.numOfObjects; i++)
	{   // �������Ĵ�СС��0�����˳�
		if (g_3DModel.pObject.size() <= 0) break;
		// ��õ�ǰ��ʾ�Ķ���
		t3DObject *pObject = &g_3DModel.pObject[i];
		// �жϸö����Ƿ�������ӳ��
		if (pObject->bHasTexture) {
			// ������ӳ��
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);
		}
		else {
			// �ر�����ӳ��
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		// ��ʼ��g_ViewModeģʽ����
		glBegin(GL_TRIANGLES);
		// �������е���
		for (int j = 0; j < pObject->numOfFaces; j++)
		{	// ���������ε����е�
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{	// ������ÿ���������
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				// ����������
				glNormal3f(pObject->pNormals[index].x, pObject->pNormals[index].y, pObject->pNormals[index].z);
				// ��������������
				if (pObject->bHasTexture) {
					// ȷ���Ƿ���UVW��������
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
		glEnd();// ���ƽ���
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

	/*  �������� */
	// ע��GL_BGR_EXT��BMP���õ������̺�洢
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, datap);

	/*  �����˲� *//*
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	*/

	/*  ˵��ӳ�䷽ʽ*/
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/*  ��������ӳ�� */
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

	glScalef(fscale, fscale, fscale); // ����ģ�ʹ�С����
	Draw3DModel(); // ��3Dģ��

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
