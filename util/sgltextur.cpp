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
	weich=repeat=true;	
	if(imageFile)Load2DImage(imageFile);
}

bool SGLTextur::Load2DImage(const char *imageFile, bool MipMap)
{
	char *buff=new char[strlen(imageFile)+1];
	strcpy(buff,imageFile);
	bool ret= Load2DImage(buff, MipMap);
	if(!MipMap)delete buff; //ilLoadImage() mag es scheinbar nicht, wenn man seinen Pfadstring löscht / hoffentlich löscht er ihn wenigstens selber
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
	else ID = ilutGLLoadImage(imageFile);//@todo in der openil-Doku steht nix ber Fehler-Return *grml*

	if(!ID)
	{
		ILenum Error;
		while ((Error = ilGetError()) != IL_NO_ERROR)
		{
			SGLprintError("%s [GLerror] beim Laden der Textur %s", iluErrorString(Error), imageFile);
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
	TexType=GL_TEXTURE_3D_EXT;
	GLint size=256;
	glGenTextures(1, &ID);
	glBindTexture(TexType, ID);
/*	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &size);
	genValidSize(GL_LUMINANCE_ALPHA,size,size,size, GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,true);*/
	//ntzt nichts - er glaubt er bekäme die Tex rein bekommt aber unten trotzem "out of memory"

	GLubyte *pixels=(GLubyte*)malloc(size*size*size*sizeof(GLubyte)*2);
 
	for(int z=0;z<size;z++)
		for(int y=0;y<size;y++)
			for(int x=0;x<size;x++)
			{
				if(x==0 || x==size-1 || y==0 || y==size-1 || z==0 || z==size-1)
				{
					pixels[x*2+(size*y*2)+(size*size*z*2)+1]=0;
					pixels[x*2+(size*y*2)+(size*size*z*2)]=0;
				}
				else 
				{
					if(x%20 && y%20 && z%20)
						pixels[x*2+(size*y*2)+(size*size*z*2)]=255;
					else 
						pixels[x*2+(size*y*2)+(size*size*z*2)]=0;
					pixels[x*2+(size*y*2)+(size*size*z*2)+1]=255;
				}
			}

	glTexImage3DEXT(TexType,0,GL_COMPRESSED_LUMINANCE_ALPHA_ARB,size,size,size,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,pixels);

	GLuint gluerr = glGetError();
	if(gluerr)
	{
		SGLprintError("%s beim Laden der Textur [GLerror]",gluErrorString(gluerr));
		return GL_FALSE;
	}
	free(pixels);
	return valid=true;
}
	
SGLBaseTex::SGLBaseTex():SGLMatrixObj(GL_TEXTURE)
{
	ID=0;
}

SGLBaseTex::~SGLBaseTex()
{
	if(ID>0 && glIsTexture(ID))glDeleteTextures(1,&ID);
}

bool SGLBaseTex::loadTex()
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
	loadMatrix();
	return ret;
}

bool SGLBaseTex::unloadTex()
{
	GLboolean ret;
	unloadMatrix();
	if(!glIsTexture(ID)){SGLprintWarning("OpenGL kennt die Textur \"%d\" nicht",ID);}

	if(glIsEnabled(TexType))glDisable(TexType);
	else{SGLprintWarning("Hä %dD-Texturen waren gar nicht aktiv ?",def2dim(TexType));}
	if(glIsTexture(0))glBindTexture(GL_TEXTURE_2D,0);//eigentlich sollte die Textur 0 immer ex.
	SGLTextur::TexLoaded=0;
	return ret;
}

/** No descriptions */
void SGLBaseTex::SetParams()
{
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);//@todo erstmal Material berschreiben - später wäre bedingtes GL_MODULATE vielleicht besser
	glTexParameterf(TexType, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);

	if(MipMap)	glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,weich ? GL_LINEAR_MIPMAP_LINEAR:GL_NEAREST_MIPMAP_NEAREST);
	else		glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,weich ? GL_LINEAR:GL_NEAREST);
	glTexParameterf(TexType, GL_TEXTURE_MAG_FILTER,weich ? GL_LINEAR:GL_NEAREST);
}

/*!
	Generiert gltige Werte fr Tiefe Breite und Höhe einer Textur.
	
	Wenn die angegebenen Werte keine Potenzen von 2 sind, werden sie durch die nächstgrößte Zweierpotenz ersetzt.
	Ist ein Wert zu groï¿½ wird er halbiert.

    \fn SGLTextur::getMaxSize(GLint internalFormat,GLsizei width,GLsizei height,GLsizei depth, GLenum format,GLenum type)
 */
bool SGLBaseTex::genValidSize(GLint internalFormat,GLsizei &width,GLsizei &height,GLsizei &depth, GLenum format,GLenum type,bool border)
{
	if(!glIsTexture(ID)){SGLprintError("OpenGL kennt die Textur \"%d\" nicht",ID);}
	GLsizei w=2,h=2,d=2;
	for(GLint x, y, z;;) 
	{
		glTexImage3DEXT(GL_PROXY_TEXTURE_3D_EXT,0,internalFormat,w+(border ? 2:0),h+(border ? 2:0),d+(border ? 2:0),(border ? 1:0),format,type,NULL);
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D_EXT, 0, GL_TEXTURE_WIDTH,  &x);
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D_EXT, 0, GL_TEXTURE_HEIGHT, &y);
		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D_EXT, 0, GL_TEXTURE_DEPTH,  &z);
		if (x > 0 && y > 0 && z > 0 && (w<width || h<height || d<depth)) 
		{
			if(w<width)w=w<<1;
			if(h<height)h=h<<1;
			if(d<depth)d=d<<1;
		}
		else break;
	}
	width = w;
	if(&height != &width)height= h;
	if(&depth != &height && &depth != &width)depth=d;
}

short SGLBaseTex::TexLoaded=0;

short SGLBaseTex::def2dim(GLenum def)
{
	switch(def)
	{
	case GL_TEXTURE_1D:return 1;
	case GL_TEXTURE_2D:return 2;
	case GL_TEXTURE_3D:return 3;
	default:{SGLprintWarning("Die Angegebene Dimension %d ist unbekannt",def);}
	}
}
