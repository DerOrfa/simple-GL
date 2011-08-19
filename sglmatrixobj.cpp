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
#include <string.h>
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

SGLMatrixObj::SGLMatrixObj(GLenum type)
{
	MatrMode= type;
}

void SGLMatrixObj::SetPosAndScale(GLdouble x, GLdouble y, GLdouble z, GLdouble fact)
{
	ResetTransformMatrix(NULL);
	MyTransformMatrix[12]=x;
	MyTransformMatrix[13]=y;
	MyTransformMatrix[14]=z;
	MyTransformMatrix[0]=MyTransformMatrix[5]=MyTransformMatrix[10]=fact;
}


void SGLMatrixObj::Move(SGLVektor to){Move(to.SGLV_X,to.SGLV_Y,to.SGLV_Z);}
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
	return typeid(*this).name();
}

SGLMatrixObj::~SGLMatrixObj(){}
