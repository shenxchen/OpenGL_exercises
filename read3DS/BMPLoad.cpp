#define _CRT_SECURE_NO_WARNINGS
#include "BMPLoad.h"

void showBmpHead(ZXCBITMAPFILEHEADER* pBmpHead)
{
	printf("位图文件头:\n");
	printf("文件大小:%d\n",pBmpHead->bfSize);
	printf("保留字:%d\n",pBmpHead->bfReserved1);
	printf("保留字:%d\n",pBmpHead->bfReserved2);
	printf("实际位图数据的偏移字节数:%d\n",pBmpHead->bfOffBits);

}


void showBmpInforHead(ZXCtagBITMAPINFOHEADER* pBmpInforHead)
{
	printf("位图信息头:\n");
	printf("结构体的长度:%d\n",pBmpInforHead->biSize);
	printf("位图宽:%d\n",pBmpInforHead->biWidth);
	printf("位图高:%d\n",pBmpInforHead->biHeight);
	printf("biPlanes平面数:%d\n",pBmpInforHead->biPlanes);
	printf("biBitCount采用颜色位数:%d\n",pBmpInforHead->biBitCount);
	printf("压缩方式:%d\n",pBmpInforHead->biCompression);
	printf("biSizeImage实际位图数据占用的字节数:%d\n",pBmpInforHead->biSizeImage);
	printf("X方向分辨率:%d\n",pBmpInforHead->biXPelsPerMeter);
	printf("Y方向分辨率:%d\n",pBmpInforHead->biYPelsPerMeter);
	printf("使用的颜色数:%d\n",pBmpInforHead->biClrUsed);
	printf("重要颜色数:%d\n",pBmpInforHead->biClrImportant);
}

void showRgbQuan(ZXCtagRGBQUAD* pRGB)
{
	printf("(%-3d,%-3d,%-3d)   ",pRGB->rgbRed,pRGB->rgbGreen,pRGB->rgbBlue);
}

BMPPic *ZXCLoadBMP(const char *szFileName)
{
	static BMPPic pic={0,0,NULL};
	ZXCBITMAPFILEHEADER   bitHead;
	ZXCBITMAPINFOHEADER bitInfoHead;
	FILE* pfile;

	//char strFile[50];
	//printf("please input the .bmp file name:\n");
	//scanf("%s",strFile);

	pfile = fopen(szFileName,"rb");//打开文件

	if(pfile!=NULL)
	{
	   // printf("file bkwood.bmp open success.\n");
	   //读取位图文件头信息
	   WORD fileType;
	   fread(&fileType,1,sizeof(WORD),pfile);
	   if(fileType != 0x4d42)
	   {
		//printf("file is not .bmp file!");
		return NULL;
	   }
	   //fseek(pfile,2,SEEK_CUR);   // "BM"
	   fread(&bitHead,sizeof(ZXCtagBITMAPFILEHEADER),1,pfile);

	   //showBmpHead(&bitHead);
	   printf("\n\n");

	   //读取位图信息头信息
	   fread(&bitInfoHead,sizeof(ZXCBITMAPINFOHEADER),1,pfile);
	   //showBmpInforHead(&bitInfoHead);
	   printf("\n");
	}
	else
	{
	   printf("file open fail!\n");
	   return NULL;
	}


	ZXCtagRGBQUAD *pRgb ;

	if(bitInfoHead.biBitCount < 24)//有调色板 *******************************************************
	{
	   //读取调色盘结信息
	   long nPlantNum = long(pow(2,double(bitInfoHead.biBitCount)));    //   Mix color Plant Number;
	   pRgb=(ZXCtagRGBQUAD *)malloc(nPlantNum*sizeof(ZXCtagRGBQUAD));
	   memset(pRgb,0,nPlantNum*sizeof(ZXCtagRGBQUAD));
	   int num = fread(pRgb,4,nPlantNum,pfile);

	   printf("Color Plate Number: %d\n",nPlantNum);

	   printf("颜色板信息:\n");
	   for (int i =0; i<nPlantNum;i++)
	   {
		if (i%5==0)
		{
		 printf("\n");
		}
		showRgbQuan(&pRgb[i]);
	   }
	   printf("\n");
	}

	// 24位、32位位图*********************************************************************************
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;
	pic.iwidth = width;
	pic.iheight = height;

	//分配内存空间把源图存入内存
//	int l_width   = WIDTHBYTES(width* bitInfoHead.biBitCount);//计算位图的实际宽度并确保它为32的倍数,看BMPLoad.h中的#define，实现4字节对齐!!!!!!!
	int l_width   = WIDTHBYTES(width)*height;//计算位图的实际宽度并确保它为32的倍数,看BMPLoad.h中的#define，实现4字节对齐!!!!!!!
	pic.pByte = (BYTE *)malloc(height*l_width);
	memset(pic.pByte,0,height*l_width);
	long nData = height*l_width;

	//把位图数据信息读到数组里
	fread(pic.pByte,1,nData,pfile);

	return &pic;
/*
	//将位图数据转化为RGB数据
	tagRGBQUAD* dataOfBmp;
	dataOfBmp = (tagRGBQUAD *)malloc(width*height*sizeof(tagRGBQUAD));//用于保存各像素对应的RGB数据
	memset(dataOfBmp,0,width*height*sizeof(tagRGBQUAD));

	if(bitInfoHead.biBitCount<24)//有调色板，即位图为非真彩色 *****************************
	{
	   int k;
	   int index = 0;
	   if (bitInfoHead.biBitCount == 1)
	   {
		for(int i=0;i<height;i++)
		 for(int j=0;j<width;j++)
		 {
		  BYTE mixIndex= 0;
		  k = i*l_width + j/8;//k:取得该像素颜色数据在实际数据数组中的序号
		  //j:提取当前像素的颜色的具体值
		  mixIndex = pColorData[k];
		  switch(j%8)
			{
			  case 0:
			   mixIndex = mixIndex<<7;
			   mixIndex = mixIndex>>7;
			   break;
			  case 1:
			   mixIndex = mixIndex<<6;
			   mixIndex = mixIndex>>7;
			   break;
			  case 2:
			   mixIndex = mixIndex<<5;
			   mixIndex = mixIndex>>7;
			   break;

			  case 3:
			   mixIndex = mixIndex<<4;
			   mixIndex = mixIndex>>7;
			   break;
			  case 4:
			   mixIndex = mixIndex<<3;
			   mixIndex = mixIndex>>7;
			   break;

			  case 5:
			   mixIndex = mixIndex<<2;
			   mixIndex = mixIndex>>7;
			   break;
			  case 6:
			   mixIndex = mixIndex<<1;
			   mixIndex = mixIndex>>7;
			   break;

			  case 7:
			   mixIndex = mixIndex>>7;
			   break;
		  }

		  //将像素数据保存到数组中对应的位置
		  dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
		  dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
		  dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
		  dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
		  index++;

		}
	   }

	   if(bitInfoHead.biBitCount==2)
	   {
		for(int i=0;i<height;i++)
		 for(int j=0;j<width;j++)
		 {
		  BYTE mixIndex= 0;
		  k = i*l_width + j/4;//k:取得该像素颜色数据在实际数据数组中的序号
		  //j:提取当前像素的颜色的具体值
		  mixIndex = pColorData[k];
		  switch(j%4)
		  {
			  case 0:
			   mixIndex = mixIndex<<6;
			   mixIndex = mixIndex>>6;
			   break;
			  case 1:
			   mixIndex = mixIndex<<4;
			   mixIndex = mixIndex>>6;
			   break;
			  case 2:
			   mixIndex = mixIndex<<2;
			   mixIndex = mixIndex>>6;
			   break;
			  case 3:
			   mixIndex = mixIndex>>6;
			   break;
		  }

		  //将像素数据保存到数组中对应的位置
		  dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
		  dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
		  dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
		  dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
		  index++;


		 }
	   }
	   if(bitInfoHead.biBitCount == 4)
	   {
		for(int i=0;i<height;i++)
		 for(int j=0;j<width;j++)
		 {
		  BYTE mixIndex= 0;
		  k = i*l_width + j/2;
		  mixIndex = pColorData[k];
		  if(j%2==0)
		  {//低
		   mixIndex = mixIndex<<4;
		   mixIndex = mixIndex>>4;
		  }
		  else
		  {//高
		   mixIndex = mixIndex>>4;
		  }

		  dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
		  dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
		  dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
		  dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
		  index++;

		 }

	   }
	   if(bitInfoHead.biBitCount == 8)
	   {
		for(int i=0;i<height;i++)
		 for(int j=0;j<width;j++)
		 {
		  BYTE mixIndex= 0;

		  k = i*l_width + j;

		  mixIndex = pColorData[k];

		  dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
		  dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
		  dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
		  dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
		  index++;



		 }
	   }
	   if(bitInfoHead.biBitCount == 16)
	   {
		for(int i=0;i<height;i++)
		 for(int j=0;j<width;j++)
		 {
		  WORD mixIndex= 0;

		  k = i*l_width + j*2;
		  WORD shortTemp;
		  shortTemp = pColorData[k+1];
		  shortTemp = shortTemp<<8;

		  mixIndex = pColorData[k] + shortTemp;

		  dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
		  dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
		  dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
		  dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
		  index++;
		 }
	   }
	}
	else//位图为24位真彩色
	{
	   int k;
	   int index = 0;
	   for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
		 k = i*l_width + j*3;
		 dataOfBmp[index].rgbRed = pColorData[k+2];
		 dataOfBmp[index].rgbGreen = pColorData[k+1];
		 dataOfBmp[index].rgbBlue = pColorData[k];
		 index++;
		}
	}


	printf("像素数据信息:\n");
	for (int i=0; i<width*height; i++)
	{
	   if (i%5==0)
	   {
		printf("\n");
	   }
	   showRgbQuan(&dataOfBmp[i]);
	}

	fclose(pfile);
	if (bitInfoHead.biBitCount<24)
	{
	   free(pRgb);
	}
	free(dataOfBmp);
	free(pColorData);
	printf("\n");

	return &pic;
*/
}
