/***************************************************************************
                          sglvektor.h  -  description
                             -------------------
    begin                : Sun Sep 23 2001
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

#ifndef SGLVEKTOR_H
#define SGLVEKTOR_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/array.hpp>
#include "sglmisc.h"
#include "sglshptr.h"

#define	SGLV_X	operator[](0)
#define	SGLV_Y	operator[](1)
#define	SGLV_Z	operator[](2)

#define	SGLV_R	Color[0]
#define	SGLV_G	Color[1]
#define	SGLV_B	Color[2]

/**
 * Auf EVektor\<GLdouble\> basierender Positionsvektor.
 * Dieser Vektor bietet einige zusätzliche Operationen zum Zeichnen und zur Transformation.
  *@author Enrico Reimer
  */
typedef boost::numeric::ublas::vector<GLdouble> dvector;
typedef boost::numeric::ublas::matrix<GLdouble,boost::numeric::ublas::column_major> dmatrix;

class SGLVektor : public dvector
{
public:
	SGLVektor(GLdouble Array[3]);
	SGLVektor(GLfloat Array[3]);
	SGLVektor(GLdouble X=0,GLdouble Y=0,GLdouble Z=0);
	SGLVektor(const dvector &src);
	SGLVektor(const SGLVektor &src);

	SGLVektor Rotate(GLdouble Yrot, GLdouble Xrot, GLdouble Zrot);
	SGLVektor Rotate(dvector VektPtr, GLdouble Xrot);
	GLdouble Xwink();
	GLdouble Ywink();
	SGLVektor operator =(const dvector &VektPtr);

	void	DrawVertex(SGLVektor* Normale);
	void	DrawVertex();
	inline	void	DrawPureVertex()const{glVertex3d(SGLV_X,SGLV_Y,SGLV_Z);}
	
	inline void	SetNormale()const{glNormal3d(SGLV_X,SGLV_Y,SGLV_Z);}

	void	SetColor(GLdouble R=-1,GLdouble G=-1,GLdouble B=-1);
	void	SetColor(unsigned char R,unsigned char G,unsigned char B);
	void	SetColor(int R=-1,int G=-1,int B=-1);
	GLdouble Color[3];
	vector<GLfloat> texKoord;
	GLdouble toWink(GLdouble &ebene_wink,GLdouble &steigung_wink);
    void translateTo();
    void DrawVektor(SGLVektor from=SGLVektor(0,0,0));
    void DrawVektorAt(SGLVektor At);
	void DrawPkt(double size=.1);
	GLdouble spatprod(const dvector &b,const dvector &c)const;
	dvector cross_prod(const dvector &b);
};

typedef SGLshPtr<SGLVektor> VektorPtr;
typedef std::vector<VektorPtr> VektorList;

#define SET_VEKTPTR_VAL(VEKT,NEW_VAL)									\
if(VEKT.use_count() > 1)												\
{}	\
if(VEKT)*VEKT=NEW_VAL;													\
else VEKT=VektorPtr(new SGLVektor(NEW_VAL));

#define TOUCH_VEKTPTR(PTR)	if(!PTR)PTR.reset(new SGLVektor)

#define COS(X)   cos( (X) * M_PI/180.0 )
#define SIN(X)   sin( (X) * M_PI/180.0 )
#define TAN(X)   tan( (X) * M_PI/180.0 )

#define ACOS(X)   (acos(X) * 180.0/M_PI )
#define ASIN(X)   (asin(X) * 180.0/M_PI )
#define ATAN(X)   (atan(X) * 180.0/M_PI )


#endif
