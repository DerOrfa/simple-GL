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

SGL3DText::SGL3DText(const char Text[], const char fontname[],MaterialPtr Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
:SGLText(std::auto_ptr<FTFont>(new FTExtrudeFont(SGLText::findFont(fontname))),Material,PosX,PosY,PosZ,SizeFact)
{
	renderer->FaceSize(72/25.4); // 72 is one inch 72/25.4 is one mm
	renderer->Depth(0.2);
	myText=Text;
}


SGL3DText::~SGL3DText()
{
}

void SGL3DText::generate()
{
	SGLVektor center=getCenter(); // move center of text to the drawing position
	glFrontFace(GL_CW);
	glTranslatef(-center.SGLV_X,-center.SGLV_Y,-center.SGLV_Z);
	renderer->Render(myText.c_str());
	glFrontFace(GL_CCW);
}

/*!
    \fn SGL3DText::getBounds(SGLQuader *BoundingQuader)
 */
void SGL3DText::getBounds(SGLQuader *BoundingQuader)
{
#warning test me
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
	FTBBox box=renderer->BBox(myText.c_str());
	const FTPoint &offset=box.Lower();
	const FTPoint ret=offset+(box.Upper()-offset)*.5;
	return SGLVektor(ret.X(),ret.Y(),ret.Z());
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
	FTBBox box=renderer->BBox(myText.c_str());
	if(width)*width=box.Upper().X()-box.Lower().X();
	if(height)*height=box.Upper().Y()-box.Lower().Y();
	if(depth)*depth=box.Upper().Z()-box.Lower().Z();

}
