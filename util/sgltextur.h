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

#include "sglimagefile.h"


/**
  *@author Enrico Reimer
  */
class SGLTextur
{
public:
	SGLTextur(const char *imageFile=0);
	~SGLTextur();
	GLuint	ID;
	GLenum TexType;
	bool valid,MipMap;
	bool Load2DImage(const char *imageFile, bool MipMap=true);
	bool Load2DImage(char *imageFile, bool MipMap=true);
	bool Load3DImage(char *imageFile, bool MipMap=true);
	bool loadTex();
	bool unloadTex();
	static short TexLoaded;
	virtual void SetParams();
	void DisableTex();
};

#endif
