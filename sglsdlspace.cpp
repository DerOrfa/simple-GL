//
// C++ Implementation: sglsdlspace
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sglsdlspace.h"

SGLsdlSpace::SGLsdlSpace(int XSize, int YSize,unsigned int R,unsigned int G,unsigned int B) : SGLSpace(XSize,YSize,R,G,B)
{
	sglInit(XSize,YSize);
	  char TitleString[255];
	  sprintf(TitleString,"OpenGL: %s, Renderer: %s von %s",glGetString(GL_VERSION),glGetString(GL_RENDERER),glGetString(GL_VENDOR));
	  setTitle(TitleString);
	mainConsole= new SGLConsole(8,4);
	mainConsole->Scale(.2);
	mainConsole->Move(-.95,2.95,0);
	
	mainConsole->Compile();
}

void SGLsdlSpace::procEvent(SDL_Event e)
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

bool SGLsdlSpace::setup_video(int width, int height)
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

void SGLsdlSpace::OnKey(SDL_keysym k, bool down)
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
	case SDLK_F12:FullScreen(!StatusInfo.FullScreen);break;// @todo Crasht unter Win ... (ka warum ... bleibt in NTDLL hï¿½gen)
	case SDLK_RETURN:Camera->ResetView();break;//Enter
	case SDLK_DELETE:Camera->Roll(1);break;//Entf
	case SDLK_ESCAPE:CallQuit();break;//ESC
	default:
		StatusInfo.StatusString[0]=0;
		//@todo Wenn das Event mehrfach aufgerufen wird, ohne das show_status aufgerufen wurde, wird das ganze immer lï¿½ger. Und lï¿½ft irgendwann ber
		//@todo Diese Lï¿½ung ist aber auch nicht so toll, denn damit geht alles was vorher hier stand verloren
		sprintf(StatusInfo.StatusString,"%sF1-F3 => Koordinatensysteme umschalten\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%slinke Maus / Cursor => Kamera rotieren (um das Ziel)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%srechte Maus => Ziel rotieren (um die Kamera)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%srechte Maus Click => Kameralicht ein/aus\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sMausrad / BildAuf|BildAb => Phys. Zoom (Kamera auf Ziel zubew.)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sHome|End => Opt. Zoom (Brennweite ï¿½dern)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sIns|Del => Kamera rollen (auf Sichtachse rotieren)\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sEnter => Kamera zurck auf Ausgangspos.\n",StatusInfo.StatusString);
		sprintf(StatusInfo.StatusString,"%sESC => beenden\n",StatusInfo.StatusString);
	break;
	}
 	reDraw();
}

bool SGLsdlSpace::Go(unsigned int Mode)
{
	//Mode MUï¿½zwischen 1 und 5 liegen
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
				else {SGLprintError("Tue grad tun");break;}
			}
		}
		else return false;
	}
	return true;
}

/*
Wird aufgerufen, wenn "geklickt" wurde.
(eine Maustaste gedrckt und wieder losgelassen, ohne die Maus zu bewegen)
*/
void SGLsdlSpace::OnMouseClick(int button, int x, int y)
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
	    sprintf(StatusInfo.StatusString,"%sEntfernung zum Ziel erhï¿½t auf %.3f\n",StatusInfo.StatusString,Camera->ViewLength());
	  }break;
	}
}

/*!
    \fn SGLSpace::CallQuit()
 */
void SGLsdlSpace::CallQuit()
{
	SDL_Event user_event;
	user_event.type=SDL_QUIT;
	SDL_PushEvent(&user_event);
}

/*!
    \fn SGLSpace::OnQuit()
 */
void SGLsdlSpace::OnQuit()
{
	SDL_Quit();
	SGLprintInfo("Quit-Event raised");
}

/*!
    \fn SGLSpace::setVideoMode(int xsize, int ysize, bool FullScreen=false,bool fixedSize=false)
 */
bool SGLsdlSpace::setVideoMode(int width, int height, bool FullScreen,bool fixedSize)
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

void SGLsdlSpace::addEventListener(Uint8 eventType,SDLEventListener evl)
{
	eventListeners.insert(pair<Uint8 , SDLEventListener>(eventType,evl));
}

void SGLsdlSpace::addEventListener(Uint8 eventType,
	SGLObjBase *target_obj,
	void (*target_func)(SGLObjBase *target,SDL_Event event))
{
	SDLEventListener evl={target_obj,target_func};
	addEventListener(eventType,evl);
}

/*!
    \fn SGLSpace::callEventListeners(Uint8 type,SDL_Event event)
 */
void SGLsdlSpace::callEventListeners(Uint8 type,SDL_Event &event)
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

void SGLsdlSpace::OnMouseBtn(int button, int state, int x, int y)
{
	if(state==SDL_PRESSED)
	{	//gedrckt
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

void SGLsdlSpace::OnMouseMove( int x, int y ,unsigned int BtnDown)
{
	if(StatusInfo.Processing)return;
	else StatusInfo.Processing=true;
	MouseInfo.MovedPastDownBtn=true;
	if(MouseInfo.FollowMouse && StatusInfo.WindowWidth && StatusInfo.WindowHeight)
	{
		double RelXPos=(x-double(StatusInfo.WindowWidth)/2)/double(StatusInfo.WindowWidth)*2;
		double RelYPos=-(y-double(StatusInfo.WindowHeight)/2)/double(StatusInfo.WindowHeight)*2;

		if(BtnDown&SDL_BUTTON(1))RotateCam(RelXPos,RelYPos,Camera);
		else if(BtnDown&SDL_BUTTON(3))RotateAim(RelXPos,RelYPos,Camera);
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

bool SGLsdlSpace::FullScreen(bool FullScreen)
{
	return setVideoMode(StatusInfo.WindowWidth,StatusInfo.WindowHeight,FullScreen);
}

void SGLsdlSpace::OnDraw()
{
	resetView(0);
	callHelper(1);
	DrawExtObjs();
	callHelper(2);

	resetView(-1);
	resetView(1);
	show_status();

	printErrors();

	glFinish();
	SDL_GL_SwapBuffers( );

	StatusInfo.update=StatusInfo.Processing=false;
}

void SGLsdlSpace::setTitle(const char title[])
{
	char *dummyTitle,*icon;
	SDL_WM_GetCaption(&dummyTitle, &icon);
	SDL_WM_SetCaption(title, icon);
}

