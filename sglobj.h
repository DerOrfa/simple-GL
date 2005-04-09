/***************************************************************************
                          eglobj.h  -  description
                             -------------------
    begin                : Sat Sep 15 2001
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

#ifndef SGLOBJ_H
#define SGLOBJ_H

#include "sglobjbase.h"
#include <boost/smart_ptr.hpp>

/**
  *@author Enrico Reimer
  */
class SGLObj:public SGLObjBase
{
public:
	SGLObj(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	GLuint Compile(bool draw=true,bool free=false);
};

/**
  *@author Enrico Reimer
  */
class SGLStrecke:public SGLObj
{
	bool myPunkte;
public:
	boost::shared_ptr<SGLVektor> punkt1,punkt2;
	SGLStrecke(boost::shared_ptr<SGLVektor> Vekt1,boost::shared_ptr<SGLVektor> Vekt2);
	SGLStrecke(SGLVektor Vekt1,SGLVektor Vekt2=SGLVektor(0,0,0));
	SGLStrecke();

	GLdouble Len();

	void generate();
	SGLVektor getCenter()const;
};
#endif

