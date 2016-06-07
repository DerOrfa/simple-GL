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

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#ifdef WIN32
		#include <windows.h>
	#endif
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

	void Move(SGLVektor to);
	void Move(GLdouble x,GLdouble y, GLdouble z);

	void SetPosAndScale(GLdouble x,GLdouble y, GLdouble z,GLdouble fact);
	void Rotate(GLdouble xfact,GLdouble yfact, GLdouble zfact, GLdouble amount);
	void ResetTransformMatrix(const GLdouble *newMatrix=NULL);
	const GLdouble* getTransformMatrix()const;
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
