/***************************************************************************
                          sglneck.cpp  -  description
                             -------------------
    begin                : Tue Jun 25 2002
    copyright            : (C) 2002 by Enrico Reimer
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

#include <emath_misc.h>
#include "sglneck.h"
#include "../sglvektor.h"

SGLnEck::SGLnEck(int ecken,double size, bool ccw)
{
    SGLVektor *Ecken=new SGLVektor[ecken];

   	double unterteilg=360/ecken;
	
	for(int i=0;i<ecken;i++)
		Ecken[i]=SGLVektor(COS(i*unterteilg)*size,SIN(i*unterteilg)*size);

	CopyEckVekt(Ecken,ecken);
}
SGLnEck::~SGLnEck(){
}
