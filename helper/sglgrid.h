/***************************************************************************
                          eglgrid.h  -  description
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

#ifndef SGLGRID_H
#define SGLGRID_H

#include "sglhelper.h"
#include "../text/sgl3dtext.h"

/**
  *@author Enrico Reimer
  */
class SGLGrid : public SGLHelper
{
public:
	SGLGrid(short GridType);
	~SGLGrid();
	virtual void generate();
	short Type;
	GLint Size;
    public: SGLVektor getCenter();
    void setSize(GLuint size);
};

#endif
