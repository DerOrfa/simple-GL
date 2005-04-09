/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sgl3dtext.h"

SGL3DText::SGL3DText(const char Text[], char fontname[],MaterialPtr Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
:SGLText(fontname,Material,PosX,PosY,PosZ,SizeFact)
{
	align=center;
	FrontFace=GL_CW;
	tiefe=0.2f;
	myText=Text;
}


SGL3DText::~SGL3DText()
{
}

void SGL3DText::generate()
{
	Backend->generate_3DText(getDepth(),myText,VisMode,align);
}

/*!
    \fn SGL3DText::getBounds(SGLQuader *BoundingQuader)
 */
void SGL3DText::getBounds(SGLQuader *BoundingQuader)
{
	GLdouble breite,hoehe,depth;
	SGLVektor center=getCenter();

	getDim(&breite,&hoehe,&depth,NULL);


	BoundingQuader->tiefe=depth*1.01;
	BoundingQuader->hoehe=hoehe;
	BoundingQuader->breite=breite;
	BoundingQuader->recalcEdges(false);
	BoundingQuader->ResetTransformMatrix(MyTransformMatrix);

	BoundingQuader->Move(center.SGLV_X,center.SGLV_Y,center.SGLV_Z);

	BoundingQuader->FaceAt=FaceAt;
	BoundingQuader->resetTexKoord();
	BoundingQuader->Compile();
}

/*!
    \fn SGL3DText::getHeight()
 */
GLdouble SGL3DText::getHeight()const
{
	GLdouble temp;
	getDim(NULL,&temp);
	return temp;
}

/*!
    \fn SGL3DText::getDepth()
 */
GLdouble SGL3DText::getDepth()const
{
	GLdouble temp;
	getDim(NULL,NULL,&temp);
	return temp;
}

/*!
    \fn SGL3DText::getWidth()
 */
GLdouble SGL3DText::getWidth()const
{
	GLdouble temp;
	getDim(&temp);
	return temp;
}

/*!
    \fn SGL3DText::getCenter()
 */
SGLVektor SGL3DText::getCenter()const
{
	SGLVektor temp;
	GLdouble width=getWidth();
	switch(align)
	{
	case left:temp.SGLV_X+=width/2;break;
	case right:temp.SGLV_X-=width/2;break;
	default:;
	}
	return temp;
}


/*!
    \fn SGL3DText::DrahtGitter(bool DO)
 */
void SGL3DText::DrahtGitter(bool DO)
{
	if(DO)VisMode=GL_LINE;
	else VisMode=GL_FILL;
}

/*!
    \fn SGL3DText::getDim(GLdouble *width,GLdouble *height,GLdouble *depth,SGLVektor *center)
 */
void SGL3DText::getDim(GLdouble *width,GLdouble *height,GLdouble *depth,SGLVektor *center)const
{
	Backend->getDim(myText,width,height,depth,center,align);
}
