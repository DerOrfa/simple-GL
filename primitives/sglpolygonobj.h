/***************************************************************************
                          sglpoygonobj.h  -  description
                             -------------------
    begin                : Thu Oct 4 2001
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

#ifndef SGLPOYGONOBJ_H
#define SGLPOYGONOBJ_H

#include "sglvieleck.h"

/**
  *@author Enrico Reimer
  */
class SGLPolygonObj : public SGLFlObj
{
public:
	SGLPolygonObj(SGLMaterial *Material=0,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	SGLPolygonObj(const SGLPolygon Flaechen[],short int FlCnt,SGLMaterial *Material=0,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	struct {SGLPolygon* Fl[50];short int Cnt;}Fl;
	~SGLPolygonObj();
	void SetFl(const SGLPolygon Flaechen[],short int FlCnt);
	void generate();

    public: SGLVektor getCenter();
};

#endif
