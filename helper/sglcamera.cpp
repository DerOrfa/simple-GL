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
#include <GL/glu.h>

SGLBaseCam::SGLBaseCam(GLdouble PosX,GLdouble PosY,GLdouble PosZ):SGLHelper()
{
	ClipFace=1;ClipHoriz=500;
	Pos.SGLV_X=PosX;Pos.SGLV_Y=PosY;Pos.SGLV_Z=PosZ;
	Angle=30;
	ViewFormat=1;
	move_cam_with_aim=true;
	lockRoll=lockGierCam=lockKippCam=false;
	loaded=lockGierAim=lockKippAim=false;
	lockMoveZoom=lockOptZoom=false;
	ResetUpVect();
	dontTouchEcken=0;
}
	
void SGLBaseCam::RotateAim(GLdouble Xdeg,GLdouble Ydeg)
{
	if(lockGierAim && lockKippAim)return;
	LookAt=LookAt-Pos;

	if(!lockGierAim)LookAt=LookAt.Rotate(UpVect,Xdeg);
	if(!lockKippAim)
	{
		SGLVektor HelpVect=UpVect.Rotate(LookAt,90);
		LookAt=LookAt.Rotate(HelpVect,Ydeg);
		UpVect=UpVect.Rotate(HelpVect,Ydeg);
	}

	LookAt=LookAt+Pos;

	Compile();
}

void SGLBaseCam::RotateCam(GLdouble Xdeg, GLdouble Ydeg)
{
	if(lockGierCam && lockKippCam)return;
	Pos=Pos-LookAt;

	if(!lockGierCam)Pos=Pos.Rotate(UpVect,Xdeg);
	if(!lockKippCam)
	{
		SGLVektor HelpVect=UpVect.Rotate(Pos,90);
		Pos=Pos.Rotate(HelpVect,Ydeg);
		UpVect=UpVect.Rotate(HelpVect,Ydeg);
	}
	Pos=Pos+LookAt;
	Compile();
}

void SGLBaseCam::Roll(GLdouble degree)
{
	if(lockRoll)return;
	UpVect=UpVect.Rotate(EVektor<GLdouble>(Pos-LookAt),degree);
	Compile();
}

void SGLBaseCam::MoveZoom(GLdouble fact)
{
	if(lockMoveZoom)return;
	Pos=(EVektor<GLdouble>(Pos-LookAt)*fact)+LookAt;
	Compile();
}
void SGLBaseCam::OptZoom(GLdouble fact)
{
	if(lockOptZoom)return;
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
}

/*
	Stellt den UpVekt basierend auf aktuellem UpVekt neu auf.
	
	Bildet Senkrechte auf Ebene von aktuellem UpVekt und dem Vektor vom Cam zum Ziel.
	Rotiert den Vektor vom Cam zum Ziel um diese Senkrechte um 90 Grad, und Normalisiert das Erg.
*/

void SGLBaseCam::ReCalcUpVect(bool PosIsOnNull,double rotdeg)
{
	if(!PosIsOnNull)Pos=Pos-LookAt;
	SGLVektor HelpVekt(Pos.kreuzprod(UpVect));
	UpVect=Pos.Rotate(HelpVekt,90);
	if(rotdeg)UpVect=UpVect.Rotate(Pos,rotdeg);
	UpVect.Normalize();
	if(!PosIsOnNull)Pos=Pos+LookAt;
}

/*
	Stellt den UpVect neu auf (Senkrecht auf ebene aus X/Z und Pos)
	Wenn deg angegeben ist, wird UpVect entsprechend nachträglich um Pos rotiert
*/
void SGLBaseCam::ResetUpVect(double rotdeg)
{
	UpVect=SGLVektor(0,1);
	ReCalcUpVect(false,rotdeg);
}

void SGLBaseCam::MoveAimTo(SGLVektor to)
{
	SGLVektor newAim=to;
	SGLVektor amount=newAim-LookAt;
	LookAt=newAim;
	if(move_cam_with_aim)MoveCam(amount.SGLV_X, amount.SGLV_Y, amount.SGLV_Z);
	else
	{
		ReCalcUpVect();
		Compile();
	}
}

void SGLBaseCam::MoveAim(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	LookAt=LookAt+SGLVektor(xAmount,yAmount,zAmount);
	if(move_cam_with_aim)MoveCam(xAmount, yAmount, zAmount);
	else 
	{
		ReCalcUpVect();	
		Compile();
	}
}

void SGLBaseCam::MoveCam(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	Pos   =Pos   +SGLVektor(xAmount,yAmount,zAmount);
	ReCalcUpVect();
	Compile();
}

void SGLBaseCam::MoveCamTo(SGLVektor to)
{
	Pos=to;
	ReCalcUpVect();
	Compile();
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
    \fn SGLBaseCam::recalcEcken(SGLVektor Ecken[4])
 */
void SGLBaseCam::recalcEcken()
{
	if(dontTouchEcken>0)
	{
		recalcAngle((Ecken[1]-Ecken[2]).Len()/2);
		dontTouchEcken--;
	}
	else
	{
		SGLVektor PosVektor=getLookVektor();
		
		double DiagWinkel=ATAN(ViewFormat);//Der Winkel der Diagonalen des Sichtfeldes zur Senkrechte
		double c = SIN(Angle)*PosVektor.Len()/COS(DiagWinkel); //die Hypotenuse des Dreiecks aus Diagonalen und der Senkrechte

		Ecken[0]=UpVect.Rotate(PosVektor,360-DiagWinkel);
		Ecken[1]=UpVect.Rotate(PosVektor,DiagWinkel);
		Ecken[2]=UpVect.Rotate(PosVektor,180-DiagWinkel);
		Ecken[3]=UpVect.Rotate(PosVektor,180+DiagWinkel);
		
		Ecken[0]=(Ecken[0]*c)+LookAt;
		Ecken[1]=(Ecken[1]*c)+LookAt;
		Ecken[2]=(Ecken[2]*c)+LookAt;
		Ecken[3]=(Ecken[3]*c)+LookAt;
	}
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
	glColor3f(1,1,1); 
	if(showCross)
	{
		SGLVektor Vert[2],Horiz[2];
		getCross(Horiz,Vert);
		glBegin(GL_LINES);
			Vert[1].DrawPureVertex();Vert[0].DrawPureVertex();
			Horiz[1].DrawPureVertex();Horiz[0].DrawPureVertex();
		glEnd();
	}
	
	recalcEcken();

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

void SGLBaseCam::loadView()
{
	if(loaded){SGLprintWarning("Die Camerasicht ist schon geladen");}
	loaded=true;
	GLenum oldmode=sglGeti(GL_MATRIX_MODE);
	glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();//neue Projektionsmatrix laden
	gluPerspective(Angle,ViewFormat,ClipFace,ClipHoriz);
	
	glMatrixMode(GL_MODELVIEW);glPushMatrix();//neue Modelviewmatrix laden
	gluLookAt(Pos.SGLV_X,Pos.SGLV_Y,Pos.SGLV_Z,
		  LookAt.SGLV_X,LookAt.SGLV_Y,LookAt.SGLV_Z,
		  UpVect.SGLV_X,UpVect.SGLV_Y,UpVect.SGLV_Z);
	glMatrixMode(oldmode);
}

void SGLBaseCam::unloadView()
{
	if(!loaded){SGLprintWarning("Die Camerasicht ist nicht geladen");}
	GLenum oldmode=sglGeti(GL_MATRIX_MODE);
	glPopMatrix();//alte Modelviewmatrix wiederherstellen
	glMatrixMode(GL_PROJECTION);glPopMatrix();//alte Projektionsmatrix wiederherstellen
	loaded=false;
	glMatrixMode(oldmode);
}


/*!
    \fn SGLBaseCam::recalcAngle()
	Berechnet den Sichtwinkel aus der Höhe des Sichtfensters (in Weltkoordinaten) und dem Abstand der Cam zu LookAt neu.
 */
void SGLBaseCam::recalcAngle(GLdouble height)
{
	Angle=ATAN(height/getLookVektor().Len())*2; // atan(a/b)
}
