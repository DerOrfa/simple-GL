/***************************************************************************
                          egltext.cpp  -  description
                             -------------------
    begin                : Thu Sep 20 2001
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

#include "sgltext.h"
#include <GL/glu.h>
#include "backend_glf/sgltextbackend_glf.h"

SGLText::SGLText(char fontname[],SGLMaterial *Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):SGLFlObj(Material,PosX,PosY,PosZ,SizeFact)
{
	loadBackend(fontname);
}

SGLText::~SGLText()
{
	delete Backend;
}

/*!
    \fn SGLText::loadBackend(char fontname[])
 */
void SGLText::loadBackend(char fontname[])
{
    Backend=new  SGLTextBackend_glf(fontname);
}
