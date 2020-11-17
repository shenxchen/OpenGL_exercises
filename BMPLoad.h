#pragma once
//ReadBitMap
//
#include <string.h> 
#include <math.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <malloc.h>


#define   WIDTHBYTES(bits) (((bits)+31)/32*4)

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;


//λͼ�ļ�ͷ��Ϣ�ṹ����,14�ֽ�
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��

typedef struct tagBITMAPFILEHEADER {
	DWORD bfSize;           //�ļ���С
	WORD   bfReserved1; 	//�����֣�������
	WORD   bfReserved2; 	//�����֣�ͬ��
	DWORD bfOffBits;        //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER;


//��ϢͷBITMAPINFOHEADER��Ҳ��һ���ṹ���䶨�����£�

typedef struct tagBITMAPINFOHEADER {
	//public:
	DWORD   biSize;         	//ָ���˽ṹ��ĳ��ȣ�Ϊ40
	LONG    biWidth;       		//λͼ��
	LONG    biHeight;       	//λͼ��
	WORD    biPlanes;       	//ƽ������Ϊ1
	WORD    biBitCount;     	//������ɫλ����������1��2��4��8��16��24���µĿ�����32
	DWORD   biCompression;  	//ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
	DWORD   biSizeImage;    	//ʵ��λͼ����ռ�õ��ֽ���
	LONG    biXPelsPerMeter;	//X����ֱ���
	LONG    biYPelsPerMeter;	//Y����ֱ���
	DWORD   biClrUsed;      	//ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
	DWORD   biClrImportant; 	//��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
} BITMAPINFOHEADER;


//��ɫ��Palette����Ȼ�������Ƕ���Щ��Ҫ��ɫ���λͼ�ļ����Եġ�24λ��32λ�ǲ���Ҫ��ɫ��ġ�
//���ƺ��ǵ�ɫ��ṹ���������ʹ�õ���ɫ������

typedef struct tagRGBQUAD {
	//public:
	BYTE     rgbBlue; //����ɫ����ɫ����
	BYTE     rgbGreen; //����ɫ����ɫ����
	BYTE     rgbRed; //����ɫ�ĺ�ɫ����
	BYTE     rgbReserved; //����ֵ
} RGBQUAD;

typedef struct
{
	WORD    iwidth;    //λͼ��
	WORD    iheight;   //λͼ��
	BYTE    *pByte;		//ͼƬ����ָ��
}BMPPic;

void showBmpHead(BITMAPFILEHEADER* pBmpHead);
void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead);
void showRgbQuan(tagRGBQUAD* pRGB);
BMPPic *ZXCLoadBMP(const char *szFileName);

