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
#include <boost/shared_ptr.hpp>

/**
  *@author Enrico Reimer
  */
class SGLFlObj : public SGLObj
{
public:
	SGLFlObj(
		SGLMaterial *Mat=NULL,
		GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,
		GLdouble SizeFact=1
		);
	SGLFlObj(const SGLFlObj& src);
	
	~SGLFlObj();
	GLuint Compile();
	virtual void generate()=0;
	void DrahtGitter(bool DO=true);
	void resetMaterial(boost::shared_ptr<SGLMaterial> NewMaterial=boost::shared_ptr<SGLMaterial>());
    virtual void resetTexKoord();
	
	GLenum VisMode;
	boost::shared_ptr<SGLMaterial> Mat;
    bool twoSideRender;
};

#endif
