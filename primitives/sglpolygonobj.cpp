/***************************************************************************
                          sglpoygonobj.cpp  -  description
                             -------------------
    begin                : Thu Oct 4 2001
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

#include "sglpolygonobj.h"
#include <stdio.h>
#include "../sglmisc.h"

SGLPolygonObj::SGLPolygonObj(SGLMaterial *Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLFlObj(Material,PosX,PosY,PosZ,SizeFact)
{
	Fl.Cnt=0;
}

SGLPolygonObj::SGLPolygonObj(const SGLPolygon Flaechen[],short int FlCnt,SGLMaterial *Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLFlObj(Material,PosX,PosY,PosZ,SizeFact)
{
	SetFl(Flaechen,FlCnt);
}

void SGLPolygonObj::SetFl(const SGLPolygon Flaechen[],short int FlCnt)
{
	Fl.Cnt=FlCnt;
	for(int i=0;i<FlCnt;i++)
		Fl.Fl[i]=boost::shared_ptr<SGLPolygon>(new SGLPolygon(Flaechen[i]));
}
/** No descriptions */
void SGLPolygonObj::generate()
{
	for(int i=0;i<Fl.Cnt;i++)
	{
		Fl.Fl[i]->VisMode=VisMode;
		Fl.Fl[i]->twoSideRender=twoSideRender;
		Fl.Fl[i]->generate();
	}
	if(!Fl.Cnt){SGLprintError("Versuch ein Polygonobjekt ohne Seitenflächen zu zeichnen");}
}

/*!
    \fn SGLPolygonObj::getCenter()
 */
SGLVektor SGLPolygonObj::getCenter()
{
	SGLVektor ret;
	for(int i=0;i<Fl.Cnt;i++)
		ret+=Fl.Fl[i]->getCenter();
	return ret/Fl.Cnt;
}
