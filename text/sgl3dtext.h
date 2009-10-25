/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGL3DTEXT_H
#define SGL3DTEXT_H

#include "sgltext.h"
#include "../primitives/sglflobj.h"
#include "../primitives/sglquader.h"

/**
@author Enrico Reimer
*/
class SGL3DText : public SGLText
{
public:
	SGL3DText(const char Text[]="", const char fontname[]="",MaterialPtr Material=MaterialPtr(),GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	~SGL3DText();

	GLdouble tiefe;
	void getBounds(SGLQuader *BoundingQuader);

	GLdouble getHeight()const;
	GLdouble getDepth()const;
	GLdouble getWidth()const;
	SGLVektor getCenter()const;
// 	@todo void ResetMaterial(SGLMaterial *NewMaterial=0);

	void getDim(GLdouble *width=NULL,GLdouble *height=NULL,GLdouble *depth=NULL,SGLVektor *center=NULL)const;
	TextAlign align;

	void generate();
	void DrahtGitter(bool DO=true);
};

#endif
