/***************************************************************************
                          eglicosahedron.h  -  description
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

#ifndef SGLICOSAHEDRON_H
#define SGLICOSAHEDRON_H

#include "sglflobj.h"

/**Ein Beispielobjekt (Ein Icosahedron)
  *@author Enrico Reimer
  */
class SGLIcosahedron : public SGLFlObj
{
public:
	unsigned short int SubDiv;
	long int PolyCount;
	SGLIcosahedron(unsigned short int SubDivisions=0,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGLIcosahedron();
	void generate();
	long int subdivide(SGLVektor v1,SGLVektor v2, SGLVektor v3,unsigned int depth);
	GLdouble X,Z;

	SGLVektor getCenter()const;
};

#endif
