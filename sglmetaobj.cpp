/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sglmetaobj.h"

SGLMetaObj::SGLMetaObj(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
:SGLObj(PosX,PosY,PosZ,SizeFact)
{}

SGLMetaObj::~SGLMetaObj()
{
	Objs.clear();
	TrObjs.clear();
}

/*!
    \fn SGLMetaObj::generate()
 */
void SGLMetaObj::generate()
{
	int i;
	for(i=Objs.size();i;i--)
		glCallList(Objs[i-1]);

	if(TrObjs.size())
	{
		//Verhindern, daﬂ das Enable wegoptimiert wird 
		//gibt Probleme wenn die Liste bei ge‰ndertem Zustand der Maschine aufgerufen wird
		glDisable(GL_BLEND);
		glEnable(GL_BLEND);
		for(i=TrObjs.size();i;i--)
			glCallList(TrObjs[i-1]);
		glDisable(GL_BLEND);
	}
}
