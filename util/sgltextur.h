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

#include "memconsumer.h"

#include "../sglmatrixobj.h"

#define SGL_MTEX_MODE_TINT NON_GL_ENUM_BLOCK_BEGIN+1
#define SGL_MTEX_MODE_OVERLAY NON_GL_ENUM_BLOCK_BEGIN+2
#define SGL_MTEX_MODE_COLORMASK NON_GL_ENUM_BLOCK_BEGIN+3

/**
  *@author Enrico Reimer
  */
class SGLBaseTex:public SGLMatrixObj, public MemConsumer
{
public:
	SGLBaseTex();
	virtual ~SGLBaseTex();
	boost::shared_ptr<SGLBaseTex> multitex;
	GLuint	ID;
	GLenum TexType;
	bool loadTex();
	bool unloadTex();
	virtual void SetParams();
	bool genValidSize(GLint internalFormat,GLsizei size[],unsigned short sizeCnt, GLenum format,GLenum type,bool border);
	
	static short TexLoaded;
	static short def2dim(GLenum def);
	static short multitex_layer;
	GLint getTexInfo(GLenum pname);
	GLint getTexElemBitSize();
	GLint getTexByteSize();
	GLint renderMode;
	void freeTexture();
	static bool checkForMultiText(unsigned short cnt);
	bool weich,repeat,shouldBeLoaded;
	bool valid,MipMap,loaded;
	GLfloat envColor[4];
};

class SGLTextur:public SGLBaseTex
{
public:
	SGLTextur(const char *imageFile=0);
	bool Load2DImage(const char *imageFile, bool MipMap=true);
	bool Load2DImage(char *imageFile, bool MipMap=true);
};

#endif
