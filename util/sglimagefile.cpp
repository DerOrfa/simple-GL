/***************************************************************************
                          sglimagefile.cpp  -  description
                             -------------------
    begin                : Sun Oct 21 2001
    copyright            : (C) 2001 by Enrico Reimer
    email                : enni_@T-Online.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sglimagefile.h"
#include "filenotfound.c"
#include <errno.h>
#include "../sglmisc.h"

#ifndef WIN32
	#include <unistd.h>
	#include <sys/stat.h>
#endif

SGLImageFile::SGLImageFile(unsigned int xSize,unsigned int ySize)
{
	sizeX=xSize;
	sizeY=ySize;
	data=new unsigned char[sizeX*sizeY*4];
}

SGLImageFile::SGLImageFile(const char *imageFile)
{
// 	GLint bytes;
// 	GLubyte *buffer;
	data=0;

	if(!RGBImageLoad(imageFile))return;
	
	if (components==3);//format = GL_RGB;
	else if (components==4);//format = GL_RGBA;
	else
	{
		SGLprintError("Error in LoadRGBImage %d-component images not implemented", components );
		return;
	}
}

SGLImageFile::~SGLImageFile()
{
	if(data) delete data;
}

void SGLImageFile::ConvertShort(unsigned short *array, long length)
{
	unsigned long b1, b2;
	unsigned char *ptr;
	ptr = (unsigned char *)array;
	while (length--)
	{
		b1 = *ptr++;
		b2 = *ptr++;
		*array++ = (unsigned short) ((b1 << 8) | (b2));
	}
}

void SGLImageFile::ConvertLong(GLuint *array, long length)
{
	unsigned long b1, b2, b3, b4;
	unsigned char *ptr;
	ptr = (unsigned char *)array;
	while (length--)
	{
		b1 = *ptr++;
		b2 = *ptr++;
		b3 = *ptr++;
		b4 = *ptr++;
		*array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
   }
}

SGLImageFile::rawImageRec*
SGLImageFile::RawImageOpen(const char *fileName)
{
	int x;
	union
	{
		int testWord;
		char testByte[4];
	}endianTest;
	SGLImageFile::rawImageRec *raw;
	GLenum swapFlag;
	endianTest.testWord = 1;
	if (endianTest.testByte[0] == 1)swapFlag = GL_TRUE;
	else swapFlag = GL_FALSE;

	raw = (SGLImageFile::rawImageRec *)malloc(sizeof(SGLImageFile::rawImageRec));
	if (raw == NULL)
	{
		SGLprintError("Out of memory!");
		return NULL;
	}

	#ifndef WIN32
    struct stat buf;
	if(stat(fileName,&buf))
	{
		if(S_ISDIR(buf.st_rdev)){SGLprintError("%s ist ein Verzeichniss.",fileName);}
		else {SGLprintError("Fehler beim Laden der Bilddatei %s: %s",fileName,strerror(errno));}
		return NULL;
	}
	#endif
	if((raw->file = fopen(fileName, "rb")) == NULL )
	{
		perror(fileName);
		return NULL;
	}

	fread(raw, 1, 12, raw->file);

	if (swapFlag)ConvertShort(&raw->imagic, 6);

	raw->tmp = (unsigned char *)malloc(raw->sizeX*256);
	raw->tmpR = (unsigned char *)malloc(raw->sizeX*256);
	raw->tmpG = (unsigned char *)malloc(raw->sizeX*256);
	raw->tmpB = (unsigned char *)malloc(raw->sizeX*256);
	if (raw->sizeZ==4)raw->tmpA = (unsigned char *)malloc(raw->sizeX*256);
	if (raw->tmp == NULL || raw->tmpR == NULL || raw->tmpG == NULL || raw->tmpB == NULL)
	{
		SGLprintError("Out of memory!");
		return NULL;
	}

	if ((raw->type & 0xFF00) == 0x0100)
	{
		x = raw->sizeY * raw->sizeZ * sizeof(GLuint);
		raw->rowStart = (GLuint *)malloc(x);
    	raw->rowSize = (GLint *)malloc(x);
		if (raw->rowStart == NULL || raw->rowSize == NULL)
		{
			SGLprintError("Out of memory!");
			return NULL;
		}
		raw->rleEnd = 512 + (2 * x);
		fseek(raw->file, 512, SEEK_SET);
		fread(raw->rowStart, 1, x, raw->file);
		fread(raw->rowSize, 1, x, raw->file);
		if (swapFlag)
		{
			ConvertLong(raw->rowStart, (long) (x/sizeof(GLuint)));
			ConvertLong((GLuint *)raw->rowSize, (long) (x/sizeof(GLint)));
		}
	}
	return raw;
}

void SGLImageFile::RawImageClose(SGLImageFile::rawImageRec *raw)
{
	fclose(raw->file);
	free(raw->tmp);
	free(raw->tmpR);
	free(raw->tmpG);
	free(raw->tmpB);
	if (raw->sizeZ>3)free(raw->tmpA);
	free(raw);
}

void SGLImageFile::RawImageGetRow(SGLImageFile::rawImageRec *raw, unsigned char *buf, int y, int z)
{
	unsigned char *iPtr, *oPtr, pixel;
	int count, done = 0;
	if ((raw->type & 0xFF00) == 0x0100)
	{
		fseek(raw->file, (long) raw->rowStart[y+z*raw->sizeY], SEEK_SET);
		fread(raw->tmp, 1, (unsigned int)raw->rowSize[y+z*raw->sizeY],raw->file);
		iPtr = raw->tmp;
		oPtr = buf;
		while (!done)
		{
			pixel = *iPtr++;
			count = (int)(pixel & 0x7F);
			if (!count){done = 1;return;}
			if (pixel & 0x80)
				while (count--)*oPtr++ = *iPtr++;
			else
			{
            	pixel = *iPtr++;
	            while (count--)*oPtr++ = pixel;
			}
		}
	}
	else
	{
		fseek(raw->file, 512+(y*raw->sizeX)+(z*raw->sizeX*raw->sizeY),SEEK_SET);
		fread(buf, 1, raw->sizeX, raw->file);
	}
}


void SGLImageFile::RawImageGetData(rawImageRec *raw)
{
	unsigned char *ptr;
	int i, j;

	data = (unsigned char *)malloc((raw->sizeX+1)*(raw->sizeY+1)*4);
	if (data == NULL){SGLprintError("Out of memory!");}

	ptr = data;
	for (i = 0; i < (int)(raw->sizeY); i++)
	{
		RawImageGetRow(raw, raw->tmpR, i, 0);
		RawImageGetRow(raw, raw->tmpG, i, 1);
		RawImageGetRow(raw, raw->tmpB, i, 2);
		if (raw->sizeZ>3)RawImageGetRow(raw, raw->tmpA, i, 3);
		for (j = 0; j < (int)(raw->sizeX); j++)
		{
			*ptr++ = *(raw->tmpR + j);
			*ptr++ = *(raw->tmpG + j);
			*ptr++ = *(raw->tmpB + j);
			if (raw->sizeZ>3)*ptr++ = *(raw->tmpA + j);
		}
	}
}


bool SGLImageFile::RGBImageLoad(const char *fileName)
{
	rawImageRec *raw = RawImageOpen(fileName);
	if (raw)
	{
		sizeX = raw->sizeX;
		sizeY = raw->sizeY;
		components = raw->sizeZ;
		RawImageGetData(raw);
		RawImageClose(raw);
		return true;
	}
	else
	{
		SGLprintError("File not found");
		sizeX=FileNotFoundImage.width;
		sizeY=FileNotFoundImage.height;
		components=FileNotFoundImage.bytes_per_pixel;
		if((data = (unsigned char *)malloc(sizeX*sizeY*4+1)))
		{
			GIMP_IMAGE_RUN_LENGTH_DECODE(data,FileNotFoundImage.rle_pixel_data,sizeX*sizeY,components);
			//sizeof(FileNotFoundImage.rle_pixel_data)
			return true;
		}
		else
		{
			SGLprintError("Out of memory!");
			return false;
		}
		
	}
}


