/***************************************************************************
                          sglvieleck.h  -  description
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

#ifndef SGLVIELECK_H
#define SGLVIELECK_H

#include "sglflobj.h"
#include "../helper/sglcamera.h"

/**
  *@author Enrico Reimer
  */
class SGLPolygon : public SGLFlObj
{
public:
	SGLPolygon();
	SGLPolygon(SGLVektor Ecken[],short int VektCnt);
//	SGLPolygon(VektorPtr Ecken[],short int VektCnt);

	void generate();

	void CopyEckVekt(SGLVektor Ecken[],short int VektCnt);
	void CopyEckVekt(VektorList &Ecken);
	void LinkEckVekt(VektorPtr Ecken[],short int VektCnt);
	void LinkEckVekt(VektorList &Ecken);

	VektorList EckVektoren;

	SGLVektor getCenter();
	void setTexKoord(int eckNr,GLfloat x,GLfloat y);
	void setTexKoord(int eckNr,GLfloat x,GLfloat y,GLfloat z);
    void generateWithNormales();
    void generateWithoutNormales();
	GLdouble spat(SGLVektor aim);
    bool canSee(SGLVektor aim);
	SGLVektor Center;
	bool useCenter;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  *@author Enrico Reimer
  */
/**
  *@author Enrico Reimer
  */
class SGLVierEck : public SGLPolygon
{
public:
	SGLVierEck(SGLVektor Ecken[4]);
	SGLVierEck(
		SGLVektor Ecke1=SGLVektor(.5,.5,0),
		SGLVektor Ecke2=SGLVektor(-.5,.5,0),
		SGLVektor Ecke3=SGLVektor(-.5,-.5,0),
		SGLVektor Ecke4=SGLVektor(.5,-.5,0));
	void Link(VektorPtr Ecke1,VektorPtr Ecke2,VektorPtr Ecke3,VektorPtr Ecke4);
	void resetTexKoord();
	void setupCenter();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  *@author Enrico Reimer
  */
class SGLDreiEck : public SGLPolygon
{
public:
	SGLDreiEck(SGLVektor Ecken[3]);
	SGLDreiEck(SGLVektor Ecke1,SGLVektor Ecke2,SGLVektor Ecke3);
	void Link(VektorPtr Ecke1,VektorPtr Ecke2,VektorPtr Ecke3);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  *@author Enrico Reimer
  */
class SGLRechtEck : public SGLVierEck
{
public:
	SGLRechtEck(GLdouble breite,GLdouble hoehe);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  *@author Enrico Reimer
  */
class SGLQuadrat : public SGLRechtEck
{
public:
	SGLQuadrat(GLdouble SeitenLaenge);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  *@author Enrico Reimer
  */
class SGL3DPlane : public SGLRechtEck
{
public:
	SGL3DPlane(GLdouble breite,GLdouble hoehe,boost::shared_ptr<SGLBaseTex> volumeTex);
	void resetTexKoord();
};

#endif
