/***************************************************************************
                          sglflobj.h  -  description
                             -------------------
    begin                : Tue Oct 2 2001
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

#ifndef SGLFLOBJ_H
#define SGLFLOBJ_H



#include "../util/sglmaterial.h"
#include "../sglobj.h"

/**
  *@author Enrico Reimer
  */
class SGLFlObj : public SGLObj
{
public:
	SGLFlObj(
		MaterialPtr Material=MaterialPtr (),
		GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,
		GLdouble SizeFact=1
		);
	
	GLuint Compile(bool draw=true);
	virtual void generate()=0;
	void DrahtGitter(bool DO=true);
	void resetMaterial(MaterialPtr NewMaterial=MaterialPtr());
    virtual void resetTexKoord();
	
	GLenum VisMode;
	MaterialPtr Mat;
    bool twoSideRender;
};

#endif
