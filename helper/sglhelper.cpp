/***************************************************************************
                          sglhelper.cpp  -  description
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

#include "sglhelper.h"
#include <stdio.h>

#define DISABLE_CLIP_PLANE(x)


SGLHelper::SGLHelper(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):SGLObj(PosX,PosY,PosZ,SizeFact)
{
	IgnoreClip=IgnoreLight=true;
	priority=floor;
}
SGLHelper::~SGLHelper()
{
}

