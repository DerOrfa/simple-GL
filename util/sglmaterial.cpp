/***************************************************************************
                          sglmaterial.cpp  -  description
                             -------------------
    begin                : Sun Oct 14 2001
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

#include "sglmaterial.h"
#include "../sglmisc.h"

SGLMaterial::SGLMaterial()
{
	resetMat(Innen);
	resetMat(Aussen);
	Transparenz=0;
	UmgebGleichDifus=InnenGleichAussen=true;
	tex=0;isMyTex=false;
}
SGLMaterial::SGLMaterial(GLfloat R,GLfloat G, GLfloat B,GLenum Face,bool selbstLeuchtend)
{
	resetMat(Innen);
	resetMat(Aussen);
	SetColor(R,G,B,Face,selbstLeuchtend);
	Transparenz=0;
	UmgebGleichDifus=InnenGleichAussen=true;
	tex=0;isMyTex=false;
}

SGLMaterial::SGLMaterial(const char *imageFile)
{
	resetMat(Innen);
	resetMat(Aussen);
	SetColor(0,0,0);
	Transparenz=0;
	UmgebGleichDifus=InnenGleichAussen=true;
	tex=0;isMyTex=false;
	SetTex(imageFile);
}
SGLMaterial::~SGLMaterial()
{
	if(isMyTex)delete tex;
}

void SGLMaterial::loadMat()
{
	if(InnenGleichAussen)setMatParams(GL_FRONT_AND_BACK,Aussen);
	else
	{
		setMatParams(GL_FRONT,Aussen);
		setMatParams(GL_BACK,Innen);
	}
	if(tex)tex->loadTex();
	SGLMaterial::MatLoaded=true;
}

void SGLMaterial::unloadMat()
{
	SGLMaterial::MatLoaded=false;
	if(tex)tex->unloadTex();
}

void SGLMaterial::SetMat(GLfloat col[3],GLfloat Transp,GLenum Face,GLenum ColorID)
{
	const GLfloat tCol[4]={col[0],col[1],col[2],1-Transp};
	glMaterialfv(Face,ColorID,tCol);
}

void SGLMaterial::SetColor(GLfloat R,GLfloat G, GLfloat B,GLenum Face,bool selbstLeuchtend)
{
	switch(Face)
	{
	case GL_FRONT:
		{
			if(selbstLeuchtend)
			{
				Aussen.Farbe.Glow[0]=R;
				Aussen.Farbe.Glow[1]=G;
				Aussen.Farbe.Glow[2]=B;
			}
			else
			{
				Aussen.Farbe.Difus[0]=.8*R;
				Aussen.Farbe.Difus[1]=.8*G;
				Aussen.Farbe.Difus[2]=.8*B;
				Aussen.Farbe.Umgebung[0]=.2*R;
				Aussen.Farbe.Umgebung[1]=.2*G;
				Aussen.Farbe.Umgebung[2]=.2*B;
			}
			InnenGleichAussen=false;
		}break;
	case GL_BACK:
		{
			if(selbstLeuchtend)
			{
				Innen.Farbe.Glow[0]=R;
				Innen.Farbe.Glow[1]=G;
				Innen.Farbe.Glow[2]=B;
			}
			else
			{
				Innen.Farbe.Difus[0]=.8*R;
				Innen.Farbe.Difus[1]=.8*G;
				Innen.Farbe.Difus[2]=.8*B;
				Innen.Farbe.Umgebung[0]=.2*R;
				Innen.Farbe.Umgebung[1]=.2*G;
				Innen.Farbe.Umgebung[2]=.2*B;
			}
			InnenGleichAussen=false;
		}break;
	case GL_FRONT_AND_BACK:
		{
			SetColor(R,G,B,GL_FRONT,selbstLeuchtend);
			SetColor(R,G,B,GL_BACK,selbstLeuchtend);
			InnenGleichAussen=true;
		}break;
	}

}

bool SGLMaterial::SetTex(SGLTextur *TexPtr)
{
	if(TexPtr && TexPtr->valid)
	{
		isMyTex=false;
		tex=TexPtr;
		return true;
	}
	else
	{
		SGLprintWarning("ignoriere ungültige Textur");
		return false;
	}
}


bool SGLMaterial::SetTex(const char *imageFile)
//Ungünstig, da das möglicherweise dazugehöreige Objekt nicht wissen kann, daß es jetzt eine Textur hat, und deswegen Texturkoordinaten braucht.
{
	if(SetTex(new SGLTextur(imageFile)))
	{
		isMyTex=true;
		return true;
	}else return false;
}

void SGLMaterial::MergeColor(int r, int g, int b, bool doGlanz)
{
	Innen.Farbe.Glow[0]*=r/255;
	Innen.Farbe.Glow[1]*=g/255;
	Innen.Farbe.Glow[2]*=b/255;

	Innen.Farbe.Difus[0]*=r/255;
	Innen.Farbe.Difus[1]*=g/255;
	Innen.Farbe.Difus[2]*=b/255;

	Innen.Farbe.Umgebung[0]*=r/255;
	Innen.Farbe.Umgebung[1]*=g/255;
	Innen.Farbe.Umgebung[2]*=b/255;

	Aussen.Farbe.Glow[0]*=r/255;
	Aussen.Farbe.Glow[1]*=g/255;
	Aussen.Farbe.Glow[2]*=b/255;

	Aussen.Farbe.Difus[0]*=r/255;
	Aussen.Farbe.Difus[1]*=g/255;
	Aussen.Farbe.Difus[2]*=b/255;

	Aussen.Farbe.Umgebung[0]*=r/255;
	Aussen.Farbe.Umgebung[1]*=g/255;
	Aussen.Farbe.Umgebung[2]*=b/255;

	if(doGlanz)
	{
		Aussen.Farbe.Glanz[0]*=r/255;
		Aussen.Farbe.Glanz[1]*=g/255;
		Aussen.Farbe.Glanz[2]*=b/255;

		Innen.Farbe.Glanz[0]*=r/255;
		Innen.Farbe.Glanz[1]*=g/255;
		Innen.Farbe.Glanz[2]*=b/255;
	}
}


/*!
    \fn SGLMaterial::resetMat(faerbung &f)
 */
void SGLMaterial::resetMat(faerbung &f)
{
	SetColor(1,1,1);
	f.GlanzFaktor=50;
	f.Farbe.Glanz[0]=f.Farbe.Glanz[1]=f.Farbe.Glanz[2]=1;
	f.Farbe.Glow[0]=f.Farbe.Glow[1]=f.Farbe.Glow[2]=0;
}


/*!
    \fn SGLMaterial::setMatParams(GLenum FACE,faerbung &f)
 */
void SGLMaterial::setMatParams(GLenum FACE,faerbung &f)
{
	if(UmgebGleichDifus)SetMat(f.Farbe.Difus,Transparenz,FACE,GL_AMBIENT_AND_DIFFUSE);
	else
	{
		SetMat(f.Farbe.Umgebung,Transparenz,FACE,GL_AMBIENT);
		SetMat(f.Farbe.Difus,Transparenz,FACE,GL_DIFFUSE);
	}
	SetMat(f.Farbe.Glanz,0,FACE,GL_SPECULAR);
	SetMat(f.Farbe.Glow,Transparenz,FACE,GL_EMISSION);
	glMaterialf(FACE,GL_SHININESS,f.GlanzFaktor);
}

bool SGLMaterial::MatLoaded=false;