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
#include <assert.h>
#include <GL/glu.h>
#include "../util/sglmaterial.h"

/*!
    \fn SGLPolygon::SGLPolygon()
 */
SGLPolygon::SGLPolygon()
{
	useCenter=false;
	twoSideRender=true;
}

/*!
    \fn SGLPolygon::SGLPolygon(SGLVektor Ecken[],short int VektCnt)
 */
SGLPolygon::SGLPolygon(SGLVektor Ecken[],short int VektCnt){CopyEckVekt(Ecken,VektCnt);}

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
	EckVektoren.resize(VektCnt);
	for(int i=0;i<VektCnt;i++)
		EckVektoren[i].reset(new SGLVektor(Ecken[i]));
	resetTexKoord();
}

/*!
    \fn SGLPolygon::CopyEckVekt(SGLVektor *Ecken[],short int VektCnt)
 */
void SGLPolygon::CopyEckVekt(VektorList &Ecken)
{
	VektorList::size_type s=Ecken.size();
	EckVektoren.resize(s);
	for(VektorList::size_type i=0;i<s;i++)
		EckVektoren[i].reset(new SGLVektor(*Ecken[i]));
	resetTexKoord();
}

/*!
    \fn SGLPolygon::CopyEckVekt(SGLVektor Ecken[],short int VektCnt)
 */
void SGLPolygon::LinkEckVekt(VektorPtr Ecken[],short int VektCnt)
{
	EckVektoren.resize(VektCnt);
	for(int i=0;i<VektCnt;i++)
		EckVektoren[i]=Ecken[i];
	resetTexKoord();
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

void SGLVierEck::Link(VektorPtr Ecke1,VektorPtr Ecke2,VektorPtr Ecke3,VektorPtr Ecke4)
{
	VektorPtr tEck[4];
	tEck[0]=Ecke1;
	tEck[1]=Ecke2;
	tEck[2]=Ecke3;
	tEck[3]=Ecke4;
	LinkEckVekt(tEck,4);
	setupCenter();
}

/*!
    \fn SGLVierEck::SGLVierEck(SGLVektor Ecken[4])
 */
SGLVierEck::SGLVierEck(SGLVektor Ecken[4]):SGLPolygon(Ecken,4){setupCenter();resetTexKoord();}

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

void SGLDreiEck::Link(VektorPtr Ecke1,VektorPtr Ecke2,VektorPtr Ecke3)
{
	VektorPtr tEck[3];
	tEck[0]=Ecke1;
	tEck[1]=Ecke2;
	tEck[2]=Ecke3;
	LinkEckVekt(tEck,3);
}

/*!
    \fn SGLDreiEck::SGLDreiEck(SGLVektor Ecken[3])
 */
SGLDreiEck::SGLDreiEck(SGLVektor Ecken[3]):SGLPolygon(Ecken,3){}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
    \fn SGLRechtEck::SGLRechtEck(GLdouble breite,GLdouble hoehe)
 */
SGLRechtEck::SGLRechtEck(GLdouble breite,GLdouble hoehe):
SGLVierEck(SGLVektor(-breite/2,-hoehe/2),SGLVektor(breite/2,-hoehe/2),SGLVektor(breite/2,hoehe/2),SGLVektor(-breite/2,hoehe/2))
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
    \fn SGLQuadrat::SGLQuadrat(GLdouble SeitenLaenge)
 */
SGLQuadrat::SGLQuadrat(GLdouble SeitenLaenge):SGLRechtEck(SeitenLaenge,SeitenLaenge){}

/*!
    \fn SGLPolygon::getCenter()
 */
SGLVektor SGLPolygon::getCenter()const
{
	SGLVektor ret;
	for(int i=EckVektoren.size();i;i--)
		ret+=*EckVektoren[i-1];
	ret/=EckVektoren.size();
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
	else if((unsigned int)eckNr>=EckVektoren.size() || !EckVektoren[(unsigned int)eckNr])
	{
		SGLprintError("Der Vektor %d existiert nicht",eckNr);
		return;
	}
	else
	{
		EckVektoren[eckNr]->texKoord.resize(2);
		EckVektoren[eckNr]->texKoord[0]=x;
		EckVektoren[eckNr]->texKoord[1]=y;
	}
}

/*!
    \fn SGLPolygon::setTexKoord(unsigned int eckNr,GLfloat x,GLfloat y,GLfloat z)
 */
void SGLPolygon::setTexKoord(int eckNr,GLfloat x,GLfloat y,GLfloat z)
{
	if(eckNr==-1)
	{
		Center.texKoord.resize(3);
		Center.texKoord[0]=x;
		Center.texKoord[1]=y;
		Center.texKoord[2]=z;
	}
	else if((unsigned int)eckNr>=EckVektoren.size()  || !EckVektoren[(unsigned int)eckNr])
	{
		SGLprintError("Der Vektor %d existiert nicht",eckNr);
		return;
	}
	else
	{
		EckVektoren[eckNr]->texKoord.resize(3);
		EckVektoren[eckNr]->texKoord[0]=x;
		EckVektoren[eckNr]->texKoord[1]=y;
		EckVektoren[eckNr]->texKoord[2]=z;
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
#define	SETUP_CENTER(DIM)	Center.DIM=(EckVektoren[0]->DIM+EckVektoren[1]->DIM+EckVektoren[2]->DIM+EckVektoren[3]->DIM)/4
	SETUP_CENTER(SGLV_X);
	SETUP_CENTER(SGLV_Y);
	SETUP_CENTER(SGLV_Z);
#undef SETUP_CENTER
	useCenter=true;
}


/*!
    \fn SGLPolygon::generateWithNormales()
 */
void SGLPolygon::generateWithNormales()
{
	VektorList::size_type s=EckVektoren.size();
	SGLVektor* Norm=new SGLVektor[s];
	SGLVektor  CenterNorm;

	//Normale aus Letztem erstem und zweitem Vektor bilden
	Norm[0]=Normale(
		*EckVektoren[s-1],
		*EckVektoren[0],
		*EckVektoren[1]
		);

	CenterNorm+=Norm[0];

	for(VektorList::size_type i=1;i<s-1;i++)
	{
		//Normale aus vorherigem, aktuellem und nächsten Vektor bilden
		Norm[i]=Normale(
			*EckVektoren[i-1],
			*EckVektoren[i],
			*EckVektoren[i+1]
			);
		CenterNorm+=Norm[i];
	}

	//Normale aus vorletztem, letztem und erstem Vektor bilden
	Norm[s-1]=Normale(
		*EckVektoren[s-2],
		*EckVektoren[s-1],
		*EckVektoren[0]);
	CenterNorm+=Norm[s-1];

	if(useCenter && VisMode!=GL_LINE)
	{
	glBegin(GL_TRIANGLE_FAN);
		Center.DrawVertex(&CenterNorm);
	}
	else
	glBegin(GL_POLYGON);
		EckVektoren[0]->DrawVertex(&Norm[0]);
		for(VektorList::size_type i=1;i<s-1;i++)
			EckVektoren[i]->DrawVertex(&Norm[i]);
		EckVektoren[s-1]->DrawVertex(&Norm[s-1]);

		if(useCenter && VisMode!=GL_LINE)
			EckVektoren[0]->DrawVertex(&Norm[0]);
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
		for(VektorList::size_type i=0;i<EckVektoren.size();i++)
			EckVektoren[i]->DrawVertex();
		if(useCenter && VisMode!=GL_LINE)
			EckVektoren[0]->DrawVertex();
	glEnd();
}

GLdouble SGLPolygon::spat(SGLVektor aim)
{
	VektorList::size_type s=EckVektoren.size();
	if(s<3)
	{
		SGLprintError("Keine Fläche");
		return -1;
	}
	SGLVektor stuetzV(*EckVektoren[0]);
	SGLVektor V1(*EckVektoren[s-1]-stuetzV);
	SGLVektor V2(*EckVektoren[1]-stuetzV);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
    \fn SGL3DPlane::SGL3DPlane(GLdouble SeitenLaenge)
 */
SGL3DPlane::SGL3DPlane(GLdouble breite,GLdouble hoehe,SGLshPtr<SGLBaseTex> volumeTex):SGLRechtEck(breite,hoehe)
{
	if(volumeTex)
	{
		if(volumeTex->TexType!=GL_TEXTURE_3D)
		{
			SGLprintError("Die Volumentextur ist kein Dreidimensionaler Datensatz");
		}
		Mat->SetTex(volumeTex);
	}
}

void SGL3DPlane::resetTexKoord()
{
	setTexKoord(0,0,0,.5);
	setTexKoord(1,0,1,.5);
	setTexKoord(2,1,1,.5);
	setTexKoord(3,1,0,.5);
	setTexKoord(-1,.5,.5,.5);
}
