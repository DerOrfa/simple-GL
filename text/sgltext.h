/***************************************************************************
                          egltext.h  -  description
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

#ifndef SGLTEXT_H
#define SGLTEXT_H

#include "../primitives/sglflobj.h"
#include <auto_ptr.h>

class FTFont;

/**
  *@author Enrico Reimer
  */
class SGLText : public SGLFlObj
{
protected:
	std::auto_ptr<FTFont> renderer;
public:
	static const char* findFont(const char fontname[]);
	SGLText(std::auto_ptr<FTFont> renderer,MaterialPtr Material=MaterialPtr(),GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
protected:
	std::string myText;
};

#endif
