/***************************************************************************
                          sglquader.h  -  description
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

#ifndef SGLQUADER_H
#define SGLQUADER_H

#include "sglpolygonobj.h"

/**
  *@author Enrico Reimer
  */
class SGLQuader : public SGLPolygonObj
{
public:
	SGLQuader(MaterialPtr Material=MaterialPtr(),GLdouble breite=1,GLdouble hoehe=1,GLdouble tiefe=1,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	VektorPtr EckPunkte[2][4];
	void ColorCube();

	GLdouble breite,hoehe,tiefe;

    void setDepth(GLdouble depth);
    void setHeight(GLdouble height);
    void setWidth(GLdouble width);

	void setDiag(SGLVektor topleftfront,SGLVektor bottomrighttail,bool compile=true);

    public: void recalcEdges(bool compile=true);
    void resetTexKoord();
};

/**
  *@author Enrico Reimer
  */
class SGLCube: public SGLQuader
{
public:
	SGLCube(MaterialPtr Material=MaterialPtr(),GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
};
#endif
