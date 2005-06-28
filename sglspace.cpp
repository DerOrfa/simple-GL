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
#include "sglshptr.h"

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
#include "util/sglmaterial.h"

bool SGLSpace::globalColorAktive=false;

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
		if(Grids.X->should_compile)
			Grids.X->Compile(true,true);
		if(Grids.Y->should_compile)
			Grids.Y->Compile(true,true);
		if(Grids.Z->should_compile)
			Grids.Z->Compile(true,true);
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
//	if(DoIdle)sprintf(StatusInfo.StatusString,"%sfps: %d",StatusInfo.StatusString,StatusInfo.fps); //@todo Animationen mal anders lösen
	PrintOnScreen(StatusInfo.StatusString);StatusInfo.StatusString[0]=0;
	if(mainConsole && !mainConsole->empty)glCallList(mainConsole->metaCompile());
}


/**
 * Kopierkonstruktor.
 * Kopiert src. Dabei wird auch sein reDraw-Slot kopiert. 
 * Folglich wird die entsprechende Debug-Warnung zur Laufzeit ausgegeben.
 * Der interne Zeiger des Slot wird jedoch reinitialisiert, die Warnung kann also in diesem Fall ignoriert werden.
 * @param src Der Space von dem ein Kopie erstellt werden soll. 
 */
SGLSpace::SGLSpace(SGLSpace &src):reDraw(NULL)
{
	*this = src;
	reDraw.myspace=this;//*hehe* dirty
}

/**
 * Standardkonstruktor.
 * @param XSize Die Breite des GL-Views bei der Initialisierung.
 * @param YSize Die Höhe des GL-Views bei der Initialisierung.
 * @param R Der Rotanteil der Hintergrundfarbe
 * @param G Der Grünanteil der Hintergrundfarbe
 * @param B Der Blauanteil der Hintergrundfarbe
 */
SGLSpace::SGLSpace(unsigned int XSize, unsigned int YSize,unsigned int R,unsigned int G,unsigned int B):reDraw(this)
{
	Grids.BeschrMat=MaterialPtr(new  SGLMaterial);
	TranspObjLst.renderTransparent=true;
	bgColor.r=float(R)/255;
	bgColor.g=float(G)/255;
	bgColor.b=float(B)/255;

	MouseInfo.OldX=MouseInfo.OldY=0;
	MouseInfo.FollowMouse=true;
	resizeMode=SGLBaseCam::scaleView;
	StatusInfo.Processing=StatusInfo.glServerReady=StatusInfo.running=false;
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
}

/**
 * Ereignissbehandlung für Änderung der Größe des GL-Views.
 * @param width die neue Breite
 * @param height die neue Höhe
 */
void SGLSpace::OnResize(int width, int height)
{
	StatusInfo.WindowWidth=width,StatusInfo.WindowHeight=height;
	if(Camera)
	{
		if(resizeMode!=SGLBaseCam::scaleView)
		{
			Camera->confirmViewMat();
			GLdouble screenDepth=Camera->ViewMatr.getDepth(Camera->LookAt);
			Camera->ViewMatr.screen2welt(width,0,screenDepth,*Camera->Ecken[0]);
			Camera->ViewMatr.screen2welt(0,0,screenDepth,*Camera->Ecken[1]);
			Camera->ViewMatr.screen2welt(0,height,screenDepth,*Camera->Ecken[2]);
			Camera->ViewMatr.screen2welt(width,height,screenDepth,*Camera->Ecken[3]);
		}
		Camera->setView(width,height);
		Camera->recalcEckenMode=resizeMode;
		Camera->Compile();
	}
	else
	{
		SGLprintError("Keine aktuelle Camera");
	}
	Size.x=width;
	Size.y=height;
	
	glViewport(0, 0, width, height);
 	StatusInfo.update=true;//@todo sollte u.U. automatisch neu zeichnen
}


/**
 * Setzt einige Parameter des Renderers auf "Standard"-Werte
 * @param reCompile wenn true wird der komplette Raum nach Setzen der Parameter neu generiert
 */
void SGLSpace::setFlags(bool reCompile)
{
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


/**
 * Gibt Text auf der mainConsole aus. 
 * Wird ignoriert, wenn (noch) keine mainConsole existiert.
 * @param String 
 */
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

/**
 * Setzt eine Clipping-Plane.
 * Die entsprechnde Plane wird initialisiert und positioniert. Außerdem wird zweiseitiges Rendering aktiviert.
 * @param PlaneNr die Nummer der zu aktivierenden Plane. ( 0 <= PlaneNr \< GL_MAX_CLIP_PLANES)
 * @param Ax 
 * @param By 
 * @param Cz 
 * @param D 
 */
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

/**
 * Rotiert den Blickpunkt der angegeben Camera um diese.
 * @param RelX Roatation nach rechts/links aus Sicht des Betrachters
 * @param RelY Roatation nach oben/unten aus Sicht des Betrachters
 * @param Cam Die Camera deren Blickpunkt rotieren soll
 */
void SGLSpace::RotateAim(GLdouble RelX,GLdouble RelY,SGLBaseCam &Cam)
{
	double Xval=(RelX-MouseInfo.OldX)*.01*localConf.aimRotSpeed;
	double Yval=(RelY-MouseInfo.OldY)*.01*localConf.aimRotSpeed;
	Cam.RotateAim(Xval*20,-Yval*20);
	sprintf(StatusInfo.StatusString,"%sZiel rotiert um: %.3f in X-Richtung und um: %.3f in Y-Richtung\n",StatusInfo.StatusString,Xval,Yval);
}

/**
 * Verschiebt den Blickpunkt der 
 * @param RelX Verschieben nach rechts/links aus Sicht des Betrachters
 * @param RelY Verschieben nach oben/unten aus Sicht des Betrachters
 * @param Cam Die Camera deren Blickpunkt verschoben werden soll
 */
void SGLSpace::MoveAim(GLdouble RelX,GLdouble RelY,SGLBaseCam &Cam)
{
	double Xval=(RelX-MouseInfo.OldX);
	double Yval=(RelY-MouseInfo.OldY);
	SGLVektor V1=(Cam.UpVect.Rotate(Cam.getLookVektor(),-90))*Xval;
	SGLVektor V2=Cam.UpVect*Yval;
	SGLVektor V = (V1+V2)*-.1*localConf.aimMoveSpeed;
	Cam.MoveAim(V.SGLV_X,V.SGLV_Y,V.SGLV_Z);
	sprintf(StatusInfo.StatusString,"%sZiel verschoben um: %.3f in X-Richtung, um: %.3f in Y-Richtung und um: %.3f in Z-Richtung\n",StatusInfo.StatusString,V.SGLV_X,V.SGLV_Y,V.SGLV_Z);
}

/**
 * Rotiert die angegebe Camera um ihren Blickpunkt.
 * @param RelX Roatation nach rechts/links aus Sicht des Betrachters
 * @param RelY Roatation nach oben/unten aus Sicht des Betrachters
 * @param Cam Die Camera die rotieren soll
 */
void SGLSpace::RotateCam(GLdouble RelX,GLdouble RelY,SGLBaseCam &Cam)
{
	RotateCamAround(RelX,RelY,Cam,Cam.LookAt);
}

void SGLSpace::RotateCamAround(GLdouble RelX,GLdouble RelY,SGLBaseCam &Cam,SGLVektor &around)
{
	double XRollFact=SIN(RelY*90),XRotateFact=COS(XRollFact*90);
	double YRollFact=SIN(RelX*90),YRotateFact=COS(YRollFact*90);
	double XRot=(RelX-MouseInfo.OldX)*.01*localConf.camRotSpeed;
	double YRot=(RelY-MouseInfo.OldY)*.01*localConf.camRotSpeed;

	Camera->RotateCamAround(-XRot*XRotateFact*40,-YRot*YRotateFact*40,around);
	Camera->Roll(XRot*30*XRollFact-YRot*30*YRollFact);

	if(StatusInfo.StatusString)sprintf(StatusInfo.StatusString,"%sCamera rotiert um: %.3f in X-Richtung und um: %.3f in Y-Richtung\n",StatusInfo.StatusString,XRot,YRot);
	if(StatusInfo.StatusString)sprintf(StatusInfo.StatusString,"%sXRotateFact: %.3f, XRollFact: %.3f\nYRotateFact: %.3f,YRollFact: %.3f\n",StatusInfo.StatusString,XRotateFact,XRollFact,YRotateFact,YRollFact);
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
	if(StatusInfo.running==1)StatusInfo.update=true;//@todo sollte u.U. automatisch neu zeichnen
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SGLSpace::CompileIntObs()
{
	if(StatusInfo.glServerReady)
	{
		if(Grids.Grid1)Grids.Grid1->Compile(true,true);
		if(Grids.Grid2)Grids.Grid2->Compile(true,true);
		if(Grids.Grid3)Grids.Grid3->Compile(true,true);
	}
}
bool SGLSpace::reCompileIntObs(bool redraw)
{
	if(!StatusInfo.glServerReady)return false;
	else CompileIntObs();
	if(redraw)StatusInfo.update=true;//@todo sollte u.U. automatisch neu zeichnen

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
	GLint ClipPlanesCnt,LightCnt;
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
	while((error=glGetError()))
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

	for(int i=0;i<5;i++)ClipPlanes[i]=new SGLClipPlane(GL_CLIP_PLANE0+i);

	Grids.Grid1= new SGLGrid(1);
	Grids.Grid2= new SGLGrid(2);
	Grids.Grid3= new SGLGrid(3);

	Grids.BeschrMat->Transparenz=.5;
	Grids.BeschrMat->SetColor(0,1,0);
	Grids.X= new SGL3DText("X","",Grids.BeschrMat,6,0,0,.5);
	Grids.Y= new SGL3DText("Y","",Grids.BeschrMat,0,6,0,.5);
	Grids.Z= new SGL3DText("Z","",Grids.BeschrMat,0,0,6,.5);

	Grids.Beschr[0]=Grids.X->Compile(true,true);
	Grids.Beschr[1]=Grids.Y->Compile(true,true);
	Grids.Beschr[2]=Grids.Z->Compile(true,true);

	defaultCam(SGLshPtr<SGLBaseCam>(new SGLCamera()));
	if(!initVis(w,h))exit(1);
	setFlags(false);

	Grids.doGrid=1;

	if(glIsEnabled(GL_LIGHTING))
	{
		StdLight= new SGLLight();
		StdLight->CamLight();
	}
	else StdLight=0;

	GetGlInfoString(StatusInfo.StatusString);

	OnResize(w,h);

	while((error=glGetError()))
	{SGLprintError("%s [GLerror]",gluErrorString(GLenum(error)));}
}

void SGLSpace::defaultCam(SGLshPtr<SGLBaseCam> cam)
{
	assert(cam);
	unregisterObj(Camera);
	registerObj(Camera=cam);
	if(StatusInfo.WindowHeight>0 && StatusInfo.WindowWidth>0)
		Camera->setView(StatusInfo.WindowWidth,StatusInfo.WindowHeight);
	else {SGLprintWarning("Die Bilddimensionen (%d x %d) sind ungültig",StatusInfo.WindowHeight,StatusInfo.WindowWidth);}
	Camera->Compile();

	Grids.X->FaceAt=&Camera->Pos;
	Grids.Y->FaceAt=&Camera->Pos;
	Grids.Z->FaceAt=&Camera->Pos;

	Camera->link(*Grids.X);
	Camera->link(*Grids.Y);
	Camera->link(*Grids.Z);
}



/*!
    \fn SGLSpace::setGridsSize(GLuint size)
 */
void SGLSpace::setGridsSize(GLuint size)
{
  //	GLuint oldsize=Grids.Grid1->Size;
	Grids.Grid1->setSize(size);
	Grids.Grid2->setSize(size);
	Grids.Grid3->setSize(size);
	
	Grids.X->MoveTo(size+size/10.,0,0);
	Grids.Y->MoveTo(0,size+size/10.,0);
	Grids.Z->MoveTo(0,0,size+size/10.);
	Grids.X->Scale(size/10.);
	Grids.Y->Scale(size/10.);
	Grids.Z->Scale(size/10.);
	
	Grids.X->compileNextTime();
	Grids.Y->compileNextTime();
	Grids.Z->compileNextTime();
}


/*!
    \fn SGLSpace::registerDynamicTex(const SGLBaseTex &tex)
 */
void SGLSpace::registerDynamicTex(SGLBaseTex &tex)
{
	tex.changed.connect(reDraw);
}

void SGLSpace::draw()
{
	if(!StatusInfo.Processing)
	{
		StatusInfo.Processing=true;
		resetView(0);
		callHelper(1);
		DrawExtObjs();
		callHelper(2);

		resetView(-1);
		resetView(1);
		show_status();
	
		printErrors();
		glFinish();

		StatusInfo.Processing=false;
	}
	else {SGLprintError("Tue grad tun");}
}

SGLSpace::redrawSlot::redrawSlot(SGLSpace *myspace){this->myspace =myspace;}
void SGLSpace::redrawSlot::operator()() 
{
	if(myspace)myspace->callUpdate();
	else{SGLprintError("redrawSlot 0x%X hat keinen Space, löse nicht aus",this);}
}
void SGLSpace::redrawSlot::operator=(redrawSlot &Slot)
{
	myspace=NULL;
}

SGLSpace::spaceConfig SGLSpace::globalConf;
