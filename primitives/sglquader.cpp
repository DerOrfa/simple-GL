/***************************************************************************
                          sglquader.cpp  -  description
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

#include "sglquader.h"
#include "sglvieleck.h"
#include "../sglmisc.h"


SGLQuader::SGLQuader(SGLMaterial *Material,GLdouble breite,GLdouble hoehe,GLdouble tiefe,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLPolygonObj(Material,PosX,PosY,PosZ,SizeFact)
{
	this->breite=breite;
	this->hoehe=hoehe;
	this->tiefe=tiefe;

	Fl.Fl[0]= new SGLVierEck(&EckPunkte[0][0],&EckPunkte[0][1],&EckPunkte[0][2],&EckPunkte[0][3]);	//Front
	Fl.Fl[1]= new SGLVierEck(&EckPunkte[1][3],&EckPunkte[1][2],&EckPunkte[1][1],&EckPunkte[1][0]);	//Hinten

	Fl.Fl[2]= new SGLVierEck(&EckPunkte[0][0],&EckPunkte[0][3],&EckPunkte[1][3],&EckPunkte[1][0]);	//linke Seite
	Fl.Fl[3]= new SGLVierEck(&EckPunkte[0][1],&EckPunkte[1][1],&EckPunkte[1][2],&EckPunkte[0][2]);	//Rechte Seite

	Fl.Fl[4]= new SGLVierEck(&EckPunkte[0][3],&EckPunkte[0][2],&EckPunkte[1][2],&EckPunkte[1][3]);	//Deckel
	Fl.Fl[5]= new SGLVierEck(&EckPunkte[1][0],&EckPunkte[1][1],&EckPunkte[0][1],&EckPunkte[0][0]);	//Boden
	Fl.Cnt=6;
	recalcEdges(false);
	resetTexKoord();
}
SGLQuader::~SGLQuader()
{
}

void SGLQuader::ColorCube()
{
	EckPunkte[1][0].SetColor(0,0,0);
	EckPunkte[1][1].SetColor(255);
	EckPunkte[1][2].SetColor(255,255);
	EckPunkte[1][3].SetColor(0,255);

	EckPunkte[0][0].SetColor(0,0,255);
	EckPunkte[0][1].SetColor(255,0,255);
	EckPunkte[0][2].SetColor(255,255,255);
	EckPunkte[0][3].SetColor(0,255,255);
	resetMaterial();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SGLCube::SGLCube(SGLMaterial *Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLQuader(Material,1,1,1,PosX,PosY,PosZ,SizeFact){}
SGLCube::~SGLCube(){}

/*!
    \fn SGLQuader::setDepth(GLdouble depth)
 */
void SGLQuader::setDepth(GLdouble depth)
{
	tiefe=depth;
	recalcEdges();
	resetTexKoord();
}

/*!
    \fn SGLQuader::setHeight(GLdouble height)
 */
void SGLQuader::setHeight(GLdouble height)
{
	hoehe=height;
	recalcEdges();
	resetTexKoord();
}

/*!
    \fn SGLQuader::setWidth(GLdouble width)
 */
void SGLQuader::setWidth(GLdouble width)
{
	breite=width;
	recalcEdges();
	resetTexKoord();
}

/*!
    \fn SGLQuader::recalcEdges(bool compile=true)
 */
void SGLQuader::recalcEdges(bool compile)
{
	EckPunkte[0][0]=SGLVektor(-breite/2,-hoehe/2,tiefe/2);
	EckPunkte[0][1]=SGLVektor( breite/2,-hoehe/2,tiefe/2);
	EckPunkte[0][2]=SGLVektor( breite/2, hoehe/2,tiefe/2);
	EckPunkte[0][3]=SGLVektor(-breite/2, hoehe/2,tiefe/2);

	EckPunkte[1][0]=SGLVektor(-breite/2,-hoehe/2,-tiefe/2);
	EckPunkte[1][1]=SGLVektor( breite/2,-hoehe/2,-tiefe/2);
	EckPunkte[1][2]=SGLVektor( breite/2, hoehe/2,-tiefe/2);
	EckPunkte[1][3]=SGLVektor(-breite/2, hoehe/2,-tiefe/2);

	for(int i=0;i<this->Fl.Cnt;i++)
		((SGLVierEck*)Fl.Fl[i])->setupCenter();

	if(compile)Compile();
}

/*!
    \fn SGLQuader::resetTexKoord()
 */
void SGLQuader::resetTexKoord()
{
	if(Fl.Fl[0])Fl.Fl[0]->resetTexKoord();
	else {SGLprintWarning("Das Fl�chenpolygon 0 ex. nicht");}
	if(Fl.Fl[1])Fl.Fl[1]->resetTexKoord();
	else {SGLprintWarning("Das Fl�chenpolygon 1 ex. nicht");}
}

void SGLQuader::setDiag(SGLVektor topleftfront,SGLVektor bottomrighttail,bool compile)
{
	SGLVektor mitte=SGLVektor(topleftfront+bottomrighttail)/2.;
	SGLVektor dim=(topleftfront-bottomrighttail);
	ResetTransformMatrix();
	Move(mitte);
	breite=dim.SGLV_X<0 ? -dim.SGLV_X:dim.SGLV_X;
	hoehe=dim.SGLV_Y < 0 ? -dim.SGLV_Y:dim.SGLV_Y;
	tiefe=dim.SGLV_Z < 0 ? -dim.SGLV_Z:dim.SGLV_Z;
	recalcEdges(compile);
}
