/***************************************************************************
                          sglclipplane.h  -  description
                             -------------------
    begin                : Mon Oct 1 2001
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

#ifndef SGLCLIPPLANE_H
#define SGLCLIPPLANE_H

#include "../sglobj.h"

/**
  *@author Enrico Reimer
  */
class SGLClipPlane : public SGLObj
{
public: 
	SGLClipPlane(short int PlaneID=-1,GLdouble A=0,GLdouble B=0,GLdouble C=0,GLdouble D=0);
	void generate();
	~SGLClipPlane();

	GLboolean Activate(bool Activate=true);
	GLboolean IsActive();
	GLenum ClippingPlaneID;
	GLdouble PlaneParam[4];

	SGLVektor getCenter()const;
};

#endif
