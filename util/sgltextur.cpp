/***************************************************************************
                          sgltextur.cpp  -  description
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

#include "sgltextur.h"
#include <GL/glu.h>
#include "../sglmisc.h"
#include <IL/ilut.h>

SGLTextur::SGLTextur(const char *imageFile)
{
	if(imageFile)valid=Load2DImage(imageFile);
	else valid=false;
}
SGLTextur::~SGLTextur()
{
	if(glIsTexture(ID))glDeleteTextures(1,&ID);
}

bool SGLTextur::Load2DImage(const char *imageFile, bool MipMap)
{
	char *buff=new char[sizeof(imageFile)];
	bool ret= Load2DImage(buff, MipMap);
	delete buff;
	return ret;
}

bool SGLTextur::Load2DImage(char *imageFile, bool MipMap)
{
	if(glIsTexture(ID))glDeleteTextures(1,&ID);ID=0;

	if(MipMap)
	{
		ILuint ImageId;
		ilGenImages(1, &ImageId);
		ilBindImage(ImageId);
		if(IL_FALSE!=ilLoadImage(&imageFile[0]))ID=ilutGLBindMipmaps();
	}
	else ID = ilutGLLoadImage(imageFile);

	if(ID)SetParams();
	else
	{
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR)
		{
			SGLprintError("%s [GLerror]", iluErrorString(Error));
			return GL_FALSE;
		}
	}
	return true;
}

bool SGLTextur::DoTexCalls()
{
	GLboolean ret;
	if(glIsTexture(ID) && glAreTexturesResident(1,&ID,&ret))
	{
		glEnable(TexType);
		glBindTexture(TexType,ID);
	}
	else glDisable(TexType);
	return ret;
}
/** No descriptions */
void SGLTextur::SetParams()
{
	glBindTexture(TexType, ID);

	glTexParameterf(TexType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(TexType, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
}
/** No descriptions */
void SGLTextur::DisableTex()
{
}
