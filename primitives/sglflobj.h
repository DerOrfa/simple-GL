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
	SGLFlObj(SGLMaterial *Material=0,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGLFlObj();
	GLuint Compile();
	GLenum VisMode;
	virtual void generate()=0;
	void DrahtGitter(bool DO=true);
	void ResetMaterial(SGLMaterial *NewMaterial=0);
    virtual void resetTexKoord();
	SGLMaterial *Mat;
	bool MatIsMine;
    bool twoSideRender;
};

#endif
