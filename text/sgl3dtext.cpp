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


const FTBBox &SGL3DText::box()const
{
	if(m_box.get()==NULL)
		const_cast<std::auto_ptr<FTBBox>&>(m_box)=
			std::auto_ptr<FTBBox>(new FTBBox( renderer->BBox(myText.c_str())));// TODO baaad voodoo
	return *m_box;
}

SGL3DText::SGL3DText(const char Text[], const char fontname[],MaterialPtr Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
:SGLText(std::auto_ptr<FTFont>(new FTExtrudeFont(SGLText::findFont(fontname))),Material,PosX,PosY,PosZ,SizeFact)
{
	renderer->FaceSize(72/25.4); // 72 is one inch 72/25.4 is one mm
	renderer->Depth(0.2);
	FrontFace=sglGeti(GL_FRONT_FACE); //ftgl gets the fron face from OpenGL, so we do the same
	myText=Text;
}


SGL3DText::~SGL3DText(){}

void SGL3DText::generate()
{
	const FTPoint &offset=box().Lower();
	const FTPoint center=offset+(box().Upper()-offset)*.5;
	renderer->Render(myText.c_str(),-1,FTPoint()-center);
}

/*!
    \fn SGL3DText::getBounds(SGLQuader *BoundingQuader)
 */
void SGL3DText::getBounds(SGLshPtr<SGLQuader> BoundingQuader)
{
	GLdouble breite,hoehe,depth;
	assert(BoundingQuader);

	getDim(&breite,&hoehe,&depth,NULL);
	BoundingQuader->tiefe=depth;
	BoundingQuader->hoehe=hoehe;
	BoundingQuader->breite=breite;
	BoundingQuader->recalcEdges(false);
	BoundingQuader->ResetTransformMatrix(MyTransformMatrix);

	BoundingQuader->FaceAt=FaceAt;
	BoundingQuader->resetTexKoord();
	BoundingQuader->Compile();
	link(*BoundingQuader);
}

/*!
    \fn SGL3DText::getCenter()
 */
SGLVektor SGL3DText::getCenter()const
{
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
	if(width)*width=box().Upper().X()-box().Lower().X();
	if(height)*height=box().Upper().Y()-box().Lower().Y();
	if(depth)*depth=box().Upper().Z()-box().Lower().Z();
}
