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
#include "../sglmisc.h"

#include <IL/ilut.h>

SGLTextur::SGLTextur(const char *imageFile)
{
	ID=0;
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
	if(ID>0 && glIsTexture(ID))glDeleteTextures(1,&ID);ID=0;
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
	if(glIsTexture(ID))glDeleteTextures(1,&ID);ID=0;
	TexType=GL_TEXTURE_3D;
#define	size 64 //@todo glPixelStore könnte das tunen - Multitexture wär vielleicht auch nich schlecht

	GLubyte pixels[size][size][size][2];
 
	for(int x=0;x<size;x++)
		for(int y=0;y<size;y++)
			for(int z=0;z<size;z++)
			{
				if(x==0 || x==size-1 || y==0 || y==size-1 || z==0 || z==size-1)
				{
					pixels[z][y][x][1]=50;
					pixels[z][y][x][0]=0;
				}
				else 
				{
					if((x/(size/4) + y/(size/4) + z/(size/4))%2)
					{
						pixels[z][y][x][0]=255;
					}
					else pixels[z][y][x][0]=0;
					pixels[z][y][x][1]=255;
				}
			}

	glGenTextures(1, &ID);
	glBindTexture(TexType, ID);

	glTexImage3D(TexType,0,GL_LUMINANCE_ALPHA,size,size,size,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,pixels);

	GLuint gluerr = glGetError();
	if(gluerr)
	{
		SGLprintError("%s [GLerror]",gluErrorString(gluerr));
		return GL_FALSE;
	}
	return valid=true;
#undef size
}

bool SGLTextur::loadTex()
{
	GLboolean ret;
	short dim=def2dim(TexType);
	if(glIsTexture(ID))
	{
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
			SGLprintWarning("Die %dD-Textur \"%d\" ist nicht im Grafikspeicher",dim,ID);
		}
	}
	else{SGLprintError("OpenGL kennt die %dD-Textur \"%d\" nicht",dim,ID);}
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
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);//@todo erstmal Material überschreiben - später wäre bedingtes GL_MODULATE vielleicht besser
	glTexParameterf(TexType, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);

	if(MipMap)	glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,weich ? GL_LINEAR_MIPMAP_LINEAR:GL_NEAREST_MIPMAP_NEAREST);
	else		glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,weich ? GL_LINEAR:GL_NEAREST);
	glTexParameterf(TexType, GL_TEXTURE_MAG_FILTER,weich ? GL_LINEAR:GL_NEAREST);
}

short SGLTextur::TexLoaded=0;
