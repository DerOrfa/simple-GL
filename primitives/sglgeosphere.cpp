/***************************************************************************
                          sglgeosphere.cpp  -  description
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

#include "sglgeosphere.h"

SGLGeosphere::SGLGeosphere(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLIcosahedron(3,PosX,PosY,PosZ,SizeFact)
{
}

SGLGeosphere::~SGLGeosphere()
{
}
