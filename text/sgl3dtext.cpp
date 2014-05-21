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
#include <FTGL/ftgl.h>


SGL3DText::SGL3DText(const char Text[], const char fontname[],MaterialPtr Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
:SGLText(std::auto_ptr<FTFont>(new FTExtrudeFont(SGLText::findFont(fontname))),Material,PosX,PosY,PosZ,SizeFact)
{
	renderer->FaceSize(72/25.4); // 72 is one inch 72/25.4 is one mm
	renderer->Depth(0.2);
	myText=Text;
	FrontFace=GL_CW;
}


SGL3DText::~SGL3DText()
{
}

void SGL3DText::generate()
{
	FTBBox box=renderer->BBox(myText.c_str());
	const FTPoint &offset=box.Lower();
	const FTPoint center=offset+(box.Upper()-offset)*.5;

	glTranslatef(-center.X(),-center.Y(),-center.Z());
	renderer->Render(myText.c_str());
}

/*!
    \fn SGL3DText::getBounds(SGLQuader *BoundingQuader)
 */
void SGL3DText::getBounds(SGLQuader *BoundingQuader)
{
	GLdouble breite,hoehe,depth;

	getDim(&breite,&hoehe,&depth,NULL);
	BoundingQuader->tiefe=depth;
	BoundingQuader->hoehe=hoehe;
	BoundingQuader->breite=breite;
	BoundingQuader->recalcEdges(false);
	BoundingQuader->ResetTransformMatrix(MyTransformMatrix);

	BoundingQuader->FaceAt=FaceAt;
	BoundingQuader->resetTexKoord();
	BoundingQuader->Compile();
}

/*!
    \fn SGL3DText::getCenter()
 */
SGLVektor SGL3DText::getCenter()const
{
	// zero by definition
	return SGLVektor(0,0,0);
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
