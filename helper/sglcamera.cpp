/***************************************************************************
                          eglcamera.cpp  -  description
                             -------------------
    begin                : Mon Sep 17 2001
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


#include "sglcamera.h"
#include "../sglmisc.h"

SGLCamera::SGLCamera(GLdouble PosX,GLdouble PosY,GLdouble PosZ):SGLHelper()
{
	ClipFace=1;ClipHoriz=500;
	Pos.SGLV_X=PosX;Pos.SGLV_Y=PosY;Pos.SGLV_Z=PosZ;
	Angle=30;
	ViewFormat=1;
	ResetUpVect();
}
SGLCamera::~SGLCamera()
{}

void SGLCamera::RotateAim(GLdouble Xdeg,GLdouble Ydeg)
{
	LookAt=LookAt-Pos;

	LookAt=LookAt.Rotate(UpVect,Xdeg);
	SGLVektor HelpVect=UpVect.Rotate(LookAt,90);
	LookAt=LookAt.Rotate(HelpVect,Ydeg);
	UpVect=UpVect.Rotate(HelpVect,Ydeg);

	LookAt=LookAt+Pos;

	Compile();
	notifyChange();
}

void SGLCamera::RotateCam(GLdouble Xdeg, GLdouble Ydeg)
{
	Pos=Pos-LookAt;

	Pos=Pos.Rotate(UpVect,Xdeg);
	SGLVektor HelpVect=UpVect.Rotate(Pos,90);
	Pos=Pos.Rotate(HelpVect,Ydeg);
	UpVect=UpVect.Rotate(HelpVect,Ydeg);

	Pos=Pos+LookAt;
	Compile();
	notifyChange();
}

void SGLCamera::Roll(GLdouble degree)
{
	UpVect=UpVect.Rotate(EVektor<GLdouble>(Pos-LookAt),degree);
	Compile();
	notifyChange();
}

void SGLCamera::MoveZoom(GLdouble fact)
{
	Pos=(EVektor<GLdouble>(Pos-LookAt)*fact)+LookAt;
	Compile();
	notifyChange();
}
void SGLCamera::OptZoom(GLdouble fact)
{
	Angle/=fact;
	if(Angle>179)Angle=179;
	if(Angle<1)Angle=1;
	Compile();
}

double SGLCamera::ViewLength()
{
	SGLVektor tVek(Pos-LookAt);
	return tVek.Len();
}
void SGLCamera::ResetView()
{
	Pos=SGLVektor(0,0,ViewLength());
	LookAt.SGLV_X=0;LookAt.SGLV_Y=0;LookAt.SGLV_Z=0;
	Angle=30;
	ResetUpVect();
	Compile();
	notifyChange();
}

void SGLCamera::generate()
{
	SGLVektor PosVektor(SGLVektor(Pos-LookAt));

	double gamma = 90-Angle;
	double a=(SIN(Angle)/SIN(gamma))*PosVektor.Len();
	double DiagWinkel=ATAN(ViewFormat);

	SGLVektor Diag2Vekt=UpVect.Rotate(PosVektor,DiagWinkel);

	SGLVektor Diag3Vekt=UpVect.Rotate(PosVektor,180-DiagWinkel);
	SGLVektor Diag4Vekt=UpVect.Rotate(PosVektor,180+DiagWinkel);
	SGLVektor Diag1Vekt=UpVect.Rotate(PosVektor,360-DiagWinkel);

	SGLVektor VertVekt=UpVect.Rotate(PosVektor,-90);
	SGLVektor HorizVekt=UpVect;

	SGLVektor MinusVertVekt=(SGLVektor()-VertVekt);
	SGLVektor MinusHorizVekt=(SGLVektor()-HorizVekt);


	Diag1Vekt=(Diag1Vekt*a)+LookAt;
	Diag2Vekt=(Diag2Vekt*a)+LookAt;
	Diag3Vekt=(Diag3Vekt*a)+LookAt;
	Diag4Vekt=(Diag4Vekt*a)+LookAt;

	VertVekt=VertVekt+LookAt;
	MinusVertVekt=MinusVertVekt+LookAt;

	HorizVekt=HorizVekt+LookAt;
	MinusHorizVekt=MinusHorizVekt+LookAt;

//	glColor3f(1,1,1);
	int error;
	glBegin(GL_LINE_LOOP);
		Diag1Vekt.DrawVertex();
		Diag2Vekt.DrawVertex();
		Diag3Vekt.DrawVertex();
		Diag4Vekt.DrawVertex();
	glEnd();
	glBegin(GL_LINES);
		MinusVertVekt.DrawVertex();VertVekt.DrawVertex();
		MinusHorizVekt.DrawVertex();HorizVekt.DrawVertex();
		Pos.DrawVertex();Diag1Vekt.DrawVertex();
		Pos.DrawVertex();Diag2Vekt.DrawVertex();
		Pos.DrawVertex();Diag3Vekt.DrawVertex();
		Pos.DrawVertex();Diag4Vekt.DrawVertex();
	glEnd();

}

void SGLCamera::ReCalcUpVect(bool PosIsOnNull)
{
	if(!PosIsOnNull)Pos=Pos-LookAt;
	SGLVektor HelpVekt(Pos.kreuzprod(UpVect));
	UpVect=Pos.Rotate(HelpVekt,90);
	UpVect.Normalize();
	if(!PosIsOnNull)Pos=Pos+LookAt;
}

void SGLCamera::ResetUpVect()
{
	UpVect=SGLVektor(0,1);
	ReCalcUpVect();
}

void SGLCamera::MoveAim(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	LookAt=LookAt-SGLVektor(xAmount,yAmount,zAmount);
	ReCalcUpVect();
}

void SGLCamera::MoveCam(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	Pos   =Pos   -SGLVektor(xAmount,yAmount,zAmount);
	ReCalcUpVect();
}

/*!
    \fn SGLCamera::getCenter()
 */
SGLVektor SGLCamera::getCenter()
{
	return Pos;
}

