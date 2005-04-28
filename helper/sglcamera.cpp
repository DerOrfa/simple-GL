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


/**
 * 
 * @param force 
 */
void ViewTrans::update(bool force)
{
	if(force || outDated)
	{
//	glGetIntegerv(GL_VIEWPORT,view);//@todo Diese Infos sind auﬂer beim Aufruf aus resize falsch 
		glGetDoublev(GL_MODELVIEW_MATRIX,model);
		glGetDoublev(GL_PROJECTION_MATRIX,proj);
	}
	outDated=false;
}
/**
 * 
 * @param defaultDepth 
 * @param force 
 */
void ViewTrans::update(const SGLVektor &defaultDepth,bool force)
{
	if(force || outDated)
	{
		update(force);
		depth_default=getDepth(defaultDepth);
	}
}

/**
 * 
 * @param src 
 * @param dst 
 * @return 
 */
bool ViewTrans::welt2window(const SGLVektor &src,SGLVektor &dst)
{
	if(outDated)
	{
		SGLprintWarning("Implizites Aktualisieren der Viewmatrix 0x%x aus dem Renderer, es ist nicht sicher daﬂ sie korrekt ist",this);
		update();
	}
	if(gluProject(src.SGLV_X,src.SGLV_Y,src.SGLV_Z,model,proj,view,&dst.SGLV_X,&dst.SGLV_Y,&dst.SGLV_Z))
	{
		dst.SGLV_Y=view[3]-dst.SGLV_Y;
		return true;
	}
	else{
	SGLprintError("Fehler beim Umrechnen der Coordinaten %g %g %g",src.SGLV_X,src.SGLV_Y,src.SGLV_Z);
	return false;
	}
}

/**
 * 
 * @param depthVekt 
 * @return 
 */
GLdouble ViewTrans::getDepth(const SGLVektor &depthVekt)
{
	SGLVektor dummy;
	welt2window(depthVekt,dummy);
	return dummy.SGLV_Z;
}

/**
 * 
 * @param x 
 * @param y 
 * @param depthVekt 
 * @param dst 
 * @return 
 */
bool ViewTrans::screen2welt(const unsigned int x,const unsigned int y,const SGLVektor &depthVekt,SGLVektor &dst)
{
	return screen2welt(x,y,getDepth(depthVekt),dst);
}

/**
 * 
 * @param x 
 * @param y 
 * @param dst 
 * @return 
 */
bool ViewTrans::screen2welt(const unsigned int x,const unsigned int y,SGLVektor &dst)
{
	return screen2welt(x,y,depth_default,dst);
}

bool ViewTrans::screen2welt(const unsigned int x,const unsigned int y,GLdouble depth,SGLVektor &dst)
{
	if(outDated)
	{
		SGLprintWarning("Implizites Aktualisieren der Viewmatrix 0x%x aus dem Renderer, es ist nicht sicher daﬂ sie korrekt ist",this);
		update();
	}
	if(gluUnProject(x,view[3] - int(y) ,depth,model,proj,view,&dst.SGLV_X,&dst.SGLV_Y,&dst.SGLV_Z))return true;
	else{
	SGLprintError("Fehler beim Umrechnen der Coordinaten %g %g %g",x,y,depth);
	return false;}
}

SGLBaseCam::SGLBaseCam(GLdouble PosX,GLdouble PosY,GLdouble PosZ):SGLHelper()
{
	for(int i=0;i<4;i++)
		Ecken[i].reset(new SGLVektor);
	ClipFace=1;ClipHoriz=1000;
	Pos.SGLV_X=PosX;Pos.SGLV_Y=PosY;Pos.SGLV_Z=PosZ;
	Angle=30;
	ViewFormat=1;
	ViewMatr.outDated= move_cam_with_aim=true;
	lockRoll=lockGierCam=lockKippCam=false;
	loaded=lockGierAim=lockKippAim=false;
	lockMoveZoom=lockOptZoom=false;
	ResetUpVect();
	recalcEckenMode=scaleView;
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
	ViewMatr.outDated=true;
}

void SGLBaseCam::RotateCam(GLdouble Xdeg, GLdouble Ydeg)
{
	RotateCamAround(Xdeg,Ydeg,LookAt);
}

void SGLBaseCam::RotateCamAround(GLdouble Xdeg, GLdouble Ydeg,SGLVektor &around)
{
	if(lockGierCam && lockKippCam)return;
	Pos=Pos-around;

	if(!lockGierCam)Pos=Pos.Rotate(UpVect,Xdeg);
	if(!lockKippCam)
	{
		SGLVektor HelpVect=UpVect.Rotate(Pos,90);
		Pos=Pos.Rotate(HelpVect,Ydeg);
		UpVect=UpVect.Rotate(HelpVect,Ydeg);
	}
	Pos=Pos+around;
	Compile();
	ViewMatr.outDated=true;
}

void SGLBaseCam::Roll(GLdouble degree)
{
	if(lockRoll)return;
	UpVect=UpVect.Rotate(EVektor<GLdouble>(Pos-LookAt),degree);
	Compile();
	ViewMatr.outDated=true;
}

void SGLBaseCam::MoveZoom(GLdouble fact)
{
	if(lockMoveZoom)return;
	Pos=(EVektor<GLdouble>(Pos-LookAt)*fact)+LookAt;
	Compile();
	ViewMatr.outDated=true;
}

void SGLBaseCam::OptZoom(GLdouble fact)
{
	if(lockOptZoom)return;
	Angle/=fact;
	if(Angle>179)Angle=179;
	if(Angle<1)Angle=1;
	Compile();
	ViewMatr.outDated=true;
}

double SGLBaseCam::ViewLength()
{
	SGLVektor tVek(Pos-LookAt);
	return tVek.Len();
	ViewMatr.outDated=true;
}

void SGLBaseCam::ResetView()
{
	Pos=SGLVektor(0,0,ViewLength());
	LookAt.SGLV_X=0;LookAt.SGLV_Y=0;LookAt.SGLV_Z=0;
	Angle=30;
	ResetUpVect();
	Compile();
	ViewMatr.outDated=true;
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
	ViewMatr.outDated=true;
}

/*
	Stellt den UpVect neu auf (Senkrecht auf ebene aus X/Z und Pos)
	Wenn deg angegeben ist, wird UpVect entsprechend nachtr‰glich um Pos rotiert
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
	ViewMatr.outDated=true;
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
	ViewMatr.outDated=true;
}

void SGLBaseCam::MoveCam(GLdouble xAmount, GLdouble yAmount, GLdouble zAmount)
{
	Pos   =Pos   +SGLVektor(xAmount,yAmount,zAmount);
	ReCalcUpVect();
	Compile();
	ViewMatr.outDated=true;
}

void SGLBaseCam::MoveCamTo(SGLVektor to)
{
	Pos=to;
	ReCalcUpVect();
	Compile();
	ViewMatr.outDated=true;
}

/*!
    \fn SGLCamera::getCenter()
 */
SGLVektor SGLBaseCam::getCenter()const
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
	switch(recalcEckenMode)
	{
	case resizeView:
	{
		recalcAngle((*Ecken[1]-*Ecken[2]).Len()/2);
	}break;
	case moveCam:
	{
		recalcPos((*Ecken[1]-*Ecken[2]).Len()/2);
	}
	case scaleView:
	{
		SGLVektor PosVektor=getLookVektor();
		
		double DiagWinkel=ATAN(ViewFormat);//Der Winkel der Diagonalen des Sichtfeldes zur Senkrechte
		double high=TAN(Angle/2)*PosVektor.Len(); //Hˆhe der senkrechten auf dem Horizont berechnen [tan(alpha)=a/b]
		double c = high/COS(DiagWinkel); //die Hypotenuse des Dreiecks aus Diagonalen und der Senkrechte [cos(alpha)=b/c]

		*Ecken[0]=UpVect.Rotate(PosVektor,360-DiagWinkel);
		*Ecken[1]=UpVect.Rotate(PosVektor,DiagWinkel);
		*Ecken[2]=UpVect.Rotate(PosVektor,180-DiagWinkel);
		*Ecken[3]=UpVect.Rotate(PosVektor,180+DiagWinkel);
		
		*Ecken[0]=(*Ecken[0]*c)+LookAt;
		*Ecken[1]=(*Ecken[1]*c)+LookAt;
		*Ecken[2]=(*Ecken[2]*c)+LookAt;
		*Ecken[3]=(*Ecken[3]*c)+LookAt;
	}break;
	default:
		SGLprintError("Ung¸ltiger Modus f¸r die Berechnung der Kameradaten");
		break;
	}
	recalcEckenMode=scaleView;
}

SGLVektor SGLBaseCam::getLookVektor(){return SGLVektor(Pos-LookAt);}
void SGLBaseCam::setLookVektor(SGLVektor Vekt)
{
	Pos=Vekt+LookAt;
	ViewMatr.outDated=true;
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
	ViewMatr.update(LookAt);
	
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
	Berechnet den Sichtwinkel aus der Hˆhe des Sichtfensters (in Weltkoordinaten) und dem Abstand der Cam zu LookAt neu.
	tan(alpha)=a/b
 */
void SGLBaseCam::recalcAngle(GLdouble height)
{
	Angle=ATAN(height/getLookVektor().Len())*2; // alpha=atan(a/b) (*2 weil wir den Winkel auf der mittelachse berechnet haben, brauchen aber den gesamten Winkel
}

void SGLBaseCam::recalcPos(GLdouble height)
{
	SGLVektor LookVekt=getLookVektor();
	double oldlen=LookVekt.Len();
	double newlen=height/TAN(Angle/2);//b=a/tan(alpha)
	setLookVektor(LookVekt*(newlen /oldlen));// newlen : oldlen = newvek : oldvek => newvek = (newlen : oldlen)*oldvek
}


/*!
    \fn SGLBaseCam::setView(unsigned int width,unsigned int height)
 */
void SGLBaseCam::setView(unsigned int width,unsigned int height)
{
	ViewFormat=double(width)/double(height);
	ViewMatr.view[0]=0;
	ViewMatr.view[1]=0;
	ViewMatr.view[2]=width;
	ViewMatr.view[3]=height;
	ViewMatr.outDated=true;//@todo glGetIntegerv(GL_VIEWPORT,view); liefert mist, deshalb von hand - auﬂerdem ‰ndert sich das nur, wenn das fenster ge‰ndert wird
}

unsigned int SGLBaseCam::getViewWidth()const
{
	return ViewMatr.view[2];
}

unsigned int SGLBaseCam::getViewHeight()const
{
	return ViewMatr.view[3];
}

void SGLBaseCam::confirmViewMat()
{
	if(ViewMatr.outDated)
	{
		bool do_unload=false;
		if(!loaded)
		{
			do_unload=true;
			loadView();
		}
		ViewMatr.update();
		if(do_unload)unloadView();
	}
}

//////////////////////////////////////////////////////////////////

SGLCamera::SGLCamera(GLdouble PosX,GLdouble PosY,GLdouble PosZ):SGLBaseCam(PosX,PosY,PosZ)
{showCross=true;}

void SGLCamera::generate()
{
	if(showCross)
	{
		glColor3f(1,1,1); 
		SGLVektor Vert[2],Horiz[2];
		getCross(Horiz,Vert);
		glBegin(GL_LINES);
			Vert[1].DrawPureVertex();Vert[0].DrawPureVertex();
			Horiz[1].DrawPureVertex();Horiz[0].DrawPureVertex();
		glEnd();
	}
	recalcEcken();
}

void SGLvisibleCamera::generate()
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

	glBegin(GL_LINE_LOOP);
		Ecken[0]->DrawPureVertex();
		Ecken[1]->DrawPureVertex();
		Ecken[2]->DrawPureVertex();
		Ecken[3]->DrawPureVertex();
	glEnd();
	glBegin(GL_LINES);
		Pos.DrawPureVertex();Ecken[0]->DrawPureVertex();
		Pos.DrawPureVertex();Ecken[1]->DrawPureVertex();
		Pos.DrawPureVertex();Ecken[2]->DrawPureVertex();
		Pos.DrawPureVertex();Ecken[3]->DrawPureVertex();
	glEnd();
}
