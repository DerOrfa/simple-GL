/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLOBJBASE_H
#define SGLOBJBASE_H

#include "sglmatrixobj.h"
#include <list>

#include <SDL.h>

class SGLObjList;

/**
@author Enrico Reimer
*/

template<class T> struct CompObj : public unary_function<T&, void>
{
	CompObj(vector<GLint> *targetList) {Objs=targetList;}
	void operator() (T &x) 
	{
		Objs->push_back(x.Compile()); 
	}
	vector<GLint> *Objs;
};

class SGLObjBase:public SGLMatrixObj
{
public:
	enum Prio{floor=INT_MIN,under=-10,std=0,flstd=10,ontop=INT_MAX};
	SGLObjBase(const SGLObjBase &src);
	SGLObjBase(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGLObjBase();
	virtual GLuint Compile()=0;
	virtual void generate()=0;

	static SGLVektor Normale(SGLVektor Vekt1,SGLVektor Vekt2);
	static SGLVektor Normale(SGLVektor Pkt1,SGLVektor Pkt2,SGLVektor Pkt3);

	GLuint ID;
	bool IgnoreClip,IgnoreLight;
	GLenum FrontFace;
	SGLVektor* FaceAt;
	void metaGenerate();

	virtual SGLVektor getCenter() = 0;

	SGLVektor getCenterInSpace();
	SGLVektor getMyPos();
	GLuint metaCompile(bool force_compile=false);
	void compileNextTime();
	void link(SGLObjBase *obj);
	void unlink(SGLObjBase *obj);
	void notifyChange();
	short priority;
	short should_compile;
	SGLObjList* myList;
protected:
	list<SGLObjBase*> changeReciver,changeSender;
};

#include "helper/sgldisplist.h"

struct SDLEventListener
{
	SGLObjBase *target;
	void (*target_func)(SGLObjBase *target,SDL_Event event);
};
#endif
