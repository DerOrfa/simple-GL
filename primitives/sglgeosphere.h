/***************************************************************************
                          sglgeosphere.h  -  description
                             -------------------
    begin                : Mon Oct 1 2001
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

#ifndef SGLGEOSPHERE_H
#define SGLGEOSPHERE_H

#include "sglicosahedron.h"

/**
  *@author Enrico Reimer
  */
class SGLGeosphere:public SGLIcosahedron
{
public: 
	SGLGeosphere(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGLGeosphere();
};

#endif
