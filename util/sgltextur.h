/***************************************************************************
                          sgltextur.h  -  description
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

#ifndef SGLTEXTUR_H
#define SGLTEXTUR_H

//#include "sglimagefile.h"
#include "../sglmatrixobj.h"


/**
  *@author Enrico Reimer
  */
class SGLBaseTex:public SGLMatrixObj
{
public:
	SGLBaseTex();
	virtual ~SGLBaseTex();
	GLuint	ID;
	GLenum TexType;
	bool loadTex();
	bool unloadTex();
	virtual void SetParams();
	bool genValidSize(GLint internalFormat,GLsizei &width,GLsizei &height,GLsizei &depth, GLenum format,GLenum type,bool border);
	
	static short TexLoaded;
	static short def2dim(GLenum def);
    GLint getTexInfo(GLenum pname);
    GLint getTexElemBitSize();
    GLint getTexByteSize();
	bool weich,repeat;
	bool valid,MipMap,loaded;
};

class SGLTextur:public SGLBaseTex
{
public:
	SGLTextur(const char *imageFile=0);
	bool Load2DImage(const char *imageFile, bool MipMap=true);
	bool Load2DImage(char *imageFile, bool MipMap=true);
};

#endif
