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
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else 
	#include <GL/glu.h>
#endif
#include "../sglmisc.h"

SGLText::SGLText(std::auto_ptr<FTFont> _renderer,MaterialPtr Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLFlObj(Material,PosX,PosY,PosZ,SizeFact),renderer(_renderer){
	if(renderer->Error())
		SGLprintError("Failed to load font rendering");
}

const char* SGLText::findFont(const char fontname[])
{
	const std::string f(fontname);
	return f.empty()? DEFAULT_TTF: fontname;
}
