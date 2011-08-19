/***************************************************************************
                          sglimagefile.h  -  description
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

#ifndef SGLIMAGEFILE_H
#define SGLIMAGEFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else 
	#include <GL/gl.h>
#endif

/**
  *@author Enrico Reimer
  */
class SGLImageFile
{
public: 
	SGLImageFile(const char *imageFile);
	SGLImageFile(unsigned int xSize,unsigned int ySize);
	~SGLImageFile();
	bool LoadRGBMipmaps( const char *imageFile, GLint intFormat );
	bool LoadRGBMipmaps2( const char *imageFile, GLenum target, GLint intFormat, GLint *width, GLint *height );
	GLint sizeX, sizeY;
	GLint components;
	unsigned char *data;
	
	struct rawImageRec
	{
		unsigned short imagic;
	    unsigned short type;
    	unsigned short dim;
	    unsigned short sizeX, sizeY, sizeZ;
    	unsigned long min, max;
	    unsigned long wasteBytes;
	    char name[80];
	    unsigned long colorMap;
	    FILE *file;
	    unsigned char *tmp, *tmpR, *tmpG, *tmpB, *tmpA;
    	unsigned long rleEnd;
	    GLuint *rowStart;
    	GLint *rowSize;
	};
	
private:
	void ConvertShort(unsigned short *array, long length);
	void ConvertLong(GLuint *array, long length);
	rawImageRec *RawImageOpen(const char *fileName);
	void RawImageClose(rawImageRec *raw);
	void RawImageGetRow(rawImageRec *raw, unsigned char *buf, int y, int z);
	void RawImageGetData(rawImageRec *raw);
	bool RGBImageLoad(const char *fileName);
//	void FreeImage(RGBImageRec *image )
};

#endif
