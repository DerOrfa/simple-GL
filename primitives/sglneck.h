/***************************************************************************
                          sglneck.h  -  description
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

#ifndef SGLNECK_H
#define SGLNECK_H

#include "sglvieleck.h"

/**
  *@author Enrico Reimer
  */
class SGLnEck : public SGLPolygon
{
public: 
	SGLnEck(int Ecken,double size, bool ccw=true);
	~SGLnEck();
};

#endif
