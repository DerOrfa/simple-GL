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

/**
  *@author Enrico Reimer
  */
class SGLPolygon : public SGLFlObj
{
public:
	SGLPolygon();
	SGLPolygon(const SGLPolygon &PolygonPtr);
	SGLPolygon(SGLVektor Ecken[],short int VektCnt);
	SGLPolygon(SGLVektor *Ecken[],short int VektCnt);
	~SGLPolygon();

	void generate();

	void CopyEckVekt(SGLVektor Ecken[],short int VektCnt);
	void CopyEckVekt(SGLVektor *Ecken[],short int VektCnt);
	void SetEckVekt(SGLVektor *Ecken[],short int VektCnt);

	bool MyVekt;
	struct{SGLVektor *Vekt[50];short int Cnt;}EckVektoren;
	SGLPolygon operator =(SGLPolygon PolygonPtr);

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
	SGLVierEck();
	SGLVierEck(SGLVektor Ecken[4]);
	SGLVierEck(SGLVektor *Ecken[4]);
	SGLVierEck(SGLVektor Ecke1,SGLVektor Ecke2,SGLVektor Ecke3,SGLVektor Ecke4);
	SGLVierEck(SGLVektor *Ecke1,SGLVektor *Ecke2,SGLVektor *Ecke3,SGLVektor *Ecke4);
	~SGLVierEck();
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
	SGLDreiEck(SGLVektor *Ecken[3]);
	SGLDreiEck(SGLVektor Ecke1,SGLVektor Ecke2,SGLVektor Ecke3);
	SGLDreiEck(SGLVektor *Ecke1,SGLVektor *Ecke2,SGLVektor *Ecke3);
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

#endif
