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

#ifdef WIN32
	#include <windows.h>								// Header File For Windows
#endif

#include <evektor.h>
#include "sglmisc.h"

//#include "util/sglmaterial.h"

#define	SGLV_X	operator[](0)
#define	SGLV_Y	operator[](1)
#define	SGLV_Z	operator[](2)

#define	SGLV_R	Color[0]
#define	SGLV_G	Color[1]
#define	SGLV_B	Color[2]

/**
  *@author Enrico Reimer
  */

using namespace emath;

class SGLVektor : public EVektor<GLdouble>
{
public:
	SGLVektor(GLdouble Array[3]);
	SGLVektor(GLfloat Array[3]);
	SGLVektor(GLdouble X=0,GLdouble Y=0,GLdouble Z=0);
	SGLVektor(const EVektor<GLdouble> &src);
	SGLVektor(const SGLVektor &src);
	~SGLVektor();
	SGLVektor Rotate(GLdouble Yrot, GLdouble Xrot, GLdouble Zrot);
	SGLVektor Rotate(SGLVektor VektPtr, GLdouble Xrot);
	GLdouble Xwink();
	GLdouble Ywink();
	SGLVektor operator =(EVektor<GLdouble> &VektPtr);

	void	DrawVertex(SGLVektor* Normale);
	void	DrawVertex();
	inline	void	DrawPureVertex(){glVertex3d(SGLV_X,SGLV_Y,SGLV_Z);}
	
	inline void	SetNormale(){glNormal3d(SGLV_X,SGLV_Y,SGLV_Z);}

	void	SetColor(GLdouble R=-1,GLdouble G=-1,GLdouble B=-1);
	void	SetColor(unsigned char R,unsigned char G,unsigned char B);
	void	SetColor(int R=-1,int G=-1,int B=-1);
	GLdouble Color[3];
	EVektor<GLfloat> texKoord;
	GLdouble toWink(GLdouble &ebene_wink,GLdouble &steigung_wink);
    void translateTo();
    void DrawVektor(SGLVektor from=SGLVektor(0,0,0));
    void DrawVektorAt(SGLVektor At);
	void DrawPkt(double size=.1);
};

#endif
