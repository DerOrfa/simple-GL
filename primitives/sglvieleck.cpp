/***************************************************************************
                          sglvieleck.cpp  -  description
                             -------------------
    begin                : Tue Oct 2 2001
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

#include "sglvieleck.h"
#include "../sglmisc.h"

/*!
    \fn SGLPolygon::SGLPolygon()
 */
SGLPolygon::SGLPolygon()
{
	useCenter=false;
	twoSideRender=true;
}
/*!
    \fn SGLPolygon::SGLPolygon(const SGLPolygon &PolygonPtr)
 */
SGLPolygon::SGLPolygon(const SGLPolygon &PolygonPtr):SGLFlObj(PolygonPtr)
{
	MyVekt=PolygonPtr.MyVekt;
	EckVektoren.Cnt=PolygonPtr.EckVektoren.Cnt;
	for(int i=0;i<PolygonPtr.EckVektoren.Cnt;i++)
	{
		if(MyVekt)
			EckVektoren.Vekt[i]=new SGLVektor(*PolygonPtr.EckVektoren.Vekt[i]);
		else
			EckVektoren.Vekt[i]=PolygonPtr.EckVektoren.Vekt[i];
	}
	useCenter=PolygonPtr.useCenter;
	twoSideRender=PolygonPtr.twoSideRender;
	if(PolygonPtr.MatIsMine)Mat=NULL;//@todo Material sollte auch kopiert werden
}

/*!
    \fn SGLPolygon::SGLPolygon(SGLVektor Ecken[],short int VektCnt)
 */
SGLPolygon::SGLPolygon(SGLVektor Ecken[],short int VektCnt)
{
	CopyEckVekt(Ecken,VektCnt);
}

/*!
    \fn SGLPolygon::SGLPolygon(SGLVektor *Ecken[],short int VektCnt)
 */
SGLPolygon::SGLPolygon(SGLVektor *Ecken[],short int VektCnt)
{
	SetEckVekt(Ecken,VektCnt);
}

/*!
    \fn SGLPolygon::~SGLPolygon()
 */
SGLPolygon::~SGLPolygon()
{
	if(MyVekt)
		for(int i=0;i<EckVektoren.Cnt;i++)
			delete EckVektoren.Vekt[i];
}

/*!
    \fn SGLPolygon::generate()
 */
void SGLPolygon::generate()
{
	if(!IgnoreLight && glIsEnabled(GL_LIGHTING))generateWithNormales();
	else generateWithoutNormales();
}

/*!
    \fn SGLPolygon::CopyEckVekt(SGLVektor Ecken[],short int VektCnt)
 */
void SGLPolygon::CopyEckVekt(SGLVektor Ecken[],short int VektCnt)
{
	MyVekt=true;EckVektoren.Cnt=VektCnt;
	for(int i=0;i<VektCnt;i++)
		EckVektoren.Vekt[i]=new SGLVektor(Ecken[i]);
}

/*!
    \fn SGLPolygon::CopyEckVekt(SGLVektor *Ecken[],short int VektCnt)
 */
void SGLPolygon::CopyEckVekt(SGLVektor *Ecken[],short int VektCnt)
{
	MyVekt=true;EckVektoren.Cnt=VektCnt;
	for(int i=0;i<VektCnt;i++)
		EckVektoren.Vekt[i]=new SGLVektor(*Ecken[i]);
}

/*!
    \fn SGLPolygon::SetEckVekt(SGLVektor *Ecken[],short int VektCnt)
 */
void SGLPolygon::SetEckVekt(SGLVektor *Ecken[],short int VektCnt)
{
	MyVekt=false;EckVektoren.Cnt=VektCnt;
	for(int i=0;i<VektCnt;i++)
		EckVektoren.Vekt[i]=Ecken[i];
}

/*!
    \fn SGLPolygon SGLPolygon::operator =(SGLPolygon PolygonPtr)
 */
SGLPolygon SGLPolygon::operator =(SGLPolygon PolygonPtr)
{
	if(PolygonPtr.MyVekt)
		CopyEckVekt(PolygonPtr.EckVektoren.Vekt,PolygonPtr.EckVektoren.Cnt);
	else
		SetEckVekt(PolygonPtr.EckVektoren.Vekt,PolygonPtr.EckVektoren.Cnt);
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
    \fn SGLVierEck::SGLVierEck(SGLVektor Ecke1,SGLVektor Ecke2,SGLVektor Ecke3,SGLVektor Ecke4)
 */
SGLVierEck::SGLVierEck(SGLVektor Ecke1,SGLVektor Ecke2,SGLVektor Ecke3,SGLVektor Ecke4):SGLPolygon()
{
	SGLVektor tEck[4];
	tEck[0]=Ecke1;
	tEck[1]=Ecke2;
	tEck[2]=Ecke3;
	tEck[3]=Ecke4;
	CopyEckVekt(tEck,4);
	setupCenter();
}

/*!
    \fn SGLVierEck::SGLVierEck(SGLVektor *Ecke1,SGLVektor *Ecke2,SGLVektor *Ecke3,SGLVektor *Ecke4)
 */
SGLVierEck::SGLVierEck(SGLVektor *Ecke1,SGLVektor *Ecke2,SGLVektor *Ecke3,SGLVektor *Ecke4):SGLPolygon()
{
	SGLVektor *tEck[4];
	tEck[0]=Ecke1;
	tEck[1]=Ecke2;
	tEck[2]=Ecke3;
	tEck[3]=Ecke4;
	SetEckVekt(tEck,4);
	setupCenter();
}
/*!
    \fn SGLVierEck::SGLVierEck(SGLVektor Ecken[4])
 */
SGLVierEck::SGLVierEck(SGLVektor Ecken[4]):SGLPolygon(Ecken,4){setupCenter();}
/*!
    \fn SGLVierEck::SGLVierEck(SGLVektor *Ecken[4])
 */
SGLVierEck::SGLVierEck(SGLVektor *Ecken[4]):SGLPolygon(Ecken,4){setupCenter();}

/*!
    \fn SGLVierEck::SGLVierEck()
 */
SGLVierEck::SGLVierEck(){}
/*!
    \fn SGLVierEck::~SGLVierEck()
 */
SGLVierEck::~SGLVierEck(){}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
    \fn SGLVierEck::~SGLVierEck()
 */
SGLDreiEck::SGLDreiEck(SGLVektor Ecke1,SGLVektor Ecke2,SGLVektor Ecke3):SGLPolygon()
{
	SGLVektor tEck[3];
	tEck[0]=Ecke1;
	tEck[1]=Ecke2;
	tEck[2]=Ecke3;
	CopyEckVekt(tEck,3);
}

/*!
    \fn SGLVierEck::~SGLVierEck()
 */
SGLDreiEck::SGLDreiEck(SGLVektor *Ecke1,SGLVektor *Ecke2,SGLVektor *Ecke3):SGLPolygon()
{
	SGLVektor *tEck[3];
	tEck[0]=Ecke1;
	tEck[1]=Ecke2;
	tEck[2]=Ecke3;
	SetEckVekt(tEck,3);
}

/*!
    \fn SGLDreiEck::SGLDreiEck(SGLVektor Ecken[3])
 */
SGLDreiEck::SGLDreiEck(SGLVektor Ecken[3]):SGLPolygon(Ecken,3){}
/*!
    \fn SGLDreiEck::SGLDreiEck(SGLVektor *Ecken[3])
 */
SGLDreiEck::SGLDreiEck(SGLVektor *Ecken[3]):SGLPolygon(Ecken,3){}

/*!
    \fn SGLDreiEck::SGLDreiEck()
 */
SGLDreiEck::SGLDreiEck(){}
SGLDreiEck::~SGLDreiEck(){}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
    \fn SGLRechtEck::SGLRechtEck(GLdouble breite,GLdouble hoehe)
 */
SGLRechtEck::SGLRechtEck(GLdouble breite,GLdouble hoehe):
SGLVierEck(SGLVektor(-breite/2,-hoehe/2),SGLVektor(breite/2,-hoehe/2),SGLVektor(breite/2,hoehe/2),SGLVektor(-breite/2,hoehe/2))
{}
SGLRechtEck::~SGLRechtEck(void){}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
    \fn SGLQuadrat::SGLQuadrat(GLdouble SeitenLaenge)
 */
SGLQuadrat::SGLQuadrat(GLdouble SeitenLaenge):SGLRechtEck(SeitenLaenge,SeitenLaenge){}

/*!
    \fn SGLPolygon::getCenter()
 */
SGLVektor SGLPolygon::getCenter()
{
	SGLVektor ret;
	for(int i=0;i<EckVektoren.Cnt;i++)
		ret+=*EckVektoren.Vekt[i];
	ret/=EckVektoren.Cnt;
	return ret;
}



/*!
    \fn SGLPolygon::setTexKoord(unsigned int eckNr,GLfloat x,GLfloat y)
 */
void SGLPolygon::setTexKoord(int eckNr,GLfloat x,GLfloat y)
{
	if(eckNr==-1)
	{
		Center.texKoord.resize(2);
		Center.texKoord[0]=x;
		Center.texKoord[1]=y;
	}
	else if(eckNr>=EckVektoren.Cnt || EckVektoren.Vekt[eckNr]==NULL)
	{
		SGLprintError("Der Vektor %d existiert nicht",eckNr);
		return;
	}
	else 
	{
		EckVektoren.Vekt[eckNr]->texKoord.resize(2);
		EckVektoren.Vekt[eckNr]->texKoord[0]=x;
		EckVektoren.Vekt[eckNr]->texKoord[1]=y;
	}
}

/*!
    \fn SGLPolygon::setTexKoord(unsigned int eckNr,GLfloat x,GLfloat y,GLfloat z)
 */
void SGLPolygon::setTexKoord(int eckNr,GLfloat x,GLfloat y,GLfloat z)
{
	if(eckNr>=EckVektoren.Cnt || EckVektoren.Vekt[eckNr]==NULL)
	{
		SGLprintError("Der Vektor %d existiert nicht",eckNr);
		return;
	}
	else if(eckNr==-1)
	{
		Center.texKoord.resize(3);
		Center.texKoord[0]=x;
		Center.texKoord[1]=y;
		Center.texKoord[2]=z;
	}
	else
	{
		EckVektoren.Vekt[eckNr]->texKoord.resize(3);
		EckVektoren.Vekt[eckNr]->texKoord[0]=x;
		EckVektoren.Vekt[eckNr]->texKoord[1]=y;
		EckVektoren.Vekt[eckNr]->texKoord[2]=z;
	}
}


/*!
    \fn SGLVierEck::resetTexKoord()
 */
void SGLVierEck::resetTexKoord()
{
	setTexKoord(0,0,0);
	setTexKoord(1,0,1);
	setTexKoord(2,1,1);
	setTexKoord(3,1,0);
	setTexKoord(-1,.5,.5);
}


/*!
    \fn SGLVierEck::setupCenter()
 */
void SGLVierEck::setupCenter()
{
	Center.SGLV_X=
		(
			EckVektoren.Vekt[0]->SGLV_X+
			EckVektoren.Vekt[1]->SGLV_X+
			EckVektoren.Vekt[2]->SGLV_X+
			EckVektoren.Vekt[3]->SGLV_X
		)/4;
	Center.SGLV_Y=
		(
			EckVektoren.Vekt[0]->SGLV_Y+
			EckVektoren.Vekt[1]->SGLV_Y+
			EckVektoren.Vekt[2]->SGLV_Y+
			EckVektoren.Vekt[3]->SGLV_Y
		)/4;
	Center.SGLV_Z=
		(
			EckVektoren.Vekt[0]->SGLV_Z+
			EckVektoren.Vekt[1]->SGLV_Z+
			EckVektoren.Vekt[2]->SGLV_Z+
			EckVektoren.Vekt[3]->SGLV_Z
		)/4;
	useCenter=true;
}


/*!
    \fn SGLPolygon::generateWithNormales()
 */
void SGLPolygon::generateWithNormales()
{
	SGLVektor* Norm=new SGLVektor[EckVektoren.Cnt];
	SGLVektor  CenterNorm;

	//Normale aus Letztem erstem und zweitem Vektor bilden
	Norm[0]=Normale(*EckVektoren.Vekt[EckVektoren.Cnt-1],*EckVektoren.Vekt[0],*EckVektoren.Vekt[1]);

	CenterNorm+=Norm[0];

	for(int i=1;i<EckVektoren.Cnt-1;i++)
	{
		//Normale aus vorherigem, aktuellem und nächsten Vektor bilden
		Norm[i]=Normale(*EckVektoren.Vekt[i-1],*EckVektoren.Vekt[i],*EckVektoren.Vekt[i+1]);
		CenterNorm+=Norm[i];
	}

	//Normale aus vorletztem, letztem und erstem Vektor bilden
	Norm[EckVektoren.Cnt-1]=Normale(*EckVektoren.Vekt[EckVektoren.Cnt-2],*EckVektoren.Vekt[EckVektoren.Cnt-1],*EckVektoren.Vekt[0]);
	CenterNorm+=Norm[EckVektoren.Cnt-1];

	if(useCenter && VisMode!=GL_LINE)
	{
	glBegin(GL_TRIANGLE_FAN);
		Center.DrawVertex(&CenterNorm);
	}
	else
	glBegin(GL_POLYGON);
		EckVektoren.Vekt[0]->DrawVertex(Norm);
		for(int i=1;i<EckVektoren.Cnt-1;i++)EckVektoren.Vekt[i]->DrawVertex(Norm+i);
		EckVektoren.Vekt[EckVektoren.Cnt-1]->DrawVertex(Norm+EckVektoren.Cnt-1);
		
		if(useCenter && VisMode!=GL_LINE)EckVektoren.Vekt[0]->DrawVertex(Norm);
	glEnd();
}


/*!
    \fn SGLPolygon::generateWithoutNormales()
 */
void SGLPolygon::generateWithoutNormales()
{
	if(useCenter && VisMode!=GL_LINE)
	{
		glBegin(GL_TRIANGLE_FAN);
		Center.DrawVertex();
	}
	else glBegin(GL_POLYGON);
		for(int i=0;i<EckVektoren.Cnt;i++)EckVektoren.Vekt[i]->DrawVertex();
		if(useCenter && VisMode!=GL_LINE)EckVektoren.Vekt[0]->DrawVertex();
	glEnd();
}

GLdouble SGLPolygon::spat(SGLVektor aim)
{
	if(EckVektoren.Cnt<3)
	{
		SGLprintError("Keine Fläche");
		return -1;
	}
	SGLVektor stuetzV(*EckVektoren.Vekt[0]);
	SGLVektor V1(*EckVektoren.Vekt[EckVektoren.Cnt-1]-stuetzV);
	SGLVektor V2(*EckVektoren.Vekt[1]-stuetzV);
	aim-=stuetzV;
	return -V1.spatprod(V2,aim);
}

/*!
    \fn SGLPolygon::canSee(SGLVektor aim)
 */
bool SGLPolygon::canSee(SGLVektor aim)
{
	return (spat(aim)>=0);
}
