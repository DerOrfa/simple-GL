/***************************************************************************
                          sglspace.cpp  -  description
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

#include "sglspace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sglmisc.h"
#ifdef USE_DEVIL
	#include <IL/ilut.h>
#endif

#ifdef WIN32
	#include <windows.h>
	#include <gl\gl.h>
	#include <gl\glu.h>
	#include <gl\glaux.h>
#else
	#include <GL/gl.h>
#endif

#include "text/backend_glf/sgltextbackend_glf.h"
#include <assert.h>

bool SGLSpace::globalColorAktive=false;

void SGLSpace::reDraw()
{
  StatusInfo.update=true;
}

void SGLSpace::resetView(short mode)
{
	glClearDepth(1);
	glClearColor(bgColor.r,bgColor.g,bgColor.b,0);
	assert(Camera->Pos.size()==3 && Camera->LookAt.size()==3 && Camera->UpVect.size()==3);
	switch(mode)
	{
	case -1:
	//Normale 3-Dimensionale Ansicht entladen
	Camera->unloadView();break;
	case 0:
	//Normale 3-Dimensionale Ansicht laden
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	Camera->loadView();break;
	case 1:
	//2-Dimensionale Display-Ansicht (HUD z.B.)
	// @todo Nochmal SAUBER lösen
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	break;
	}
}

void SGLSpace::callHelper(int stage)
{
	switch(stage)
	{
	case 1:
	//Helper Zeichnen
	for(int i=0;i<4;i++)
		if(ClipPlanes[i]->ID)glCallList(ClipPlanes[i]->ID);
	
	if(Grids.doGrid & 1)glCallList(Grids.Grid1->ID);
	if(Grids.doGrid & 2)glCallList(Grids.Grid2->ID);
	if(Grids.doGrid & 4)glCallList(Grids.Grid3->ID);
	break;
	case 2:
	if(Grids.doGrid & 1)
	  {
		glEnable(GL_BLEND);
		if(Grids.X->should_compile)Grids.X->Compile();
		if(Grids.Y->should_compile)Grids.Y->Compile();
		if(Grids.Z->should_compile)Grids.Z->Compile();
		glCallLists(3,GL_UNSIGNED_INT,Grids.Beschr);
		glDisable(GL_BLEND);
	  }
	break;
	}
}

void SGLSpace::DrawExtObjs()
{
	ObjLst.CallAllLists();//Std-Objekte zeichnen
	TranspObjLst.CallAllLists(); //Transp-Obj zeichnen
}

void SGLSpace::show_status()
{
	if(DoIdle)sprintf(StatusInfo.StatusString,"%sfps: %d",StatusInfo.StatusString,StatusInfo.fps);
	PrintOnScreen(StatusInfo.StatusString);StatusInfo.StatusString[0]=0;
	if(mainConsole && !mainConsole->empty)glCallList(mainConsole->metaCompile());
}

SGLSpace::SGLSpace(unsigned int XSize, unsigned int YSize,unsigned int R,unsigned int G,unsigned int B)
{
	Grids.BeschrMat=MaterialPtr(new  SGLMaterial);
	TranspObjLst.renderTransparent=true;
	bgColor.r=float(R)/255;
	bgColor.g=float(G)/255;
	bgColor.b=float(B)/255;

	Camera=NULL;;
	MouseInfo.FollowMouse=true;
	resizeMode=SGLBaseCam::scaleView;
	isMyCam=DoIdle=StatusInfo.Processing=StatusInfo.glServerReady=StatusInfo.running=false;
	StatusInfo.StatusString[0]=StatusInfo.time=StatusInfo.framecount=StatusInfo.fps=0;
	MouseInfo.DownBtns=0;
	cloned=false;
}

SGLSpace::~SGLSpace()
{
	if(!cloned)//@todo Dieses cloned wird nicht automatisch gesetzt
	{
		delete Grids.Grid1;
		delete Grids.Grid2;
		delete Grids.Grid3;
		delete Grids.X;
		delete Grids.Y;
		delete Grids.Z;
		for(int i=0;i<5;i++)delete ClipPlanes[i];
	}
	if(isMyCam)delete Camera;

}

void SGLSpace::OnResize(int width, int height)
{
	StatusInfo.WindowWidth=width,StatusInfo.WindowHeight=height;
	if(Camera)
	{
		if(resizeMode!=SGLBaseCam::scaleView)
		{
			GLdouble screenDepth=Camera->ViewMatr.getDepth(Camera->LookAt);
			Camera->ViewMatr.screen2welt(width,0,screenDepth,*Camera->Ecken[0]);
			Camera->ViewMatr.screen2welt(0,0,screenDepth,*Camera->Ecken[1]);
			Camera->ViewMatr.screen2welt(0,height,screenDepth,*Camera->Ecken[2]);
			Camera->ViewMatr.screen2welt(width,height,screenDepth,*Camera->Ecken[3]);
		}
		Camera->recalcEckenMode=resizeMode;
		Camera->setView(width,height);
		Camera->Compile();
	}
	else
	{
		SGLprintError("Keine aktuelle Camera");
	}
	Size.x=width;
	Size.y=height;
	
	glViewport(0, 0, width, height);
 	reDraw();
}


void SGLSpace::setFlags(bool reCompile)
{
	GLuint error=0;
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0);
	
	glDisable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
//	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);/*GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA*/

	glDisable(GL_DITHER);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT,GL_FILL);

	glCullFace( GL_BACK );
	glShadeModel( GL_SMOOTH );

	//Die Texturen aktivieren, und deaktivieren ihre entspr. Modi selbst.
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);

	if(reCompile)
	  {
		CompileIntObs();
		ObjLst.Compile(true);
		TranspObjLst.Compile(true);
	  }
	printErrors();
}

void SGLSpace::PrintOnScreen(char* String)
{
	if(mainConsole)
	{
		mainConsole->clear();
		mainConsole->print(String);
	}
/*	else
	{SGLprintWarning("Es gibt keine Hauptkonsole");}*///@todo das nervt
}

void SGLSpace::SetClipPlane(unsigned short int PlaneNr,GLdouble Ax,GLdouble By, GLdouble Cz,GLdouble D)
{
	ClipPlanes[PlaneNr-1]->PlaneParam[0]=Ax;
	ClipPlanes[PlaneNr-1]->PlaneParam[1]=By;
	ClipPlanes[PlaneNr-1]->PlaneParam[2]=Cz;
	ClipPlanes[PlaneNr-1]->PlaneParam[3]=D;

	ClipPlanes[PlaneNr-1]->Compile();
	ClipPlanes[PlaneNr-1]->Activate();

	TwoSided();
}

void SGLSpace::RotateAim(GLdouble RelX,GLdouble RelY,SGLBaseCam*Cam)
{
	double Xval=(RelX-MouseInfo.OldX);
	double Yval=(RelY-MouseInfo.OldY);
	Cam->RotateAim(Xval*20,-Yval*20);
	sprintf(StatusInfo.StatusString,"%sZiel rotiert um: %.3f in X-Richtung und um: %.3f in Y-Richtung\n",StatusInfo.StatusString,Xval,Yval);
}

void SGLSpace::MoveAim(GLdouble RelX,GLdouble RelY,SGLBaseCam*Cam)
{
	double Xval=(RelX-MouseInfo.OldX);
	double Yval=(RelY-MouseInfo.OldY);
	SGLVektor V1=(Camera->UpVect.Rotate(Camera->getLookVektor(),-90))*Xval;
	SGLVektor V2=Camera->UpVect*Yval;
	SGLVektor V = (V1+V2)*-10;
	Cam->MoveAim(V.SGLV_X,V.SGLV_Y,V.SGLV_Z);
	sprintf(StatusInfo.StatusString,"%sZiel verschoben um: %.3f in X-Richtung, um: %.3f in Y-Richtung und um: %.3f in Z-Richtung\n",StatusInfo.StatusString,V.SGLV_X,V.SGLV_Y,V.SGLV_Z);
}

void SGLSpace::RotateCam(GLdouble RelX,GLdouble RelY,SGLBaseCam*Cam)
{
	RotateCamAround(RelX,RelY,Cam,Cam->LookAt);
}

void SGLSpace::RotateCamAround(GLdouble RelX,GLdouble RelY,SGLBaseCam*Cam,SGLVektor &around)
{
	double XRollFact=SIN(RelY*90),XRotateFact=COS(XRollFact*90);
	double YRollFact=SIN(RelX*90),YRotateFact=COS(YRollFact*90);
	double XRot=(RelX-MouseInfo.OldX);
	double YRot=(RelY-MouseInfo.OldY);

	Camera->RotateCamAround(-XRot*XRotateFact*40,-YRot*YRotateFact*40,around);
	Camera->Roll(XRot*30*XRollFact-YRot*30*YRollFact);

	sprintf(StatusInfo.StatusString,"%sCamera rotiert um: %.3f in X-Richtung und um: %.3f in Y-Richtung\n",StatusInfo.StatusString,XRot,YRot);
	sprintf(StatusInfo.StatusString,"%sXRotateFact: %.3f, XRollFact: %.3f\nYRotateFact: %.3f,YRollFact: %.3f\n",StatusInfo.StatusString,XRotateFact,XRollFact,YRotateFact,YRollFact);
}

void SGLSpace::OnIdle()
{
	Camera->RotateCam(1,1);
	reDraw();
}

/*
Schaltet den Default für zweiseitiges Rendering um.
Danach mssen alle PolygonObjekte neu Compiliert werden.
(Für die Standartobjekte und die aktuelle Camera wird das automatisch geregelt)
*/
void SGLSpace::TwoSided(bool TwoSideRendering)
{
	assert(StatusInfo.glServerReady);
	if(TwoSideRendering)glDisable(GL_CULL_FACE);
	else glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,TwoSideRendering);

	if(StatusInfo.glServerReady)CompileIntObs();
}

void SGLSpace::SetRaumLicht(GLfloat R,GLfloat G, GLfloat B)
{
//	ASSERT(StatusInfo.glServerReady);
	const GLfloat lMod[4]={R,G,B,1};
	if(StatusInfo.glServerReady)glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lMod);
	if(StatusInfo.running==1)reDraw();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SGLSpace::registerObj(SGLFlObj *Obj)
{
	if(Obj->Mat && Obj->Mat->Transparenz)
		TranspObjLst.AddOb(Obj);
	else ObjLst.AddOb(Obj);
}
void SGLSpace::unregisterObj(SGLFlObj *Obj)
{
	if(Obj->Mat && Obj->Mat->Transparenz)
		TranspObjLst.removeOb(Obj);
	else ObjLst.removeOb(Obj);
}
void SGLSpace::registerObj(SGLObj *Obj){ObjLst.AddOb(Obj);}
void SGLSpace::unregisterObj(SGLObj *Obj){ObjLst.removeOb(Obj);}

void SGLSpace::CompileIntObs()
{
	if(StatusInfo.glServerReady)
	{
		if(Grids.Grid1)Grids.Grid1->Compile();
		if(Grids.Grid2)Grids.Grid2->Compile();
		if(Grids.Grid3)Grids.Grid3->Compile();
	}
}
bool SGLSpace::reCompileIntObs(bool redraw)
{
	if(!StatusInfo.glServerReady)return false;
	else CompileIntObs();
	if(redraw)reDraw();
	return true;
}

void SGLSpace::SetQuality(unsigned short int qual)
{
	switch(qual)
	{
	case 0:
	{
		glHint(GL_FOG_HINT,GL_FASTEST);
		glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
		glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
		glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
	}break;
	case 1:
	{
		glHint(GL_FOG_HINT,GL_NICEST);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	}break;
	}
}

void SGLSpace::GetGlInfoString(char str[])
{
	str[0]=0;
	GLint LightCnt,RedBits,GreenBits,BlueBits,AlphaBits,ClipPlanesCnt;
	GLboolean RGBAMode;
	glGetIntegerv(GL_MAX_LIGHTS,&LightCnt);
	glGetIntegerv(GL_MAX_CLIP_PLANES,&ClipPlanesCnt);
	glGetBooleanv(GL_RGBA_MODE,&RGBAMode);

	sprintf(str,"%sOpenGL- Version: %s\nRenderer: %s\nHersteller: %s\n",str,glGetString(GL_VERSION),glGetString(GL_RENDERER),glGetString(GL_VENDOR));
//	sprintf(str,"%sExtensions: %s\n",str,glGetString(GL_EXTENSIONS));

	sprintf(str,"%sMaximal %d Lichtquellen\n",str,LightCnt);
	if(RGBAMode)sprintf(str,"%sRGBA- Modus\n",str);
	else sprintf(str,"%sFarb- Index- Modus\n",str);
}

void SGLSpace::printErrors()
{
	GLuint error=0;
	while(error=glGetError())
	{SGLprintError("%s [GLerror]",gluErrorString(GLenum(error)));}
}


/*!
    \fn SGLSpace::initVis()
 */
bool SGLSpace::initVis(unsigned int XSize, unsigned int YSize)
{
	if(setup_video(XSize,YSize))
	{
	#ifdef USE_DEVIL
	  ilInit();
	  iluInit();
	  ilutRenderer(ILUT_OPENGL);
	#endif
	  return true;
	}
	else
	{
		SGLprintError("Der GL-Raum konnte NICHT erzeugt werden");
		return false;
	}
}


/*!
    \fn SGLSpace::sglInit(unsigned int w,unsigned int h)
 */
void SGLSpace::sglInit(unsigned int w,unsigned int h)
{
	GLuint	error=0;
	if(!initVis(w,h))exit(1);
	setFlags(false);

	for(int i=0;i<5;i++)ClipPlanes[i]=new SGLClipPlane(GL_CLIP_PLANE0+i);

	Grids.Grid1= new SGLGrid(1);
	Grids.Grid2= new SGLGrid(2);
	Grids.Grid3= new SGLGrid(3);

	Grids.BeschrMat->Transparenz=.5;
	Grids.BeschrMat->SetColor(0,1,0);
	Grids.X= new SGL3DText("X","",Grids.BeschrMat,6,0,0,.5);
	Grids.Y= new SGL3DText("Y","",Grids.BeschrMat,0,6,0,.5);
	Grids.Z= new SGL3DText("Z","",Grids.BeschrMat,0,0,6,.5);

	Grids.Beschr[0]=Grids.X->Compile();
	Grids.Beschr[1]=Grids.Y->Compile();
	Grids.Beschr[2]=Grids.Z->Compile();

	assert(!isMyCam);
	defaultCam(new SGLCamera());
	isMyCam=true;

	Grids.doGrid=1;

	if(glIsEnabled(GL_LIGHTING))
	{
		StdLight= new SGLLight();
		StdLight->CamLight();
	}
	else StdLight=0;

	GetGlInfoString(StatusInfo.StatusString);

	OnResize(w,h);

	while(error=glGetError())
	{SGLprintError("%s [GLerror]",gluErrorString(GLenum(error)));}
}

void SGLSpace::defaultCam(SGLBaseCam *cam)
{
	assert(cam);
	if(isMyCam)
		delete Camera;
	isMyCam=false;
	registerObj(Camera=cam);
	if(StatusInfo.WindowHeight>0 && StatusInfo.WindowWidth>0)
		Camera->setView(StatusInfo.WindowWidth,StatusInfo.WindowHeight);
	else {SGLprintWarning("Die Bilddimensionen (%d x %d) sind ungültig",StatusInfo.WindowHeight,StatusInfo.WindowWidth);}
	Camera->Compile();

	Grids.X->FaceAt=&Camera->Pos;
	Grids.Y->FaceAt=&Camera->Pos;
	Grids.Z->FaceAt=&Camera->Pos;

	Camera->link(Grids.X);
	Camera->link(Grids.Y);
	Camera->link(Grids.Z);
}

