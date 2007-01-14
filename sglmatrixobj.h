//
// C++ Interface: sglmatrixobj
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLMATRIXOBJ_H
#define SGLMATRIXOBJ_H

#ifdef WIN32
	#include <windows.h>	// Header File For Windows
	#include <gl\gl.h>		// Header File For The OpenGL32 Library
	#include <gl\glu.h>		// Header File For The GLu32 Library
	#include <gl\glaux.h>	// Header File For The GLaux Library
#else
	#include <GL/gl.h>
#endif

#include "sglvektor.h"

/**
@author Enrico Reimer,,,
*/
class SGLMatrixObj
{
public:
	SGLMatrixObj(GLenum type);
	void Scale(GLdouble xfact,GLdouble yfact, GLdouble zfact);
	void Scale(GLdouble fact);
	void ScaleTo(GLdouble fact);
	void Move(SGLVektor to);
	void Move(GLdouble x,GLdouble y, GLdouble z);
	void MoveTo(SGLVektor to);
	void MoveTo(GLdouble x,GLdouble y, GLdouble z);
	void Rotate(GLdouble xfact,GLdouble yfact, GLdouble zfact, GLdouble amount);
	void ResetTransformMatrix(const GLdouble *newMatrix=NULL);
	void loadMatrix();
	void unloadMatrix();
	void saveMatrix(GLenum type,GLdouble dst[16]=NULL);
	void saveAktMatrix(GLdouble dst[16]=NULL);
	virtual const char* guesType();
	virtual ~SGLMatrixObj();
protected:
	GLdouble MyTransformMatrix[16];
	GLenum MatrMode;
};

#endif
