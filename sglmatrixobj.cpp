//
// C++ Implementation: sglmatrixobj
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sglmatrixobj.h"
#include "sglmisc.h"
#include <GL/glu.h>

SGLMatrixObj::SGLMatrixObj(GLenum type)
{
	MatrMode= type;
}



void SGLMatrixObj::MoveTo(SGLVektor to){MoveTo(to.SGLV_X,to.SGLV_Y,to.SGLV_Z);}
void SGLMatrixObj::Move(SGLVektor to){Move(to.SGLV_X,to.SGLV_Y,to.SGLV_Z);}

void SGLMatrixObj::MoveTo(GLdouble x,GLdouble y, GLdouble z)
{
	ResetTransformMatrix(NULL);
	Move(x,y,z);
}

void SGLMatrixObj::Move(GLdouble x,GLdouble y, GLdouble z)
{
	loadMatrix();
	glTranslated(x,y,z);
	saveMatrix(MatrMode);
	unloadMatrix();
}

void SGLMatrixObj::Scale(GLdouble xfact,GLdouble yfact, GLdouble zfact)
{
	loadMatrix();
	glScaled(xfact,yfact,zfact);
	saveMatrix(MatrMode);
	unloadMatrix();
}
void SGLMatrixObj::Rotate(GLdouble xfact,GLdouble yfact, GLdouble zfact, GLdouble amount)
{
	loadMatrix();
	glRotated(amount,xfact,yfact,zfact);
	saveMatrix(MatrMode);
	unloadMatrix();
}

void SGLMatrixObj::ResetTransformMatrix(const GLdouble *newMatrix)
{
	if(newMatrix)memcpy(MyTransformMatrix,newMatrix,sizeof(GLdouble)*16);
	else for(int i=0;i<16;i++)
	{
		if(i%5)MyTransformMatrix[i]=0;
		else MyTransformMatrix[i]=1;
	}
}

void SGLMatrixObj::Scale(GLdouble fact){Scale(fact,fact,fact);}
void SGLMatrixObj::ScaleTo(GLdouble fact)
{
	ResetTransformMatrix(NULL);
	Scale(fact);
}


/*!
    \fn SGLMatrixObj::loadMatrix()
 */
void SGLMatrixObj::loadMatrix()
{
	glMatrixMode(MatrMode);
	glPushMatrix();
	glMultMatrixd(MyTransformMatrix);
}

/*!
    \fn SGLMatrixObj::unloadMatrix()
 */
void SGLMatrixObj::unloadMatrix()
{
	glMatrixMode(MatrMode);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


/*!
    \fn SGLMatrixObj::saveAktMatrix()
 */
void SGLMatrixObj::saveAktMatrix(GLdouble dst[16])
{
	if(dst==0)dst=MyTransformMatrix;
	saveMatrix(sglGeti(GL_MATRIX_MODE),dst);
}
void SGLMatrixObj::saveMatrix(GLenum mode,GLdouble dst[16])
{
#define CASE_SET(CASE)	case CASE:type=CASE ## _MATRIX
	GLenum type;
	if(dst==0)dst=MyTransformMatrix;
	switch(mode)
	{
		CASE_SET(GL_MODELVIEW);break;
		CASE_SET(GL_PROJECTION);break;
		CASE_SET(GL_TEXTURE);break;
		CASE_SET(GL_COLOR);break;
		default:SGLprintError("Unbekannter MatrixMode");type=0;break;
	}
	if(type)glGetDoublev(type,dst);
#undef CASE_SET
}

/*!
    \fn SGLObjBase::guesType()
 */
const char*  SGLMatrixObj::guesType()
{
#ifndef WIN32
	return typeid(*this).name();
#else
	return "VC60 kann kein typeid??";
#endif
}

