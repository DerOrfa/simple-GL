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

SGLBaseCam::SGLBaseCam(GLdouble PosX,GLdouble PosY,GLdouble PosZ):SGLHelper()
{
	ClipFace=1;ClipHoriz=500;
	Pos.SGLV_X=PosX;Pos.SGLV_Y=PosY;Pos.SGLV_Z=PosZ;
	Angle=30;
	ViewFormat=1;
	ResetUpVect();
}

void SGLBaseCam::RotateAim(GLdouble Xdeg,GLdouble Ydeg)
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

void SGLBaseCam::RotateCam(GLdouble Xdeg, GLdouble Ydeg)
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

void SGLBaseCam::Roll(GLdouble degree)
{
	UpVect=UpVect.Rotate(EVektor<GLdouble>(Pos-LookAt),degree);
	Compile();
	notifyChange();
}

void SGLBaseCam::MoveZoom(GLdouble fact)
{
	Pos=(EVektor<GLdouble>(Pos-LookAt)*fact)+LookAt;
	Compile();
	notifyChange();
}
void SGLBaseCam::OptZoom(GLdouble fact)
{
	Angle/=fact;
	if(Angle>179)Angle=179;
	if(Angle<1)Angle=1;
	Compile();
}

double SGLBaseCam::ViewLength()
{
	SGLVektor tVek(Pos-LookAt);
	return tVek.Len();
}
void SGLBaseCam::ResetView()
{
	Pos=SGLVektor(0,0,ViewLength());
	LookAt.SGLV_X=0;LookAt.SGLV_Y=0;LookAt.SGLV_Z=0;
	Angle=30;
	ResetUpVect();
	Compile();
	notifyChange();
}

void SGLBaseCam::ReCalcUpVect(bool PosIsOnNull)
{
	if(!PosIsOnNull)Pos=Pos-LookAt;
	SGLVektor HelpVekt(Pos.kreuzprod(UpVect));
	UpVect=Pos.Rotate(HelpVekt,90);
	UpVect.Normalize();
	if(!PosIsOnNull)Pos=Pos+LookAt;
}

void SGLBaseCam::ResetUpVect()
{
	UpVect=SGLVektor(0,1);
	ReCalcUpVect();
}

void SGLBaseCam::MoveAim(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	LookAt=LookAt-SGLVektor(xAmount,yAmount,zAmount);
	ReCalcUpVect();
}

void SGLBaseCam::MoveCam(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	Pos   =Pos   -SGLVektor(xAmount,yAmount,zAmount);
	ReCalcUpVect();
}

/*!
    \fn SGLCamera::getCenter()
 */
SGLVektor SGLBaseCam::getCenter()
{
	return Pos;
}
void SGLBaseCam::getCross(SGLVektor Horiz[2],SGLVektor Vert[2])
{
	SGLVektor PosVektor=getLookVektor();
	Vert[0]=UpVect.Rotate(PosVektor,-90);
	Horiz[0]=UpVect;

	Vert[1]=(SGLVektor()-Vert[0])+LookAt;
	Horiz[1]=(SGLVektor()-Horiz[0])+LookAt;

	Vert[0]=Vert[0]+LookAt;
	Horiz[0]=Horiz[0]+LookAt;
}

/*!
    \fn SGLBaseCam::getViewRect(SGLVektor Ecken[4])
 */
void SGLBaseCam::getViewRect(SGLVektor Ecken[4])
{
	SGLVektor PosVektor=getLookVektor();

	double gamma = 90-Angle;
	double a=(SIN(Angle)/SIN(gamma))*PosVektor.Len();
	double DiagWinkel=ATAN(ViewFormat);

	Ecken[0]=UpVect.Rotate(PosVektor,360-DiagWinkel);
	Ecken[1]=UpVect.Rotate(PosVektor,DiagWinkel);
	Ecken[2]=UpVect.Rotate(PosVektor,180-DiagWinkel);
	Ecken[3]=UpVect.Rotate(PosVektor,180+DiagWinkel);
	Ecken[0]=(Ecken[0]*a)+LookAt;
	Ecken[1]=(Ecken[1]*a)+LookAt;
	Ecken[2]=(Ecken[2]*a)+LookAt;
	Ecken[3]=(Ecken[3]*a)+LookAt;
}

SGLVektor SGLBaseCam::getLookVektor()
{
	return SGLVektor(Pos-LookAt);
}

SGLCamera::SGLCamera(GLdouble PosX,GLdouble PosY,GLdouble PosZ):SGLBaseCam(PosX,PosY,PosZ)
{
	showCross=true;
}
void SGLCamera::generate()
{
	if(showCross)
	{
		SGLVektor Vert[2],Horiz[2];
		getCross(Horiz,Vert);
		glBegin(GL_LINES);
			Vert[1].DrawPureVertex();Vert[0].DrawPureVertex();
			Horiz[1].DrawPureVertex();Horiz[0].DrawPureVertex();
		glEnd();
	}
	
	SGLVektor Ecken[4];
	
	getViewRect(Ecken);

	glColor3f(1,1,1); 
	int error;
	glBegin(GL_LINE_LOOP);
		Ecken[0].DrawPureVertex();
		Ecken[1].DrawPureVertex();
		Ecken[2].DrawPureVertex();
		Ecken[3].DrawPureVertex();
	glEnd();
	glBegin(GL_LINES);
		Pos.DrawPureVertex();Ecken[0].DrawPureVertex();
		Pos.DrawPureVertex();Ecken[1].DrawPureVertex();
		Pos.DrawPureVertex();Ecken[2].DrawPureVertex();
		Pos.DrawPureVertex();Ecken[3].DrawPureVertex();
	glEnd();

}
