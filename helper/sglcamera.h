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

struct ViewTrans
{
	GLdouble model[16],proj[16];
	GLint view[4];
	bool outDated;
	GLdouble depth_default;
	void update(bool force=false);
	void update(const SGLVektor &defaultDepth,bool force=false);
	bool welt2window(const SGLVektor &src,SGLVektor &dst);
	GLdouble getDepth(const SGLVektor &depthVekt);
	bool screen2welt(const unsigned int x,const unsigned int y,SGLVektor &dst);
	bool screen2welt(const unsigned int x,const unsigned int y,const SGLVektor &depthVekt,SGLVektor &dst);
	bool screen2welt(const unsigned int x,const unsigned int y,GLdouble depth,SGLVektor &dst);
	bool screen2welt(pair<unsigned int,unsigned int> screen[],const SGLVektor &depthVekt,SGLVektor welt[],unsigned int Vcnt);//irgendwann mal weghauen
	bool screen2welt(pair<unsigned int,unsigned int> screen[],const SGLVektor &depthVekt,VektorPtr welt[],unsigned int Vcnt);
};

class SGLBaseCam: public SGLHelper
{
public:
	enum ViewModi{resizeView,moveCam,scaleView}recalcEckenMode;
	SGLBaseCam(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=10);
	void RotateAim(GLdouble Xdeg, GLdouble Ydeg);
	void RotateCam(GLdouble Xdeg, GLdouble Ydeg);

	void Roll(GLdouble degree);

	void MoveZoom(GLdouble fact);
	void OptZoom(GLdouble fact);
	void MoveAimTo(SGLVektor to);
	void MoveCamTo(SGLVektor to);
	void MoveAim(GLdouble xAmount, GLdouble yAmount=0, GLdouble zAmount=0);
	void MoveCam(GLdouble xAmount, GLdouble yAmount=0, GLdouble zAmount=0);
	double ViewLength();
	void ResetView();

	SGLVektor Pos,LookAt,UpVect;
	GLdouble Angle,ClipFace,ClipHoriz;
	double ViewFormat;

	void ReCalcUpVect(bool PosIsOnNull=false,double deg=0);
	void ResetUpVect(double rotdeg=0);

    SGLVektor getCenter();
    void recalcEcken();
	void getCross(SGLVektor Horiz[2],SGLVektor Vert[2]);
	SGLVektor getLookVektor();
	void setLookVektor(SGLVektor Vekt);
	void recalcAngle(GLdouble height);
	void recalcPos(GLdouble height);
    bool showCross,move_cam_with_aim;
	bool lockRoll,lockGierCam,lockKippCam;
	bool lockGierAim,lockKippAim;
	bool lockMoveZoom,lockOptZoom,loaded;
	void loadView();
	void unloadView();
	VektorPtr Ecken[4];

	ViewTrans ViewMatr;
	inline SGLVektor screen2welt(const unsigned int x,const unsigned int y)
	{
		SGLVektor ret;
		ViewMatr.screen2welt(x,y,ret);
		return ret;
	}
	inline SGLVektor screen2welt(const unsigned int x,const unsigned int y,const SGLVektor &depthVekt)
	{
		SGLVektor ret;
		ViewMatr.screen2welt(x,y,depthVekt,ret);
		return ret;
	}
	SGLVektor screen2welt(const unsigned int x,const unsigned int y,GLdouble depth)
	{
		SGLVektor ret;
		ViewMatr.screen2welt(x,y,depth,ret);
		return ret;
	}
    void setView(unsigned int width,unsigned int height);
};

class SGLCamera: public SGLBaseCam
{
public:
	SGLCamera(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=15);
	virtual void generate();
};

#include "sgllensflare.h"

#endif
