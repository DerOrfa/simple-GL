/***************************************************************************
                          sglspace.h  -  description
                             -------------------
    begin                : Fri Sep 14 2001
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

#ifndef SGLSPACE_H
#define SGLSPACE_H

#ifdef WIN32
	#include <windows.h>								// Header File For Windows
	#include <gl\gl.h>									// Header File For The OpenGL32 Library
	#include <gl\glu.h>									// Header File For The GLu32 Library
	#include <gl\glaux.h>								// Header File For The GLaux Library
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <time.h>
#include "helper/sglgrid.h"
#include "helper/sglcamera.h"
#include "helper/sglclipplane.h"
#include "helper/sgllight.h"
#include "helper/sgldisplist.h"
#include "text/sgltext.h"
#include "text/sglconsole.h"

#include "primitives/sglflobj.h"
#include <map>


/**Basisklasse für alle SGL- Raumklassen
  *@author Enrico Reimer
  */

class SGLSpace
{
public:
	int DoIdle;
	void OnIdle();
	
	struct
	{
		double OldX,OldY;
		bool FollowMouse,MovedPastDownBtn;
		unsigned short MouseMode;
		unsigned int DownBtns;
	}MouseInfo;

	struct MSVC_IST_ZU_BLEOD_FUER_ANONYME_KONSTRUKTOREN
	{
		SGLGrid *Grid1,*Grid2,*Grid3;
		SGL3DText *X,*Y,*Z;
		GLuint Beschr[3];
		MaterialPtr BeschrMat;
		short doGrid;
	}Grids;

	struct MSVC_IST_IMMERNOCH_ZU_BLEOD_FUER_ANONYME_KONSTRUKTOREN
	{
		bool glServerReady,Processing,update,FullScreen;
		char StatusString[4096];
		int	running,WindowWidth,WindowHeight,framecount,fps;
		time_t	time;
	}StatusInfo;
	
	struct
	{
		GLfloat r,g,b;
	}bgColor;


	SGLClipPlane	*ClipPlanes[5];
	boost::shared_ptr<SGLBaseCam> Camera;
	SGLLight		*StdLight;

	SGLObjList ObjLst;
	SGLObjList TranspObjLst;

	SGLSpace(unsigned int XSize=800,unsigned  int YSize=600,unsigned int R=0,unsigned int G=0,unsigned int B=0);
	virtual ~SGLSpace();
	virtual void OnResize(int width, int height);
	virtual void setFlags(bool reCompile=true);
	virtual void reDraw();

	void PrintOnScreen(char* String);
	void SetClipPlane(unsigned short int PlaneNr,GLdouble Ax,GLdouble By=0, GLdouble Cz=0,GLdouble D=0);

	void RotateCam(GLdouble RelX,GLdouble RelY,SGLBaseCam *Cam);
	void RotateCamAround(GLdouble RelX,GLdouble RelY,SGLBaseCam*Cam,SGLVektor &around);
	void RotateAim(GLdouble RelX,GLdouble RelY,SGLBaseCam *Cam);
	void MoveAim(GLdouble RelX,GLdouble RelY,SGLBaseCam *Cam);

	void TwoSided(bool TwoSideRendering=true);

	void CompileIntObs();
	bool reCompileIntObs(bool redraw=true);
	void SetRaumLicht(GLfloat R=0,GLfloat G=0, GLfloat B=0);
	void registerObj(shared_obj Obj);
	void unregisterObj(shared_obj Obj);
	void SetQuality(unsigned short int qual=1);
	void GetGlInfoString(char str[]);
	void printErrors();
	virtual bool setup_video(int w,int h)=0;
	void sglInit(unsigned int w=100,unsigned int h=100);
	static bool globalColorAktive;
	SGLConsole	*mainConsole;
	void defaultCam(boost::shared_ptr<SGLBaseCam> cam);
	struct {unsigned int x,y;}Size;
	SGLBaseCam::ViewModi resizeMode;

protected:
	void callHelper(int stage=1);
	void resetView(short mode=0);
	void DrawExtObjs();
	void show_status();
	bool initVis(unsigned int XSize, unsigned int YSize);
	bool cloned;
};

#endif
