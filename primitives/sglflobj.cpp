/***************************************************************************
                          sglflobj.cpp  -  description
                             -------------------
    begin                : Tue Oct 2 2001
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

#include "sglflobj.h"
#include <stdio.h>
#include <GL/glu.h>
#include "../sglmisc.h"

SGLFlObj::SGLFlObj(SGLMaterial *Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):SGLObj(PosX,PosY,PosZ,SizeFact)
{
	GLint MasterPolyMode[2];
	glGetIntegerv(GL_POLYGON_MODE,MasterPolyMode);
	VisMode=GLenum(MasterPolyMode[0]);
	Mat=0;
	resetMaterial(Material);
	priority=flstd;
	twoSideRender=false;
}

SGLFlObj::~SGLFlObj()
{
	if(MatIsMine)delete Mat;
}

GLuint SGLFlObj::Compile()
{
	GLboolean	doCullFaces=false,doBlend=false;
	GLint		CullFace;
	GLuint error=0;
	int i;
	bool EnableClip[5];

	while(error=glGetError())
	{
		SGLprintError("%s [GLerror] beim Zeichnen von %s",gluErrorString(GLenum(error)),guesType());
	}

	//NVIDIA optimiert Setzoperationen komplett raus aus den Listen, wenn der Wert schon gesetzt ist
	//Deshalb bringt das vorsorgliche glPolygonMode nix mehr (es kann nicht garantiert werden, da� es
	//auch in der Liste landet)
	//Behandle GL_FILL jetzt als Normfall, und alle anderen setzen ihren Modus UND nehmen ihn auch
	//wieder raus
	glNewList(ID,GL_COMPILE_AND_EXECUTE);
		for(i=0;i<5;i++)
			if(EnableClip[i]=(IgnoreClip && glIsEnabled(GLenum(GL_CLIP_PLANE0+i))))
				glDisable(GLenum(GL_CLIP_PLANE0+i));
		if(IgnoreLight)glDisable(GL_LIGHTING);

		if(VisMode!=GL_FILL)
		{
			glPolygonMode(GL_FRONT_AND_BACK,VisMode);
		}

		if(	doCullFaces=glIsEnabled(GL_CULL_FACE) &&
			(VisMode!=GL_FILL || twoSideRender)
		)
		{
			glGetIntegerv(GL_CULL_FACE_MODE,&CullFace);
			glDisable(GL_CULL_FACE);
		}

		if(Mat)
		{
			//Wenn Lightning aus ist, scheint er Material-Settings zu ignorieren
			if(IgnoreLight)glColor3fv(Mat->Aussen.Farbe.Glow);
			else Mat->loadMat();
		}

		metaGenerate();

		if(Mat)
		{
			//Wenn Lightning aus ist, scheint er Material-Settings zu ignorieren
			if(IgnoreLight)glColor3fv(Mat->Aussen.Farbe.Glow);
			else Mat->unloadMat();
		}

		if(VisMode!=GL_FILL)
		{
			glPolygonMode(GL_FRONT,GL_FILL);
			glPolygonMode(GL_BACK,GL_FILL);
		}
		if(doCullFaces)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GLenum(CullFace));
		}
		if(IgnoreLight)glEnable(GL_LIGHTING);
		for(i=0;i<5;i++)if(EnableClip[i])
			glDisable(GLenum(GL_CLIP_PLANE0+i));

	glEndList();

	while(error=glGetError())
	{
		SGLprintError("%s [GLerror] beim Zeichnen von %s",gluErrorString(GLenum(error)),guesType());
	}
	return ID;
}

void SGLFlObj::DrahtGitter(bool DO)
{
	if(DO)VisMode=GL_LINE;
	else
		VisMode=GL_FILL;
}

void SGLFlObj::resetMaterial(SGLMaterial *NewMaterial)
{
	if(Mat)
	{
		if(MatIsMine)delete Mat;
		Mat=0;
	}
	if(MatIsMine=!NewMaterial)Mat= new SGLMaterial;
	else Mat=NewMaterial;
/*	if(Mat && Mat->tex && Mat->tex->valid)
		resetTexKoord();*/
	//nicht mehr n�tig, da ALLE Texturkoordinaten haben
}

/*!
    \fn SGLFlObj::resetTexKoord()
 */
void SGLFlObj::resetTexKoord()
{
	SGLprintInfo("resetTexKoord-Dummy-used for %s",this->guesType());
}
