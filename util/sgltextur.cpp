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
#include <GL/glext.h>

SGLTextur::SGLTextur(const char *imageFile)
{
	ID=-1;
	if(imageFile)Load2DImage(imageFile);
}
SGLTextur::~SGLTextur()
{
	if(ID>0 && glIsTexture(ID))glDeleteTextures(1,&ID);
}

bool SGLTextur::Load2DImage(const char *imageFile, bool MipMap)
{
	char *buff=new char[strlen(imageFile)+1];
	strcpy(buff,imageFile);
	bool ret= Load2DImage(buff, MipMap);
	if(!MipMap)delete buff; //ilLoadImage() mag es scheinbar nicht, wenn man seinen Pfadstring löscht
	return ret;
}

bool SGLTextur::Load2DImage(char *imageFile, bool MipMap)
{
	this->MipMap=MipMap;
	if(ID>0 && glIsTexture(ID))glDeleteTextures(1,&ID);ID=-1;
	TexType=GL_TEXTURE_2D;

	if(MipMap)
	{
		ILuint ImageId;
		ilGenImages(1, &ImageId);
		ilBindImage(ImageId);
		if(IL_FALSE!=ilLoadImage(imageFile))ID=ilutGLBindMipmaps();
	}
	else ID = ilutGLLoadImage(imageFile);//@todo in der openil-Doku steht nix über Fehler-Return *grml*

	if(!ID)
	{
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR)
		{
			SGLprintError("%s [GLerror]", iluErrorString(Error));
			return GL_FALSE;
		}
	}
	SGLcheckGLError;

	return valid=true;
}

bool SGLTextur::Load3DImage(char *imageFile, bool MipMap)
{
	this->MipMap=false;
	if(glIsTexture(ID))glDeleteTextures(1,&ID);ID=-1;
	TexType=GL_TEXTURE_3D;

	GLfloat pixels[32][32][32][3];

	for(int x=0;x<32;x++)
		for(int y=0;y<32;y++)
			for(int z=0;z<32;z++)
			{
				pixels[x][y][z][0]=x/32.;
				pixels[x][y][z][1]=y/32.;
				pixels[x][y][z][2]=z/32.;
			}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_3D, ID);

	glTexImage3D(GL_TEXTURE_3D,0,3,32,32,32,0,GL_RGB,GL_FLOAT,pixels);

	GLuint gluerr = glGetError();
	if(gluerr)
	{
		SGLprintError("%s [GLerror]",gluErrorString(gluerr));
		return GL_FALSE;
	}
	return valid=true;
}

bool SGLTextur::loadTex()
{
	GLboolean ret;
	if(glIsTexture(ID))
	{
		short dim=def2dim(TexType);
		if(glIsEnabled(TexType))
		{
			SGLprintWarning("%dD-Texturen sind schon aktiviert",dim,ID);
			glDisable(TexType);
		}
		glEnable(TexType);
		glBindTexture(TexType,ID);
		SetParams();
		SGLTextur::TexLoaded=dim;

		if(!glAreTexturesResident(1,&ID,&ret))
		{
			SGLprintWarning("Die Textur \"%d\" ist nicht im Grafikspeicher",ID);
		}
	}
	else{SGLprintError("OpenGL kennt die Textur \"%d\" nicht",ID);}
	return ret;
}

bool SGLTextur::unloadTex()
{
	GLboolean ret;
	if(!glIsTexture(ID)){SGLprintWarning("OpenGL kennt die Textur \"%d\" nicht",ID);}

	if(glIsEnabled(TexType))glDisable(TexType);
	else{SGLprintWarning("Hä, %dD-Texturen waren gar nicht aktiv ?",def2dim(TexType));}
	if(glIsTexture(0))glBindTexture(GL_TEXTURE_2D,0);//eigentlich sollte die Textur 0 immer ex.
	SGLTextur::TexLoaded=0;
	return ret;
}

/** No descriptions */
void SGLTextur::SetParams()
{
	glTexParameterf(TexType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,MipMap ? GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR);
	glTexParameterf(TexType, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
}

short SGLTextur::TexLoaded=0;
