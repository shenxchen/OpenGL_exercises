#include <GL/glut.h>
#include "BMPLoad.h"

#include "3DS.H"
GLfloat fscale=0.8;  // 模型绘制时的缩放比例！！

//#define FILE_NAME    "shark1.3ds" //"shark1.3ds" "铲车.3ds"	"航天发射台.3ds"	"轿车0.3ds"   "飞机1.3ds"  直升机0.3DS    "FACE.3DS"	lamp.3DS
char FILE_NAME[] = "飞机1.3ds";
UINT g_Texture[20] = {0}; // 纹理ID，有的模型可能多个纹理

CLoad3DS g_Load3ds; // CLoad3DS类处理3D模型读入
t3DModel g_3DModel; // 模型信息结构体对象

float angle = 0.0f; // 角度，动画旋转模型

//  从文件中创建纹理
void CreateTexture(UINT textureArray[], const char *strFileName, int textureID)
{
    BMPPic * pBitmap;
    pBitmap = ZXCLoadBMP(strFileName);
	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);
	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->iwidth, pBitmap->iheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBitmap->pByte);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	if (pBitmap)// 释放位图占用的资源
	{  if (pBitmap->pByte)
        free(pBitmap->pByte);
	}
}


void Load3DSModel()
{	g_Load3ds.Import3DS(&g_3DModel, FILE_NAME);			// 将3ds文件装入到模型结构体中
	// 遍历所有的材质
	for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{	// 判断是否是一个文件名
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
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
{	// 遍历模型中所有的对象
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{   // 如果对象的大小小于0，则退出
		if(g_3DModel.pObject.size() <= 0) break;
		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel.pObject[i];
		// 判断该对象是否有纹理映射
		if(pObject->bHasTexture) {
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
		for(int j = 0; j < pObject->numOfFaces; j++)
		{	// 遍历三角形的所有点
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{	// 获得面对每个点的索引
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				// 给出法向量
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				// 如果对象具有纹理
				if(pObject->bHasTexture) {
					// 确定是否有UVW纹理坐标
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
		glEnd();// 绘制结束
	}
}


void Display()
{	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
	gluLookAt(0., 0. , 0,  0.,0,-50, 0,1,0);
	glTranslatef(0,0,-50);
	glRotatef(angle,0,1,0);

	glScalef(fscale,fscale,fscale); // 设置模型大小比例
	Draw3DModel(); // 画3D模型

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
	glutCreateWindow("读取3DS格式模型");

	glClearColor(0,0,0,1);
	Load3DSModel();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutTimerFunc(10,TimerFun,1);

	glutMainLoop();
}
