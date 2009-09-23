/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLOBJBASE_H
#define SGLOBJBASE_H

#include "sglmatrixobj.h"
#include <list>
#include "sglsignal.h"
#include <climits>

class SGLObjList;

/** Basisklasse f�r Zeichenobjekte.
  * @author Enrico Reimer
  */
class SGLObjBase:public SGLMatrixObj
{
public:
	class CompilerMerker:public SGLSlot
	{
	public:
		CompilerMerker(SGLObjBase *obj);
		SGLObjBase *obj;
		void operator()() const;
	}
	/** Generierungs-Slot. Dieser Slot l�st die Neuberechnung des Zeichenobjektes aus. Das Objekt wird dabei jedoch nicht gezeichnet. */
	compileNextTime; 
	/** Zeichenpriorit�t des Objektes.*/
	enum Prio{
		/** Hintergrund (wird zuerst gezeichnet ) */ 
		floor=INT_MIN, 
		/** hinterstes Objekt (wird �ber Hintergrund gezeichnet ) */ 
		under=-10, 
		/** normales Objekt*/
		std=0,
		/** Objekt, das vor normalen Objektes gezeichnet wird (�blicherweise transparente Objekte)*/
		flstd=10,
		/** vorderstes Objekt z.B. GUI-Elemente */
		ontop=INT_MAX 
	}; 
	SGLObjBase(const SGLObjBase &src);
	SGLObjBase(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	virtual ~SGLObjBase();
	
	/**
	 * Berechnet das Objekt neu.
	 * F�hrt alle Operationen zum Zeichnen des Objektes aus und legt sie im Renderer-Cache unter seiner ID ab.
	 * Dabei werden neben den reinen Zeichenoperationen gegebenfalls auch Operationen zur Transformation des Raumes und zur Konfiguration des Renderers ausgef�hrt.
	 * @param draw wenn true, wird das Objekt au�erdem sofort gezeichnet (zus�tzlich zu dem �blichen Zeichnen, wenn die Objektliste neu gezeichnet wird)
	 * @param free wenn true, ist das Objekt "frei", d.h. es geh�rt keiner Objektliste an.
	 * @return die Renderer-Cache-ID des Objektes
	 */
	virtual GLuint Compile(bool draw=true,bool free=false)=0;
	/**
	 * Zeichnet das Objekt.
	 * F�hrt alle Operationen zum Zeichnen des Objektes aus.
	 * Es werden nur die reinen Zeichenfunktionen aufgerufen. Andere Eigenschaften des Objektes (z.B. Transparenz) und seine Transformation werden nicht ber�cksichtigt.
	 * 
	 * Beispiel f�r eine Implemetierung (Erzeugt eine Linie von [-1,-1,-1] nach [1,1,1]):
	 * \code 	glBegin(GL_LINES);
	 * SGLVektor(-1,-1,-1).DrawVertex();
	 * SGLVektor( 1, 1, 1).DrawVertex();
	 * glEnd();
	 * \endcode
	 */
	virtual void generate()=0;

	static SGLVektor Normale(SGLVektor Vekt1,SGLVektor Vekt2);
	static SGLVektor Normale(SGLVektor Pkt1,SGLVektor Pkt2,SGLVektor Pkt3);

	GLuint ID;
	bool IgnoreClip,IgnoreLight,shared,is_free;
	GLenum FrontFace;
	SGLVektor* FaceAt;
	void metaGenerate();

	/**
	 * Liefert das Zentrum des Objektes innerhalb seines eigenen Raumes.
	 * Diese Funktion ist unabh�ngig von der Transformation (also z.B. der Position) des Objektes.
	 * Das Zentrum eines Objektes liegt meist im Ursprung seines Raumes, daher geben fast alle Implementierungen SGLVektor(0,0,0) zur�ck.
	 * @return der Positionsvektor des Objektzentrums in seinem eigenen Raum
	 */
	virtual SGLVektor getCenter()const = 0;

	SGLVektor getCenterInSpace();
	SGLVektor getMyPos();
	GLuint metaCompile(bool force_compile=false);
	SGLConnection link(SGLObjBase &obj);
	void unlink(SGLConnection conn);
	SGLSignal<void ()> notifyChange;
	GLint beginList(bool draw);
	void endList();
	Prio priority;
	short should_compile;
	SGLObjList* myList;
};
typedef SGLshPtr<SGLObjBase> shared_obj;

#include "helper/sgldisplist.h"

#endif
