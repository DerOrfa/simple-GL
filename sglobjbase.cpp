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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

using boost::numeric::ublas::matrix;

/**
 * Kopierkonstruktor.
 * Alle Parameter bis auf is_free und shared werden kopiert.
 * Das kopierte Objekt ist nocht nicht im Renderercache und gehört zu keiner Liste.
 * Sein compileNextTime-Slot muss noch an ein Signal gebunden werden.
 * @param src Das zu kopierende Objekt
 */
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

/**
 * Standardkonstruktor.
 * Erzeugt ein Objekt an den gegebenen Raumkoordinaten.
 * @param PosX die Raumkoordinaten, an denen das Objekt erz. wird
 * @param PosY
 * @param PosZ
 * @param SizeFact Die Grundskalierung, mit der das Objekt erzeugt wird.
 */
SGLObjBase::SGLObjBase(GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLMatrixObj(GL_MODELVIEW),
compileNextTime(this)
{
	ID=0;
	ResetTransformMatrix(NULL);
	MyTransformMatrix[12]=PosX;
	MyTransformMatrix[13]=PosY;
	MyTransformMatrix[14]=PosZ;

	MyTransformMatrix[0]=MyTransformMatrix[5]=MyTransformMatrix[10]=SizeFact;

	IgnoreClip=IgnoreLight=false;
	FrontFace=GL_CCW;
	FaceAt=NULL;
	myList=NULL;
	should_compile=1;
	shared=false;
}


/**
 * Destruktor.
 * Sollte nicht explizit ausgelöst werden.
 * Das Objekt wird automatisch gelöscht, wenn es nicht mehr verwedet wird.
 * Ist das gelöschte Objekt noch in einer Objektliste eingetragen, wird eine Warnung ausgegeben.
 */
SGLObjBase::~SGLObjBase()
{
	if(myList)
	{
		unsigned int count=myList->isThere(this);
		if(count){SGLprintError("lösche noch %d mal verwendetes Objekt",count);}
	}
	glDeleteLists(ID,1);
	if(!ID)
	{SGLprintDebug("Nie generiertes Objekt 0x%x (%s) gelöscht",this,this->guesType());}//@todo prüfen, ob ID=0 immer heißt, daß obj nie gen wurde
}


/**
 * Bestimmt die Normale auf der durch zwei Vektoren bestimmten Ebene.
 * Die Ebene schneidet den Koordinatenursprung.
 * @param Vekt1 die zwei Vektoren zur beschreibung der Ebene
 * @param Vekt2
 * @return einen Vektor, der senkrecht auf der Ebene steht
 */
SGLVektor SGLObjBase::Normale(SGLVektor Vekt1,SGLVektor Vekt2)
{
	SGLVektor ret=Vekt1.cross_prod(Vekt2);
	if(!glIsEnabled(GL_NORMALIZE))
		ret/=boost::numeric::ublas::norm_2(ret);
	return ret;
}

/**
 * Bestimmt die Normale auf der durch zwei Vektoren und einen Stützvektor bestimmten Ebene.
 * Die Ebene schneidet den durch den Stützvektor gegebenen Punkt im Raum.
 * @param Pkt1 @param Pkt2 die zwei Vektoren zur beschreibung der Ebene
 * @param Pkt3 der Stützvektor der Ebene
 * @return
 */
SGLVektor SGLObjBase::Normale(SGLVektor Pkt1,SGLVektor Pkt2,SGLVektor Pkt3)
{return Normale(SGLVektor(Pkt2-Pkt1),SGLVektor(Pkt3-Pkt1));}

/**
 * Basis-Zeichenfunktion.
 * Lädt die Transformationsmatrix des Objektes, richtet es danach gegebenfalls nach dem FaceAt-Punkt aus und ruft generate() auf.
 */
void SGLObjBase::metaGenerate()
{
	loadMatrix();
	if(FaceAt)
	{
		glMatrixMode(GL_MODELVIEW);
		GLdouble ebene_wink,steigung;
		SGLVektor tVekt=getCenterInSpace();
		tVekt.resize(3);
		tVekt=*FaceAt-tVekt;
		tVekt.toWink(ebene_wink,steigung);

		glRotated(-ebene_wink,0,1,0);
		glRotated(90,0,1,0);
		glRotated(-steigung,1,0,0);
	}

	GLenum error;
	while((error=glGetError()))
	{
		SGLprintError("%s [GLerror] vor dem Generieren von %s",gluErrorString(GLenum(error)),guesType());
	}
	generate();
	while((error=glGetError()))
	{
		SGLprintError("%s [GLerror] beim Generieren von %s",gluErrorString(GLenum(error)),guesType());
	}

	unloadMatrix();
	notifyChange();
}

/**
 * Wendet die Transformationen des Objektes auf das Ergebniss von getCenter() an und gibt das Ergebniss zurück.
 * @return die Position des Objektzentrums im Raum.
 */
SGLVektor SGLObjBase::getCenterInSpace()
{
	dvector ret=getCenter();
	ret.resize(4);
	ret[3]=1;
	matrix<GLdouble,boost::numeric::ublas::column_major> Matr(4,4);
	std::copy(MyTransformMatrix,MyTransformMatrix+16,Matr.data().begin());
	
	ret=boost::numeric::ublas::prod(Matr,ret);
	ret.resize(3);
	return ret;
}


/**
 * Wendet die Transformationen des Objektes auf einen Nullvektor an und gibt das Ergebniss zurück.
 * @return die Position des Objektes im Raum.
 */
SGLVektor SGLObjBase::getMyPos()
{
	dvector ret(4);
	dmatrix tMatr(4,4);
	ret[3]=1;
	std::copy(MyTransformMatrix,MyTransformMatrix+16,tMatr.data().begin());
	ret=boost::numeric::ublas::prod(tMatr,ret);
	ret.resize(3);
	return ret;
}


/**
 * Stellt sicher, das dieses Objekt im Renderer-Cache vorliegt und liefert seine ID.
 * Wenn force_compile war ist, das Objekt noch nicht in der OpenGL-Pipeline vorliegt
 * oder wenn es neu berechnet werden muss, wird Compile() aufgerufen.
 * Das Objekt wird dabei jedoch nicht gezeichnet.
 * @param force_compile wenn true wird das Objekt in jedem Fall neu generiert.
 * @return die ID, die das Objekt im Cache des Renderers hat. Diese ID ist innerhalb eines Renderers eindeutig.
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

/**
 * Bindet das Objekt an ein Anderes.
 * Löst dieses Objekt das Signal notifyChange aus, wird das gebundene Objekt bei nächster Gelegenheit neu berechnet.
 * Da beim Generieren notifyChange nicht ausgelöst wird, ist auch gegenseitiges Binden möglich.
 * \code
 * objA.link(objB);
 * objB.link(objA);
 * \endcode
 * @param obj das Zeilobjekt an das bei notifyChange neu berechnet werden soll.
 * @return
 */
SGLConnection SGLObjBase::link(SGLObjBase &obj)
{
	return notifyChange.connect(obj.compileNextTime);
}


void SGLObjBase::unlink(SGLConnection conn)
{
	conn.disconnect();
}


/**
 * Bereitet den Renderercache für das Objekt vor.
 * Hat das Objekt noch keine ID, wird ihm eine zugewiesen.
 * @param draw Wenn true, wird gleichzeitig in den Renderercache geschrieben und das Objekt gezeichnet.
 * @return die ID des Objektes.
 */
GLint SGLObjBase::beginList(bool draw)
{
	if(!(glIsList(ID) || (ID=glGenLists(1))))
	{
	    GLuint error=glGetError();
	    SGLprintError("Konnte Zeichencache für das %s-Objekt nicht erzeugen. (%s) Wurde openGL vielleicht noch nicht initialisiert?",guesType(),gluErrorString(GLenum(error)));
	}
	rendering=true;
	glNewList(ID,draw ? GL_COMPILE_AND_EXECUTE:GL_COMPILE);
	return ID;
}

/**
 * Schließt den Renderercache für das Objekt ab.
 * Alle GL-Zeichenoperationen müssen zwischen beginList() und endList aufgerufen werden.
 */
void SGLObjBase::endList()
{
	glEndList();
	rendering=false;
}


SGLObjBase::CompilerMerker::CompilerMerker(SGLObjBase *obj){this->obj=obj;}

/**
 * Stellt sicher, daß der Renderercache dieses Objektes bei nächster Gelegenheit neu berechnet wird
 */
void SGLObjBase::CompilerMerker::operator()() const
{
	if(obj->myList)
		obj->myList->check_recompile=true;
	else if(!obj->is_free)
		{SGLprintDebug("Objekt vom Typ %s ist scheinbar in (noch) keiner Objektliste.\nKann nicht vermerken, daß es neu generiert werden muß", obj->guesType());}
	if(!obj->should_compile)obj->should_compile=1;
}
