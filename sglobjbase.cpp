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
#include <GL/glu.h>

SGLObjBase::SGLObjBase(const SGLObjBase &src):
SGLMatrixObj(GL_MODELVIEW),
compileNextTime(this)
{
	ID=0;
	FaceAt = src.FaceAt;
	FrontFace = src.FrontFace;
	IgnoreClip = src.IgnoreClip;
	IgnoreLight = src.IgnoreLight;
	myList = src.myList;
	ResetTransformMatrix(src.MyTransformMatrix);
	priority = src.priority;
	should_compile = 1;
	is_free=shared=false;
}

SGLObjBase::SGLObjBase(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLMatrixObj(GL_MODELVIEW),
compileNextTime(this)
{
	ID=0;
	ResetTransformMatrix();
	if(PosX || PosY || PosZ)Move(PosX,PosY,PosZ);
	if(SizeFact!=1)Scale(SizeFact,SizeFact,SizeFact);
	IgnoreClip=IgnoreLight=false;
	FrontFace=GL_CCW;
	FaceAt=NULL;
	myList=NULL;
	should_compile=1;
	shared=false;
}
SGLObjBase::~SGLObjBase()
{

	if(myList)
	{
		cout << myList->Objects.size() << endl;
		unsigned int count=myList->isThere(this);
		if(count){SGLprintError("lösche noch %d mal verwendetes Objekt",count);}
	}
	glDeleteLists(ID,1);
/*	if(!ID)
	{SGLprintInfo("Nie generiertes Objekt 0x%x gelöscht",this);}*/
}


SGLVektor SGLObjBase::Normale(SGLVektor Vekt1,SGLVektor Vekt2)
{
	SGLVektor ret=Vekt1.kreuzprod(Vekt2);
	if(!glIsEnabled(GL_NORMALIZE))ret.Normalize();
	return ret;
}

SGLVektor SGLObjBase::Normale(SGLVektor Pkt1,SGLVektor Pkt2,SGLVektor Pkt3)
{return Normale(SGLVektor(Pkt2-Pkt1),SGLVektor(Pkt3-Pkt1));}

/*!
    \fn SGLObjBase::metaGenerate()
 */
void SGLObjBase::metaGenerate()
{
	GLenum error;
	
	loadMatrix();
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

	generate();

	unloadMatrix();
	notifyChange();
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
    \fn SGLObjBase::link(SGLObjBase *obj)
 */
connection SGLObjBase::link(SGLObjBase &obj)
{
	return notifyChange.connect(obj.compileNextTime);
}

void SGLObjBase::unlink(connection conn)
{
	conn.disconnect();
}

GLint SGLObjBase::beginList(bool draw)
{
	if(!(glIsList(ID) || (ID=glGenLists(1))))
	{SGLprintError("Konnte keine Displayliste für das %s-Objekt erzeugen. Wurde openGL vielleicht noch nicht initialisiert?",guesType());}
	rendering=true;
	glNewList(ID,draw ? GL_COMPILE_AND_EXECUTE:GL_COMPILE);
}
void SGLObjBase::endList()
{
	glEndList();
	rendering=false;
}


SGLObjBase::CompilerMerker::CompilerMerker(SGLObjBase *obj){this->obj=obj;}

/*!
	Stellt sicher, daß das Objekt bei nächster Gelegenheit Kompiliert wird
    \fn SGLObjBase::notifyChange()
 */
void SGLObjBase::CompilerMerker::operator()() const
{
	if(obj->myList)
		obj->myList->check_recompile=true;
	else if(!obj->is_free)
		{SGLprintDebug("Objekt vom Typ %s ist scheinbar in (noch) keiner Objektliste.\nKann nicht vermerken, daß es neu generiert werden muß", obj->guesType());}
	if(!obj->should_compile)obj->should_compile=1;
}
