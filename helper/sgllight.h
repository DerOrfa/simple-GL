/***************************************************************************
                          sgllight.h  -  description
                             -------------------
    begin                : Fri Oct 5 2001
    copyright            : (C) 2001 by Enrico Reimer
    email                : enni_@T-Online.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SGLLIGHT_H
#define SGLLIGHT_H

#include "../sglobj.h"

struct LichtFarbe{GLfloat Umgebung[4],Difus[4],Glanz[4];};

/**
 *@author Enrico Reimer
 */
class SGLLight : public SGLObj
{
public:
	SGLLight(int LID=-1,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0,GLdouble SizeFact=1);
	GLenum LightID;
	bool On(bool on=true);
	bool IsOn();
	void generate();
	void CamLight();
	virtual GLuint Compile(bool draw=true,bool free=false);
	void SetParam(GLenum pname,GLfloat param);
	void SetParam(GLenum pname,const GLfloat *params);
	void SetUp();
	LichtFarbe Farbe;
	struct
	#if defined WIN32
		noname1
	#endif //grrr.... ANSI-C Ihr M$- Dumpfbacken
	{SGLVektor Richtung;GLfloat Ausleuchtung,Winkel;}Spot;
	struct
	#if defined WIN32
		noname2
	#endif //grrr.... ANSI-C Ihr M$- Dumpfbacken
	{GLfloat Linear,Quadratisch;}Abnahme;
	bool IsSpot;

	SGLVektor getCenter()const;
	bool isVisible();
	void setVisible(bool visible=true);
	LichtFarbe getFarbeAt(SGLVektor vekt);
	void SetColor(GLfloat r,GLfloat g,GLfloat b);
	bool visible;
};

#endif
