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

	return valid=true;
#else
	SGLprintError("Für 2D-Texturen wird DevIL benötigt");
#endif
}

	
SGLBaseTex::SGLBaseTex():SGLMatrixObj(GL_TEXTURE),update(this)
{
	ID=0;
	ResetTransformMatrix();
	shouldBeLoaded=loaded=false;
	renderMode=GL_REPLACE;//Per Default ersetzt die Textur alles was da war
	envColor[0]=envColor[1]=envColor[2]=envColor[3]=0;
}

SGLBaseTex::~SGLBaseTex()
{
	if(ID>0 && glIsTexture(ID))glDeleteTextures(1,&ID);
}

bool SGLBaseTex::loadTex()
{
	GLboolean ret;
	glActiveTextureARB(GL_TEXTURE0_ARB+multitex_layer);
	glClientActiveTextureARB(GL_TEXTURE0_ARB+multitex_layer);
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
	if(multitex)
	{
		multitex_layer++;
		multitex->loadTex();
	}
	return ret;
}

bool SGLBaseTex::unloadTex()
{
	GLboolean ret;
	if(multitex)
		multitex->unloadTex();
	glClientActiveTextureARB(GL_TEXTURE0_ARB+multitex_layer);
	glActiveTextureARB(GL_TEXTURE0_ARB+multitex_layer);
	if(multitex_layer)multitex_layer--;
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
	switch(renderMode)
	{
		case SGL_MTEX_MODE_TINT:
		{
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB);
	
			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_MODULATE);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_ARB,GL_PREVIOUS_ARB);
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_ARB,GL_TEXTURE);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_ARB,GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_ARB,GL_ONE_MINUS_SRC_COLOR);
		}break;
		case SGL_MTEX_MODE_COLORMASK:
		{
/*			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_ARB, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
 			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_ARB,GL_SRC_ALPHA);

			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_ARB, GL_CONSTANT);
 			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_ARB,GL_SRC_COLOR);*/
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_ARB, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
 			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_ARB,GL_SRC_ALPHA);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_INTERPOLATE_ARB);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_ARB,GL_CONSTANT);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_ARB,GL_SRC_COLOR);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_ARB,GL_PREVIOUS_ARB);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_ARB,GL_SRC_COLOR);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE2_RGB_ARB, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND2_RGB_ARB,GL_SRC_ALPHA);
		}break;
		case SGL_MTEX_MODE_OVERLAY:
		{
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE_ARB);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_ALPHA_ARB, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
 			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_ALPHA_ARB,GL_SRC_ALPHA);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,GL_INTERPOLATE_ARB);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE0_RGB_ARB,GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND0_RGB_ARB,GL_SRC_COLOR);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE1_RGB_ARB,GL_PREVIOUS);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND1_RGB_ARB,GL_SRC_COLOR);
			
			glTexEnvf(GL_TEXTURE_ENV,GL_SOURCE2_RGB_ARB, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV,GL_OPERAND2_RGB_ARB,GL_SRC_ALPHA);
		}break;
		default:
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,renderMode);
	}

	glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,envColor);//@todo erstmal Material überschreiben - später wäre bedingtes GL_MODULATE vielleicht besser
	glTexParameterf(TexType, GL_TEXTURE_WRAP_S, repeat?GL_REPEAT:GL_CLAMP);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_T, repeat?GL_REPEAT:GL_CLAMP);
	glTexParameterf(TexType, GL_TEXTURE_WRAP_R, repeat?GL_REPEAT:GL_CLAMP);

	if(MipMap)	glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	else		glTexParameterf(TexType, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameterf(TexType, GL_TEXTURE_MAG_FILTER,weich ? GL_LINEAR:GL_NEAREST);
}

/*!
	Generiert gültige Werte für Tiefe Breite und Höhe einer Textur.
	
	Wenn die angegebenen Werte keine Potenzen von 2 sind, werden sie durch die nächstgrößte Zweierpotenz ersetzt.
	Ist ein Wert zu groß wird er halbiert.

    \fn SGLTextur::getMaxSize(GLint internalFormat,GLsizei width,GLsizei height,GLsizei depth, GLenum format,GLenum type)
 */

bool SGLBaseTex::genValidSize(GLint internalFormat,GLsizei size[],unsigned short sizeCnt, GLenum format,GLenum type,bool border)
{
	#define FORALL_SIZE(IT)	for(unsigned short IT=0;IT<sizeCnt;IT++)

	if(!glIsTexture(ID)){SGLprintError("OpenGL kennt die Textur \"%d\" nicht",ID);}
	GLsizei *newSize=new GLsizei[sizeCnt];
	GLsizei tmpSize=4;
	FORALL_SIZE(i)newSize[i]=4;
	GLenum proxyType;
	bool sizeEnought=false;
	unsigned short grown=0;
	for(;;)
	{
		switch(sizeCnt)
		{
		case 1:
			proxyType=GL_PROXY_TEXTURE_1D;
			glTexImage1D(proxyType,0,internalFormat,newSize[0]+(border ? 2:0),(border ? 1:0),format,type,NULL);break;
		case 2:
			proxyType=GL_PROXY_TEXTURE_2D;
			glTexImage2D(proxyType,0,internalFormat,newSize[0]+(border ? 2:0),newSize[1]+(border ? 2:0),(border ? 1:0),format,type,NULL);break;
		case 3:
			proxyType=GL_PROXY_TEXTURE_3D;
			glTexImage3DEXT(proxyType,0,internalFormat,newSize[0]+(border ? 2:0),newSize[1]+(border ? 2:0),newSize[2]+(border ? 2:0),(border ? 1:0),format,type,NULL);break;
		default:
			SGLprintError("Ungültiges Texturformat (%dD) beim Prüfen der Texturdaten",sizeCnt);return false;break;
		}
		glGetTexLevelParameteriv(proxyType, 0, GL_TEXTURE_WIDTH,  &tmpSize);
		bool sizeOK=true;
		if(tmpSize <= 0)
		{
			sizeOK=false;
			newSize[grown]>>=1;
		}
		if(sizeEnought)break;
		else for(;grown<sizeCnt;grown++)
		{
			if(newSize[grown]<size[grown]){newSize[grown]<<=1;break;}
			else if(grown==sizeCnt-1)sizeEnought=true;
		}
		
		if (!sizeOK) break;
	}
	
	GLuint err = glGetError();
	if(err)
	{
		SGLprintError("%s beim Prüfen der Texturdaten [GLerror]",gluErrorString(err));
		for(int i=0;i<sizeCnt;i++)newSize[i]=0;
	}
	else if(!sizeEnought)
	{
		unsigned int StrLen=sizeCnt-1;
		FORALL_SIZE(i)StrLen+=strlen(newSize[i]);
		char *formatStr=new char[StrLen+1];
		formatStr[0]=0;
		FORALL_SIZE(i)
			sprintf(formatStr+strlen(formatStr),"%d%s",newSize[i],i<sizeCnt-1 ? "x":"");
		SGLprintError("Der Texturspeicher der Grafikkarte ist zu klein. Sie lässt höchstens eine %s-Textur zu",formatStr);
		err=true;
		delete formatStr;
	}

	FORALL_SIZE(i)size[i]=newSize[i];
	delete[] newSize;
	return !err;
	#undef FORALL_SIZE
}

short SGLBaseTex::TexLoaded=0;
short SGLBaseTex::multitex_layer=0;

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
	//ATI macht hier mist wenn die Extension fehlt - also erst fragen dann schießen
	if(gluCheckExtension((const GLubyte*)"GL_EXT_paletted_texture",glGetString(GL_EXTENSIONS)))
		glGetTexLevelParameteriv(TexType,0,GL_TEXTURE_INDEX_SIZE_EXT,&index);
	else index=0;
	if(unload)unloadTex();
	return r+b+b+alpha+lum+intens+index;
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


/*!
    \fn SGLBaseTex::checkForMultiText(unsigned short cnt)
 */
bool SGLBaseTex::checkForMultiText(unsigned short cnt)
{
	if(gluCheckExtension((const GLubyte*)"GL_ARB_multitexture",glGetString(GL_EXTENSIONS)))
	{
		return (sglGeti(GL_MAX_TEXTURE_UNITS_ARB)>=cnt);
	}
	else return false;
	//@todo kann durch sglChkExt ersetzt werden
}

SGLBaseTex::updateSlot::updateSlot(SGLBaseTex *obj){this->mytex =obj;}
void SGLBaseTex::updateSlot::operator()() const{mytex->changed();}
