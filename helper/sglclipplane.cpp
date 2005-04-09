/***************************************************************************
                          sglclipplane.cpp  -  description
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

#include "sglclipplane.h"

SGLClipPlane::SGLClipPlane(short int PlaneID,GLdouble A,GLdouble B,GLdouble C,GLdouble D):SGLObj()
{
	if(PlaneID>=0)ClippingPlaneID=GLenum(PlaneID);
	else
	{
		if(!glIsEnabled(GL_CLIP_PLANE0))ClippingPlaneID=GL_CLIP_PLANE0;
		else if(!glIsEnabled(GL_CLIP_PLANE1))ClippingPlaneID=GL_CLIP_PLANE1;
		else if(!glIsEnabled(GL_CLIP_PLANE2))ClippingPlaneID=GL_CLIP_PLANE2;
		else if(!glIsEnabled(GL_CLIP_PLANE3))ClippingPlaneID=GL_CLIP_PLANE3;
		else if(!glIsEnabled(GL_CLIP_PLANE4))ClippingPlaneID=GL_CLIP_PLANE4;
		else ClippingPlaneID=GL_CLIP_PLANE0;
	}

	PlaneParam[0]=A;PlaneParam[1]=B;PlaneParam[2]=C;PlaneParam[3]=D;
}
SGLClipPlane::~SGLClipPlane()
{
	Activate(false);
}

void SGLClipPlane::generate()
{
	glClipPlane(ClippingPlaneID,PlaneParam);
}

GLboolean SGLClipPlane::IsActive()
{
	return glIsEnabled(ClippingPlaneID);
}

GLboolean SGLClipPlane::Activate(bool Activate)
{
	GLboolean ret = IsActive();
	if(Activate!=ret)glEnable(ClippingPlaneID);
	return ret;
}

/*!
    \fn SGLClipPlane::getCenter()
 */
SGLVektor SGLClipPlane::getCenter()const
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}

