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
#include <IL/ilut.h>

#ifdef WIN32
	#include <windows.h>
	#include <gl\gl.h>
	#include <gl\glu.h>
	#include <gl\glaux.h>
#else
	#include <GL/gl.h>
#endif

#include "text/backend_glf/sgltextbackend_glf.h"


void SGLSpace::procEvent(SDL_Event e)
{
  switch( e.type )
    {
	case SDL_KEYDOWN:OnKey(e.key.keysym ,true);break;
	case SDL_MOUSEMOTION:OnMouseMove(e.motion.x,e.motion.y,e.motion.state);break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:OnMouseBtn(e.button.button,e.button.state,e.button.x,e.button.y);break;
	case SDL_QUIT:OnQuit();exit(0);break;//Beendende Signale von OS (SIGINT, Fenster geschlossen ...)
	case SDL_VIDEORESIZE:setVideoMode(e.resize.w,e.resize.h);
    }
	callEventListeners(e.type,e);
}

bool SGLSpace::setup_video(int width, int height)
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		SGLprintError("Konnte SDL's video-subsystem nicht initialisieren: %s",SDL_GetError( ) );
		exit(1);
	}
	else atexit(SDL_Quit);//Bei exit autom. Aufräumen

	setVideoMode(width, height);

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
	return StatusInfo.glServerReady=true;
}

void SGLSpace::reDraw()
{
  StatusInfo.update=true;
}

void SGLSpace::resetView(short mode)
{
	glClearDepth(1);
	glClearColor(bgColor.r,bgColor.g,bgColor.b,0);
	switch(mode)
	{
	case 0:
	//Normale 3-Dimensionale Ansicht
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(Camera->Angle,double(StatusInfo.WindowWidth)/double(StatusInfo.WindowHeight),Camera->ClipFace,Camera->ClipHoriz);
	glMatrixMode(GL_MODELVIEW);

	gluLookAt(Camera->Pos.SGLV_X,Camera->Pos.SGLV_Y,Camera->Pos.SGLV_Z,
		  Camera->LookAt.SGLV_X,Camera->LookAt.SGLV_Y,Camera->LookAt.SGLV_Z,
		  Camera->UpVect.SGLV_X,Camera->UpVect.SGLV_Y,Camera->UpVect.SGLV_Z);
	break;
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
	glCallList(ClipPlanes[0]->ID);
	glCallList(ClipPlanes[1]->ID);
	glCallList(ClipPlanes[2]->ID);
	glCallList(ClipPlanes[3]->ID);


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
	if(!mainConsole->empty)glCallList(mainConsole->metaCompile());
}

void SGLSpace::OnDraw()
{
	resetView();
	callHelper(1);
	DrawExtObjs();
	callHelper(2);

	resetView(1);
	show_status();

	printErrors();

	glFinish();
	SDL_GL_SwapBuffers( );

	StatusInfo.update=StatusInfo.Processing=false;
}

void SGLSpace::setTitle(const char title[])
{
  char *dummyTitle,*icon;
  SDL_WM_GetCaption(&dummyTitle, &icon);
  SDL_WM_SetCaption(title, icon);
}

SGLSpace::SGLSpace(int XSize, int YSize,unsigned int R,unsigned int G,unsigned int B)
{
	GLuint	error=0;
	TranspObjLst.renderTransparent=true;
	bgColor.r=float(R)/255;
	bgColor.g=float(G)/255;
	bgColor.b=float(B)/255;

	MouseInfo.FollowMouse=true;
	DoIdle=StatusInfo.glServerReady=StatusInfo.running=false;
	StatusInfo.StatusString[0]=StatusInfo.time=StatusInfo.framecount=StatusInfo.fps=0;
	MouseInfo.DownBtns=0;

	if(setup_video(XSize,YSize))
	{
	  char TitleString[255];
	  ilutRenderer(ILUT_OPENGL);

	  sprintf(TitleString,"OpenGL: %s, Renderer: %s von %s",glGetString(GL_VERSION),glGetString(GL_RENDERER),glGetString(GL_VENDOR));
	  setTitle(TitleString);
	}
	else
	{
		SGLprintError("Der GL-Raum konnte NICHT erzeugt werden");
		exit(1);
	}

	setFlags(false);

	for(int i=0;i<5;i++)ClipPlanes[i]=new SGLClipPlane(GL_CLIP_PLANE0+i);

	Grids.Grid1= new SGLGrid(1);
	Grids.Grid2= new SGLGrid(2);
	Grids.Grid3= new SGLGrid(3);
	mainConsole= new SGLConsole(8,4);

	Grids.BeschrMat.Transparenz=.5;
	Grids.BeschrMat.SetColor(0,1,0);
	Grids.X= new SGL3DText("X","",&Grids.BeschrMat,6,0,0,.5);
	Grids.Y= new SGL3DText("Y","",&Grids.BeschrMat,0,6,0,.5);
	Grids.Z= new SGL3DText("Z","",&Grids.BeschrMat,0,0,6,.5);

	Grids.Beschr[0]=Grids.X->Compile();
	Grids.Beschr[1]=Grids.Y->Compile();
	Grids.Beschr[2]=Grids.Z->Compile();

	registerObj(Camera=new SGLCamera());
	Grids.X->FaceAt=&Camera->Pos;
	Grids.Y->FaceAt=&Camera->Pos;
	Grids.Z->FaceAt=&Camera->Pos;

	Camera->link(Grids.X);
	Camera->link(Grids.Y);
	Camera->link(Grids.Z);

	mainConsole->Scale(.2);
	mainConsole->Move(-.95,2.95,0);
	mainConsole->Compile();

	Grids.doGrid=1;

	if(glIsEnabled(GL_LIGHTING))
	{
		StdLight= new SGLLight();
		StdLight->CamLight();
	}
	else StdLight=0;

	GetGlInfoString(StatusInfo.StatusString);

	OnResize(XSize,YSize);

	while(error=glGetError())
	{SGLprintError("%s [GLerror]",gluErrorString(GLenum(error)));}
}

SGLSpace::~SGLSpace()
{
	delete Grids.Grid1;
	delete Grids.Grid2;
	delete Grids.Grid3;
	delete Grids.X;
	delete Grids.Y;
	delete Grids.Z;
	delete Camera;
	for(int i=0;i<5;i++)delete ClipPlanes[i];
}

void SGLSpace::OnMouseMove( int x, int y ,unsigned int BtnDown)
{
	if(StatusInfo.Processing)return;
	else StatusInfo.Processing=true;
	MouseInfo.MovedPastDownBtn=true;
	if(MouseInfo.FollowMouse && StatusInfo.WindowWidth && StatusInfo.WindowHeight)
	{
		double RelXPos=(x-double(StatusInfo.WindowWidth)/2)/double(StatusInfo.WindowWidth)*2;
		double RelYPos=-(y-double(StatusInfo.WindowHeight)/2)/double(StatusInfo.WindowHeight)*2;

		if(BtnDown&SDL_BUTTON(1))MoveCam(RelXPos,RelYPos,Camera);
		else if(BtnDown&SDL_BUTTON(3))	MoveAim(RelXPos,RelYPos,Camera);
		else if(BtnDown&SDL_BUTTON(4))
		{
			ClipPlanes[0]->Rotate(5,0,0,MouseInfo.OldX-RelXPos);
			ClipPlanes[0]->Rotate(0,5,0,MouseInfo.OldY-RelYPos);
			ClipPlanes[0]->Compile();
		}

		MouseInfo.OldX=RelXPos;
		MouseInfo.OldY=RelYPos;

		sprintf(StatusInfo.StatusString,"%sRelXPos: %.3f, RelYPos: %.3f\n",StatusInfo.StatusString,RelXPos,RelYPos);
		sprintf(StatusInfo.StatusString,"%sDownBtns: %d\n",StatusInfo.StatusString,MouseInfo.DownBtns);
		reDraw();
	}
}

void SGLSpace::OnMouseBtn(int button, int state, int x, int y)
{
	if(state==SDL_PRESSED)
	  {	//gedrückt
	    MouseInfo.MovedPastDownBtn=false;
	    MouseInfo.DownBtns=button;
	  }
	else
	  {	//losgelassen
	    if(!MouseInfo.MovedPastDownBtn)OnMouseClick(button,x,y);
	    MouseInfo.DownBtns=button;
	  }
	sprintf(StatusInfo.StatusString,"%sDownBtns: %d\n",StatusInfo.StatusString,MouseInfo.DownBtns);
	reDraw();
}

void SGLSpace::OnResize(int width, int height)
{
	StatusInfo.WindowWidth=width,StatusInfo.WindowHeight=height;
	if(Camera)
	{
		Camera->ViewFormat=double(width)/double(height);
		Camera->Compile();
	}
	glViewport(0, 0, width, height);
 	reDraw();
}

void SGLSpace::OnKey(SDL_keysym k, bool down)
{
	if(down)switch (k.sym)
	{
	case SDLK_LEFT:Camera->RotateCam(-2,0);break;
	case SDLK_RIGHT:Camera->RotateCam(2,0);break;
	case SDLK_UP:Camera->RotateCam(0,2);break;
	case SDLK_DOWN:Camera->RotateCam(0,-2);break;
	case SDLK_PAGEUP:Camera->MoveZoom(0.95);break;
	case SDLK_PAGEDOWN:Camera->MoveZoom(1.05);break;
	case SDLK_HOME:Camera->OptZoom(1.05);break;
	case SDLK_END:Camera->OptZoom(0.95);break;
	case SDLK_INSERT:Camera->Roll(-1);break;
	case SDLK_F1:Grids.doGrid^=1;break;
	case SDLK_F2:Grids.doGrid^=2;break;
	case SDLK_F3:Grids.doGrid^=4;break;
	case SDLK_F12:FullScreen(!StatusInfo.FullScreen);break;// @todo Crasht unter Win ... (ka warum ... bleibt in NTDLL hängen)
	case SDLK_RETURN:Camera->ResetView();break;//Enter
	case SDLK_DELETE:Camera->Roll(1);break;//Entf
	case SDLK_ESCAPE:CallQuit();break;//ESC
	default:
		sprintf(StatusInfo.StatusString,"%sF1-F3 => Koordinatensysteme umschalten\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%slinke Maus / Cursor => Kamera rotieren (um das Ziel)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%srechte Maus => Ziel rotieren (um die Kamera)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%srechte Maus Click => Kameralicht ein/aus\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sMausrad / BildAuf|BildAb => Phys. Zoom (Kamera auf Ziel zubew.)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sHome|End => Opt. Zoom (Brennweite ändern)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sIns|Del => Kamera rollen (auf Sichtachse rotieren)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sEnter => Kamera zurück auf Ausgangspos.\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sESC => beenden\n",StatusInfo.StatusString);
		reDraw();
	break;
	}
 	reDraw();
}

void SGLSpace::setFlags(bool reCompile)
{
	GLuint error=0;
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
//	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DITHER);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT,GL_FILL);

	glCullFace( GL_BACK );
	glShadeModel( GL_SMOOTH );


	if(reCompile)
	  {
		CompileIntObs();
		ObjLst.Compile(true);
		TranspObjLst.Compile(true);
	  }
	printErrors();
}

bool SGLSpace::FullScreen(bool FullScreen)
{
	return setVideoMode(StatusInfo.WindowWidth,StatusInfo.WindowHeight,FullScreen);
}

void SGLSpace::PrintOnScreen(char* String)
{
	mainConsole->clear();
	mainConsole->print(String);
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

void SGLSpace::MoveAim(GLdouble RelX,GLdouble RelY,SGLCamera *Cam)
{
	double XRot=(RelX-MouseInfo.OldX);
	double YRot=(RelY-MouseInfo.OldY);

	Cam->RotateAim(XRot*20,-YRot*20);

	sprintf(StatusInfo.StatusString,"%sZiel rotiert um: %.3f° in X-Richtung und um: %.3f° in Y-Richtung\n",StatusInfo.StatusString,XRot,YRot);
}

void SGLSpace::MoveCam(GLdouble RelX,GLdouble RelY,SGLCamera *Cam)
{
	double XRollFact=SIN(RelY*90),XRotateFact=COS(XRollFact*90);
	double YRollFact=SIN(RelX*90),YRotateFact=COS(YRollFact*90);
	double XRot=(RelX-MouseInfo.OldX);
	double YRot=(RelY-MouseInfo.OldY);

	Camera->RotateCam(-XRot*XRotateFact*40,-YRot*YRotateFact*40);
	Camera->Roll(XRot*30*XRollFact-YRot*30*YRollFact);

	sprintf(StatusInfo.StatusString,"%sCamera rotiert um: %.3f° in X-Richtung und um: %.3f° in Y-Richtung\n",StatusInfo.StatusString,XRot,YRot);
	sprintf(StatusInfo.StatusString,"%sXRotateFact: %.3f, XRollFact: %.3f\nYRotateFact: %.3f,YRollFact: %.3f\n",StatusInfo.StatusString,XRotateFact,XRollFact,YRotateFact,YRollFact);
}

void SGLSpace::OnIdle()
{
	Camera->RotateCam(1,1);
	reDraw();
}

bool SGLSpace::Go(unsigned int Mode)
{
  //Mode MUß zwischen 1 und 5 liegen
  // @todo .... naja mal schauen
  DoIdle=Mode-1;
  if(StatusInfo.glServerReady)
    {
      if(!StatusInfo.running)
	{
	  CompileIntObs();
	  StatusInfo.running=Mode;
	  while(StatusInfo.running)
	    {
	      if(!StatusInfo.Processing)
		{
		  SDL_Event e;
		  if(DoIdle)
		    {
		      if(DoIdle<4)SDL_Delay(30);//Idle mit Bremse
		      OnIdle();
		    }
		  else if(SDL_WaitEvent(&e))procEvent(e);//Kein Idle => wartet auf Event

		  while(SDL_PollEvent(&e))procEvent(e);//Events einsammeln
		  if(StatusInfo.update)
			  OnDraw();//bei Bedarf neu Zeichnen
		}
		  else {SGLprintWarning("Tue grad tun");}
	    }
	}
      else return false;
    }
  return true;
}

/*
Schaltet den Default für zweiseitiges Rendering um.
Danach müssen alle PolygonObjekte neu Compiliert werden.
(Für die Standartobjekte und die aktuelle Camera wird das automatisch geregelt)
*/
void SGLSpace::TwoSided(bool TwoSideRendering)
{
//	ASSERT(StatusInfo.glServerReady);
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

/*
Wird aufgerufen, wenn "geklickt" wurde.
(eine Maustaste gedrückt und wieder losgelassen, ohne die Maus zu bewegen)
*/
void SGLSpace::OnMouseClick(int button, int x, int y)
{
	switch(button)
	{
	case SDL_BUTTON_RIGHT:StdLight->On(!StdLight->IsOn());break;
	case SDL_BUTTON_LEFT:break;
	case 4:
	  {
	    Camera->MoveZoom(0.95);
	    sprintf(StatusInfo.StatusString,"%sEntfernung zum Ziel verringert auf %.3f\n",StatusInfo.StatusString,Camera->ViewLength());
	  }break;
	case 5:
	  {
	    Camera->MoveZoom(1.05);
	    sprintf(StatusInfo.StatusString,"%sEntfernung zum Ziel erhöht auf %.3f\n",StatusInfo.StatusString,Camera->ViewLength());
	  }break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SGLSpace::registerObj(SGLFlObj *Obj)
{
	if(Obj->Mat && Obj->Mat->Transparenz)TranspObjLst.AddOb(Obj);
	else ObjLst.AddOb(Obj);
}
void SGLSpace::registerObj(SGLObj *Obj){ObjLst.AddOb(Obj);}

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
    \fn SGLSpace::CallQuit()
 */
void SGLSpace::CallQuit()
{
	SDL_Event user_event;
	user_event.type=SDL_QUIT;
	SDL_PushEvent(&user_event);
}

/*!
    \fn SGLSpace::OnQuit()
 */
void SGLSpace::OnQuit()
{SGLprintInfo("Quit-Event raised");}

/*!
    \fn SGLSpace::setVideoMode(int xsize, int ysize, bool FullScreen=false,bool fixedSize=false)
 */
bool SGLSpace::setVideoMode(int width, int height, bool FullScreen,bool fixedSize)
{
	int flags=SDL_OPENGL;
	/* Let's get some video information. */
    const SDL_VideoInfo* info = SDL_GetVideoInfo( );
	if(FullScreen)flags|=SDL_FULLSCREEN;
	if(!fixedSize)flags|=SDL_RESIZABLE;

	if(!info)
	{
		/* This should probably never happen. */
		SGLprintError("Video query failed: %s",SDL_GetError( ) );
		return false;
	}

    if( SDL_SetVideoMode( width, height, info->vfmt->BitsPerPixel, flags) == 0 )
	{
        SGLprintError("Video mode set failed: %s", SDL_GetError( ) );
        return false;
    }

	StatusInfo.WindowWidth=width;
	StatusInfo.WindowHeight=height;
	if(Camera)
	{
		Camera->ViewFormat=double(width)/double(height);
		Camera->Compile();
	}
	StatusInfo.FullScreen=FullScreen;
	glViewport(0, 0, width, height);
	reDraw();
	return true;
}

void SGLSpace::addEventListener(Uint8 eventType,SDLEventListener evl)
{
	eventListeners.insert(pair<Uint8 , SDLEventListener>(eventType,evl));
}

void SGLSpace::addEventListener(Uint8 eventType,
	SGLObjBase *target_obj,
	void (*target_func)(SGLObjBase *target,SDL_Event event))
{
	SDLEventListener evl={target_obj,target_func};
	addEventListener(eventType,evl);
}

/*!
    \fn SGLSpace::callEventListeners(Uint8 type,SDL_Event event)
 */
void SGLSpace::callEventListeners(Uint8 type,SDL_Event &event)
{
	for(multimap<Uint8, SDLEventListener>::iterator it=eventListeners.find(type);
		it!=eventListeners.end();
		it++)
	{
		SGLObjBase *target=it->second.target;
		it->second.target_func(target,event);
		Uint8 newType=it->first;
		if(newType!=type)break;
	}
}  

