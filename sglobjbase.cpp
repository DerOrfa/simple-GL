/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sglobjbase.h"
#include "sglmisc.h"
#include <typeinfo> 

SGLObjBase::SGLObjBase(const SGLObjBase &src)
{
	ID=glGenLists(1);
	changeReciver = src.changeReciver;
	changeSender = src.changeSender;
	FaceAt = src.FaceAt;
	FrontFace = src.FrontFace;
	IgnoreClip = src.IgnoreClip;
	IgnoreLight = src.IgnoreLight;
	myList = src.myList;
	ResetTransformMatrix(src.MyTransformMatrix);
	priority = src.priority;
	should_compile = true;
}

SGLObjBase::SGLObjBase(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact)
{
	ID=glGenLists(1);
	ResetTransformMatrix();
	if(PosX || PosY || PosZ)Move(PosX,PosY,PosZ);
	if(SizeFact!=1)Scale(SizeFact,SizeFact,SizeFact);
	IgnoreClip=IgnoreLight=false;
	FrontFace=GL_CCW;
	FaceAt=NULL;
	myList=NULL;
	should_compile=1;
}
SGLObjBase::~SGLObjBase()
{
	list<SGLObjBase*>::iterator i=changeReciver.begin();
	list<SGLObjBase*>::size_type len=changeReciver.size();
	while(len--)//lt. STL-Standart sollten Iteratoren trotz remove gültig bleiben
		unlink(*i++);

	i=changeSender.begin();
	len=changeSender.size();
	while(len--)
		(*i++)->unlink(this);

	glDeleteLists(ID,1);
}


SGLVektor SGLObjBase::Normale(SGLVektor Vekt1,SGLVektor Vekt2)
{
	SGLVektor ret=Vekt1.kreuzprod(Vekt2);
	if(!glIsEnabled(GL_NORMALIZE))ret.Normalize();
	return ret;
}

SGLVektor SGLObjBase::Normale(SGLVektor Pkt1,SGLVektor Pkt2,SGLVektor Pkt3)
{
	return Normale(SGLVektor(Pkt2-Pkt1),SGLVektor(Pkt3-Pkt1));
}

void SGLObjBase::MoveTo(SGLVektor to){MoveTo(to.SGLV_X,to.SGLV_Y,to.SGLV_Z);}
void SGLObjBase::Move(SGLVektor to){Move(to.SGLV_X,to.SGLV_Y,to.SGLV_Z);}

void SGLObjBase::MoveTo(GLdouble x,GLdouble y, GLdouble z)
{
	ResetTransformMatrix(NULL);
	Move(x,y,z);
}

void SGLObjBase::Move(GLdouble x,GLdouble y, GLdouble z)
{
	glPushMatrix();
	glLoadMatrixd(MyTransformMatrix);
	glTranslated(x,y,z);
	glGetDoublev(GL_MODELVIEW_MATRIX,MyTransformMatrix);
	glPopMatrix();
}

void SGLObjBase::Scale(GLdouble xfact,GLdouble yfact, GLdouble zfact)
{
	glPushMatrix();
	glLoadMatrixd(MyTransformMatrix);
	glScaled(xfact,yfact,zfact);
	glGetDoublev(GL_MODELVIEW_MATRIX,MyTransformMatrix);

	glPopMatrix();
}
void SGLObjBase::Rotate(GLdouble xfact,GLdouble yfact, GLdouble zfact, GLdouble amount)
{
 	glPushMatrix();
	glLoadMatrixd(MyTransformMatrix);
	glRotated(amount,xfact,yfact,zfact);
	glGetDoublev(GL_MODELVIEW_MATRIX,MyTransformMatrix);
	glPopMatrix();
}

void SGLObjBase::ResetTransformMatrix(const GLdouble *newMatrix)
{
	for(int i=0;i<16;i++)
	{
		if(newMatrix)
		{
			MyTransformMatrix[i]=newMatrix[i];
		}
		else
		{
			if(i%5)MyTransformMatrix[i]=0;
			else MyTransformMatrix[i]=1;
		}
	}
}

void SGLObjBase::Scale(GLdouble fact)
{Scale(fact,fact,fact);}

/*!
    \fn SGLObjBase::metaGenerate()
 */
void SGLObjBase::metaGenerate()
{
	glPushMatrix();
	glMultMatrixd(MyTransformMatrix);
	if(FaceAt)
	{
		GLdouble XZ_wink,XY_wink;
		SGLVektor tVekt=getCenterInSpace();
		tVekt.resize(3);
		//Für toWink darf der Vektor nur 3 El haben (sonst geht toWink von nem 4Dim Raum aus)
		tVekt=(*FaceAt)-tVekt;
		tVekt.toWink(XZ_wink,XY_wink);
		glRotated(-XZ_wink,0,1,0);
		glRotated(90,0,1,0);
		glRotated(-XY_wink,1,0,0);
	}
//	SGLprintInfo("Generiere \"%s\" Prio: %d ID: %d",guesType(),priority,ID);
	generate();
	glPopMatrix();
}

/*!
    \fn SGLObjBase::getCenterInSpace()
 */
SGLVektor SGLObjBase::getCenterInSpace()
{
	SGLVektor ret=getCenter();
	ret.resize(4);
	ret[3]=1;
	EMatrix<GLdouble> Matr(4,4);
	Matr.fromArray(4,4,MyTransformMatrix);
	Matr=Matr.transp();
	ret=SGLVektor(Matr*(ret));
	return ret;
}


/*!
    \fn SGLObjBase::getMyPos()
 */
SGLVektor SGLObjBase::getMyPos()
{
	EVektor<GLdouble> ret(4);
	EMatrix<GLdouble> tMatr(4,4);
	ret.resize(4);ret[3]=1;
	tMatr.fromArray(4,4,MyTransformMatrix);
	tMatr=tMatr.transp();
	ret=SGLVektor(tMatr*(ret));
	ret.resize(3);
	return ret;
}


/*!
	Stellt sicher, das eine CallListe für dieses Objekt in der OpenGL-Pipeline
	vorliegt, und liefert sie. ES WIRD ABER NICHT GEZEICHNET
    \fn SGLObjBase::metaCompile(bool force_compile=false)
 */
GLuint SGLObjBase::metaCompile(bool force_compile)
{
	GLuint ret;
	ret = (force_compile || should_compile || !glIsList(ID)) ?
		Compile():
		ID;

	if(should_compile>0)should_compile=0;
	return ret;
}


/*!
	Stellt sicher, daß das Objekt bei nächster Gelegenheit mindestens einmal Kompiliert wird
    \fn SGLObjBase::compileNextTime
 */
void SGLObjBase::compileNextTime()
{
	if(myList)myList->check_recompile=true;
	if(!should_compile)should_compile=1;
}


/*!
    \fn SGLObjBase::link(SGLObjBase *obj)
 */
void SGLObjBase::link(SGLObjBase *obj)
{
	//Der unwarscheinliche Fall, das obj mehrfach eingetr. wird ist nich weiter schlimm
	//jedesmal die liste durchsuchen wäre "schlimmer"
	changeReciver.push_back(obj);
	obj->changeSender.push_back(this);
}

void SGLObjBase::unlink(SGLObjBase *obj)
{
	//Hoffentlich flennt der nicht rum, wenn obj nich da ist
	changeReciver.remove(obj);
	obj->changeSender.remove(this);
}


/*!
    \fn SGLObjBase::notifyChange()
 */
void SGLObjBase::notifyChange()
{
	list<SGLObjBase*>::iterator i=changeReciver.begin();
	list<SGLObjBase*>::size_type len=changeReciver.size();
	while(len--)
		(*i++)->compileNextTime();
}


/*!
    \fn SGLObjBase::guesType()
 */
const char*  SGLObjBase::guesType()
{
#ifndef WIN32
	return typeid(*this).name();
#else
	return "VC60 kann kein typeid??";
#endif
}
