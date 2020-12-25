#include <GL/glut.h>
#include "BMPLoad.h"

#include "3DS.H"
GLfloat fscale=0.8;  // ģ�ͻ���ʱ�����ű�������

//#define FILE_NAME    "shark1.3ds" //"shark1.3ds" "����.3ds"	"���췢��̨.3ds"	"�γ�0.3ds"   "�ɻ�1.3ds"  ֱ����0.3DS    "FACE.3DS"	lamp.3DS
char FILE_NAME[] = "�ɻ�1.3ds";
UINT g_Texture[20] = {0}; // ����ID���е�ģ�Ϳ��ܶ������

CLoad3DS g_Load3ds; // CLoad3DS�ദ��3Dģ�Ͷ���
t3DModel g_3DModel; // ģ����Ϣ�ṹ�����

float angle = 0.0f; // �Ƕȣ�������תģ��

//  ���ļ��д�������
void CreateTexture(UINT textureArray[], const char *strFileName, int textureID)
{
    BMPPic * pBitmap;
    pBitmap = ZXCLoadBMP(strFileName);
	// ��������
	glGenTextures(1, &textureArray[textureID]);
	// �������ض����ʽ
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->iwidth, pBitmap->iheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBitmap->pByte);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if (pBitmap)// �ͷ�λͼռ�õ���Դ
	{  if (pBitmap->pByte)
        free(pBitmap->pByte);
	}
}


void Load3DSModel()
{	g_Load3ds.Import3DS(&g_3DModel, FILE_NAME);			// ��3ds�ļ�װ�뵽ģ�ͽṹ����
	// �������еĲ���
	for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{	// �ж��Ƿ���һ���ļ���
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
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

void Reshape(int width, int Height)
{
	Height = (Height == 0 ? 1: Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width/Height,0.1,1000);
	glViewport(0,0,width,Height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Draw3DModel()
{	// ����ģ�������еĶ���
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{   // �������Ĵ�СС��0�����˳�
		if(g_3DModel.pObject.size() <= 0) break;
		// ��õ�ǰ��ʾ�Ķ���
		t3DObject *pObject = &g_3DModel.pObject[i];
		// �жϸö����Ƿ�������ӳ��
		if(pObject->bHasTexture) {
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
		for(int j = 0; j < pObject->numOfFaces; j++)
		{	// ���������ε����е�
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{	// ������ÿ���������
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				// ����������
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				// ��������������
				if(pObject->bHasTexture) {
					// ȷ���Ƿ���UVW��������
					if(pObject->pTexVerts)
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
				}
				else {
					if(g_3DModel.pMaterials.size() && pObject->materialID >= 0)
					{
						BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}
		glEnd();// ���ƽ���
	}
}


void Display()
{	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
	gluLookAt(0., 0. , 0,  0.,0,-50, 0,1,0);
	glTranslatef(0,0,-50);
	glRotatef(angle,0,1,0);

	glScalef(fscale,fscale,fscale); // ����ģ�ʹ�С����
	Draw3DModel(); // ��3Dģ��

    glPopMatrix();
	glutSwapBuffers();
}

void TimerFun(int value)
{	angle +=0.5;
	glutPostRedisplay();
	glutTimerFunc(20,TimerFun,1);
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowPosition(200,200);
	glutInitWindowSize(500,500);
	glutCreateWindow("��ȡ3DS��ʽģ��");

	glClearColor(0,0,0,1);
	Load3DSModel();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutTimerFunc(10,TimerFun,1);

	glutMainLoop();
}
