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

#ifdef WIN32
	#include <windows.h>								// Header File For Windows
	#include <gl\gl.h>								// Header File For The OpenGL32 Library
	#include <gl\glu.h>								// Header File For The GLu32 Library
	#include <gl\glaux.h>								// Header File For The GLaux Library
#else
	#include <GL/gl.h>
#endif

#include "sglvektor.h"
#include <list>

class SGLObjList;

/**
@author Enrico Reimer
*/
class SGLObjBase{
public:
	enum Prio{floor=INT_MIN,under=-10,std=0,flstd=10,ontop=INT_MAX};
	void Scale(GLdouble fact);
	SGLObjBase(const SGLObjBase &src);
	SGLObjBase(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGLObjBase();
	virtual GLuint Compile()=0;
	virtual void generate()=0;

	static SGLVektor Normale(SGLVektor Vekt1,SGLVektor Vekt2);
	static SGLVektor Normale(SGLVektor Pkt1,SGLVektor Pkt2,SGLVektor Pkt3);

	void Scale(GLdouble xfact,GLdouble yfact, GLdouble zfact);
	void Move(SGLVektor to);
	void Move(GLdouble x,GLdouble y, GLdouble z);
	void MoveTo(SGLVektor to);
	void MoveTo(GLdouble x,GLdouble y, GLdouble z);
	void Rotate(GLdouble xfact,GLdouble yfact, GLdouble zfact, GLdouble amount);

	void ResetTransformMatrix(const GLdouble *newMatrix=NULL);

	GLdouble MyTransformMatrix[16];
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
	virtual const char* guesType();
	short priority;
	short should_compile;
	SGLObjList* myList;
protected:
	list<SGLObjBase*> changeReciver,changeSender;
};

#include "helper/sgldisplist.h"
#endif
