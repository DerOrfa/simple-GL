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
#include <GL/glu.h>
#ifdef USE_DEVIL
	#include <IL/ilut.h>
#endif

SGLTextur::SGLTextur(const char *imageFile)
{
	weich=repeat=true;	
	if(imageFile)Load2DImage(imageFile);
	shouldBeLoaded=loaded=false;
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
#ifdef USE_DEVIL
	this->MipMap=MipMap;
	freeTexture();
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
#else
	SGLprintError("Für 2D-Texturen wird DevIL benötigt");
#endif
}

	
SGLBaseTex::SGLBaseTex():SGLMatrixObj(GL_TEXTURE)
{
	ID=0;
	ResetTransformMatrix();
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
		loaded=true;

		if(!glAreTexturesResident(1,&ID,&ret) && shouldBeLoaded)
		{
			SGLprintInfo("Die %dD-Textur \"%d\" ist nicht im Grafikspeicher",dim,ID);
		}
		shouldBeLoaded=true;
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
	loaded=false;
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
	Generiert gltige Werte für Tiefe Breite und Höhe einer Textur.
	
	Wenn die angegebenen Werte keine Potenzen von 2 sind, werden sie durch die nächstgrößte Zweierpotenz ersetzt.
	Ist ein Wert zu groß wird er halbiert.

    \fn SGLTextur::getMaxSize(GLint internalFormat,GLsizei width,GLsizei height,GLsizei depth, GLenum format,GLenum type)
 */
bool SGLBaseTex::genValidSize(GLint internalFormat,GLsizei &width,GLsizei &height,GLsizei &depth, GLenum format,GLenum type,bool border)
{
	if(!glIsTexture(ID)){SGLprintError("OpenGL kennt die Textur \"%d\" nicht",ID);}
	GLsizei w=2,h=2,d=2;
	for(GLint x, y, z;;) 
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D_EXT,0,internalFormat,w+(border ? 2:0),h+(border ? 2:0),d+(border ? 2:0),(border ? 1:0),format,type,NULL);
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
	GLuint gluerr = glGetError();
	if(gluerr)
	{
		SGLprintError("%s beim Prüfen der Texturdaten [GLerror]",gluErrorString(gluerr));
		w=h=d=0;
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

/*!
	Liefert tatsächliche Informationen über die Textur in der GL-Maschiene.
	(tatsächliche Dimensionen, tatsächliche Größe)
    \fn SGLBaseTex::GLint getTexInfo(GLenum pname)
 */
GLint SGLBaseTex::getTexInfo(GLenum pname)
{
	bool unload=false;
	GLint ret;
	if(!loaded)//Falls die Text nicht geladen ist
		loadTex();
	
	glGetTexLevelParameteriv(TexType,0,pname,&ret);
	if(unload)unloadTex();
	return ret;
}


/*!
    \fn SGLBaseTex::getTexElemBitSize()
 */
GLint SGLBaseTex::getTexElemBitSize()
{
	#define GET_CHAN_SIZE(CH,RET)	glGetTexLevelParameteriv(TexType,0,GL_TEXTURE_ ## CH ## _SIZE,&RET)
	bool unload=false;
	GLint r,g,b,alpha,lum,intens,index;
	if(!loaded)//Falls die Text nicht geladen ist
		loadTex();
	
	GET_CHAN_SIZE(RED,r);
	GET_CHAN_SIZE(GREEN,g);
	GET_CHAN_SIZE(BLUE,b);
	GET_CHAN_SIZE(ALPHA,alpha);
	GET_CHAN_SIZE(LUMINANCE,lum);
	GET_CHAN_SIZE(INTENSITY,intens);
	glGetTexLevelParameteriv(TexType,0,GL_TEXTURE_INDEX_SIZE_EXT,&index);
	if(unload)unloadTex();
	return r+b+b+alpha+lum+intens,index;
	#undef GET_CHAN_SIZE
}


/*!
    \fn SGLBaseTex::getTexByteSize
 */
GLint SGLBaseTex::getTexByteSize()
{
	bool unload=false;
	GLint w,h,d;
	double fakt=getTexElemBitSize()/8.;
	if(!loaded)//Falls die Text nicht geladen ist
		loadTex();
	
	glGetTexLevelParameteriv(TexType,0,GL_TEXTURE_WIDTH,&w);
	glGetTexLevelParameteriv(TexType,0,GL_TEXTURE_DEPTH,&d);
	glGetTexLevelParameteriv(TexType,0,GL_TEXTURE_HEIGHT,&h);
	if(unload)unloadTex();
	if(w+d+h)
	#define NZ(val)	(val ? val:1)
	{
		return NZ(w)*NZ(d)*NZ(h)*fakt;
	}
	#undef NZ
	return 0;
}


/*!
    \fn SGLBaseTex::freeTexture()
 */
void SGLBaseTex::freeTexture()
{
	if(ID<=0 || glIsTexture(ID))
	{
		SGLprintWarning("Versuch die ungültige Textur %d zu löschen",ID);
	}
	else 
	{
		glDeleteTextures(1,&ID);
		ID=0;
		shouldBeLoaded=false;
	}
}
