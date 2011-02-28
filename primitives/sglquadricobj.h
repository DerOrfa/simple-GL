/***************************************************************************
                          sglsphere.h  -  description
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

#ifndef SGLSPHERE_H
#define SGLSPHERE_H

#include "sglflobj.h"
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else 
	#include <GL/glu.h>
#endif

/**
  *@author Enrico Reimer
  */
class SGLQuadricObj : public SGLFlObj
{
public:
	SGLQuadricObj(MaterialPtr Material=MaterialPtr (),GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGLQuadricObj();
	GLUquadricObj *quadric;
	SGLVektor getCenter()const;
};

/**
  *@author Enrico Reimer
  */
class SGLSphere : public SGLQuadricObj
{
public:
	SGLSphere(MaterialPtr Material=MaterialPtr(),GLint slices=0,GLint stacks=0,GLdouble Size=1);
	GLdouble size;
	GLint slices,stacks;
	void generate();
};

class SGLZylinder : public SGLQuadricObj
{
public:
	SGLZylinder(MaterialPtr Material=MaterialPtr(),GLint slices=0,GLdouble height=1,GLdouble Size_top=1,GLdouble Size_bottom=1);
	GLdouble size_top,size_bottom;
	GLint slices,stacks;
	void generate();
	GLdouble height;
};

#endif
