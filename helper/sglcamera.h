/***************************************************************************
                          eglcamera.h  -  description
                             -------------------
    begin                : Mon Sep 17 2001
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

#ifndef SGLCAMERA_H
#define SGLCAMERA_H

#ifdef WIN32
	#include <windows.h>								// Header File For Windows
	#include <gl\gl.h>								// Header File For The OpenGL32 Library
	#include <gl\glu.h>								// Header File For The GLu32 Library
	#include <gl\glaux.h>								// Header File For The GLaux Library
#else
	#include <GL/gl.h>
#endif

#include "sglhelper.h"
#include "sgldisplist.h"

class SGLLensFlare;

/**
  *@author Enrico Reimer
  */
class SGLCamera: public SGLHelper
{
public:
	SGLCamera(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=15);
	~SGLCamera();

//  void RotateX(GLdouble amount);
//  void RotateY(GLdouble amount);

	void RotateAim(GLdouble Xdeg, GLdouble Ydeg);
	void RotateCam(GLdouble Xdeg, GLdouble Ydeg);

	void Roll(GLdouble degree);

	void MoveZoom(GLdouble fact);
	void OptZoom(GLdouble fact);
	void MoveAim(GLdouble xAmount, GLdouble yAmount=0, GLdouble zAmount=0);
	void MoveCam(GLdouble xAmount, GLdouble yAmount=0, GLdouble zAmount=0);
	double ViewLength();
	void ResetView();

	SGLVektor Pos,LookAt,UpVect;
	GLdouble Angle,ClipFace,ClipHoriz;
	double ViewFormat;

	virtual void generate();

	void ReCalcUpVect(bool PosIsOnNull=false);
	void ResetUpVect();

    SGLVektor getCenter();
};

#include "sgllensflare.h"

#endif
