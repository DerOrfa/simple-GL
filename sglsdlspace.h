//
// C++ Interface: sglsdlspace
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLSDLSPACE_H
#define SGLSDLSPACE_H

#include <sglspace.h>

/**
@author Enrico Reimer,,,
*/
class SGLsdlSpace : public SGLSpace
{
public:
	multimap<Uint8, SDLEventListener> eventListeners;
	void addEventListener(Uint8 eventType,SDLEventListener evl);
	void addEventListener(Uint8 eventType,SGLObjBase *target_obj,void (*target_func)(SGLObjBase *target,SDL_Event event));
	void callEventListeners(Uint8 type,SDL_Event &event);
    SGLsdlSpace(int XSize=800, int YSize=600,unsigned int R=0,unsigned int G=0,unsigned int B=0);
	virtual void OnKey(SDL_keysym k, bool down);
	void procEvent(SDL_Event e);
	virtual bool setup_video(int w,int h);
	void OnMouseClick(int button, int x, int y);
	virtual void OnMouseMove( int x, int y, unsigned int BtnDown );
	virtual void OnMouseBtn(int button, int state, int x, int y);
	virtual void OnQuit();
	virtual void OnDraw();
	void CallQuit();
	bool setVideoMode(int xsize=800, int ysize=600, bool FullScreen=false,bool fixedSize=false);
	bool Go(unsigned int Mode=1);
	bool FullScreen(bool FullScreen=true);
	void setTitle(const char title[]);
};

#endif
