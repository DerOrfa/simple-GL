#include "sglquadricobj.h"

SGLQuadricObj::SGLQuadricObj(MaterialPtr Material,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
  :SGLFlObj(Material,PosX,PosY,PosZ,SizeFact)
{
	quadric=gluNewQuadric();
}

SGLQuadricObj::~SGLQuadricObj()
{
	gluDeleteQuadric(quadric);
}

SGLVektor SGLQuadricObj::getCenter()
{return SGLVektor(0,0,0);}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SGLSphere::SGLSphere(MaterialPtr Material,GLint slices,GLint stacks,GLdouble Size) :SGLQuadricObj(Material)
{
	this->slices= slices ? slices:GLint(Size*15.)+10;
	this->stacks= stacks ? stacks:GLint(Size*15.)+10;
	size=Size;
}

SGLSphere::~SGLSphere()
{}

void SGLSphere::generate()
{
	gluSphere(quadric,size,slices,stacks);
}
