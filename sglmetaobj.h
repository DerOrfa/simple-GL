/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLMETAOBJ_H
#define SGLMETAOBJ_H

#include "sglobj.h"
#include <vector>

/*! \brief Abstrakte Basisklasse für Objekte, die sich aus mehreren Objekten beliebiger Art zusammensetzen.

Die Funktion, die Teilobjekte compiliert, und in ihre Aufruflisten legt, muss noch von der Ableitung implementiert werden.
Der Aufruf der Teilobjekte aus diesen Listen wird vom generate von SGLMetaObj erledigt.
Es gibt eine Liste für transparente Objekte, und eine für nicht transparente Objekte.

@author Enrico Reimer
*/
class SGLMetaObj : public SGLObj
{
public:
    SGLMetaObj(GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);

    ~SGLMetaObj();

    vector<GLint> Objs, TrObjs;

    virtual void compileSubObjects() = 0;

    public: void generate();
};

#endif
