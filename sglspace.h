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
#include <SDL/SDL.h>
#include <map>


/**Basisklasse für alle SGL- Raumklassen
  *@author Enrico Reimer
  */

class SGLSpace
{
public:
//	bool reCompile();
	bool Go(unsigned int Mode=1);
	int DoIdle;
	void OnIdle();
	
	multimap<Uint8, SDLEventListener> eventListeners;
	
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
		SGLMaterial BeschrMat;
		short doGrid;
	}Grids;

	struct
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

	bool FullScreen(bool FullScreen=true);

	SGLClipPlane	*ClipPlanes[5];
	SGLCamera		*Camera;
	SGLLight		*StdLight;
    SGLConsole		*mainConsole;

	SGLObjList ObjLst,TranspObjLst;
	SGLSpace(int XSize=800, int YSize=600,unsigned int R=0,unsigned int G=0,unsigned int B=0);
	~SGLSpace();
	virtual void OnDraw();
	virtual void OnMouseMove( int x, int y, unsigned int BtnDown );
	virtual void OnMouseBtn(int button, int state, int x, int y);
	virtual void OnResize(int width, int height);
	virtual void OnKey(SDL_keysym k, bool down);
	virtual void setFlags(bool reCompile=true);
	virtual void reDraw();

	void PrintOnScreen(char* String);
	void SetClipPlane(unsigned short int PlaneNr,GLdouble Ax,GLdouble By=0, GLdouble Cz=0,GLdouble D=0);

	void MoveCam(GLdouble RelX,GLdouble RelY,SGLCamera *Cam);
	void MoveAim(GLdouble RelX,GLdouble RelY,SGLCamera *Cam);

	void TwoSided(bool TwoSideRendering=true);

	void CompileIntObs();
	bool reCompileIntObs(bool redraw=true);
	void SetRaumLicht(GLfloat R=0,GLfloat G=0, GLfloat B=0);
	void OnMouseClick(int button, int x, int y);
	void registerObj(SGLFlObj *Obj);
	void registerObj(SGLObj *Obj);
	void SetQuality(unsigned short int qual=1);
	void GetGlInfoString(char str[]);
	void printErrors();
	void setTitle(const char title[]);

	void procEvent(SDL_Event e);

	virtual void OnQuit();
	void CallQuit();
	bool setVideoMode(int xsize=800, int ysize=600, bool FullScreen=false,bool fixedSize=false);
	void addEventListener(Uint8 eventType,SDLEventListener evl);
	void addEventListener(Uint8 eventType,SGLObjBase *target_obj,void (*target_func)(SGLObjBase *target,SDL_Event event));
	void callEventListeners(Uint8 type,SDL_Event &event);

private:
	bool setup_video(int w,int h);
	void resetView(short mode=0);
	void callHelper(int stage=1);
	void DrawExtObjs();
	void show_status();
};


#endif
