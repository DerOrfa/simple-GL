/***************************************************************************
                          sglobj.cpp  -  description
                             -------------------
    begin                : Sat Sep 15 2001
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

#include "sglobj.h"
#include "sglmisc.h"
#include <stdio.h>
#include "sglmetaobj.h"

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else 
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

SGLObj::SGLObj(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLObjBase(PosX,PosY,PosZ,SizeFact)
{
	priority=std;
}

GLuint SGLObj::Compile(bool draw,bool free)
{
	is_free=free;
	GLuint error=0,i;
	bool EnableClip[5];

	SGLMetaObj *meta=dynamic_cast<SGLMetaObj*>(this);
	if(meta)meta->compileSubObjects();
	while((error=glGetError()))
	{
		SGLprintError("%s [GLerror] beim Zeichnen von %s",gluErrorString(GLenum(error)),guesType());
	}
	beginList(draw);
	{
		if(IgnoreLight)
			glDisable(GL_LIGHTING);
		for(i=0;i<5;i++)
			if((EnableClip[i]=(IgnoreClip && glIsEnabled(GLenum(GL_CLIP_PLANE0+i)))))
				glDisable(GLenum(GL_CLIP_PLANE0+i));
	metaGenerate();


	if(IgnoreLight)
		glEnable(GL_LIGHTING);
	for(i=0;i<5;i++)if(EnableClip[i])
		glEnable(GLenum(GL_CLIP_PLANE0+i));

	}
	endList();
	
	while((error=glGetError()))
	{
		SGLprintError("[GLerror] %s beim Zeichnen von %s",gluErrorString(GLenum(error)),guesType());
	}
	return ID;
}


/**
 * Standardkonstruktor.
 * Erzeugt eine Strecke, die von Vekt1 nach Vekt2 geht.
 * @param Vekt1 Anfangspunkt der Strecke
 * @param Vekt2 Endpunkt der Strecke
 */
SGLStrecke::SGLStrecke(SGLshPtr<SGLVektor> Vekt1,SGLshPtr<SGLVektor> Vekt2)
{
	punkt1=Vekt1;
	punkt2=Vekt2;
	IgnoreLight=true;
}

SGLStrecke::SGLStrecke()
{
	punkt1=SGLshPtr<SGLVektor>(new SGLVektor(0,0,0));
	punkt2=SGLshPtr<SGLVektor>(new SGLVektor(0,0,0));
	IgnoreLight=true;
}

SGLStrecke::SGLStrecke(SGLVektor Vekt1,SGLVektor Vekt2)
{
	punkt1=SGLshPtr<SGLVektor> (new SGLVektor(Vekt1));
	punkt2=SGLshPtr<SGLVektor> (new SGLVektor(Vekt2));
	IgnoreLight=true;
}

/**
 * Die Länge der Strecke.
 * @return der Abstand zwischen den beiden Endpunkten der Strecke
 */
GLdouble SGLStrecke::Len(){return boost::numeric::ublas::norm_2(*punkt1 - *punkt2);}

void SGLStrecke::generate()
{
	glBegin(GL_LINES);
		punkt1->DrawVertex();
		punkt2->DrawVertex();
	glEnd();
}

SGLVektor SGLStrecke::getCenter()const
{
	return dvector((*punkt1 + *punkt2)/2);
}
