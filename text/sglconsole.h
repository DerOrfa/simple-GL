/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLCONSOLE_H
#define SGLCONSOLE_H

#include "../primitives/sglquader.h"
#include "sgltext.h"
#include "../sglmetaobj.h"

/**
@author Enrico Reimer
*/
class SGLConsoleText : public SGLText
{
public:
	SGLConsoleText(GLdouble breite,GLdouble hoehe, char fontname[]="text/fonts/courier1.glf");
	~SGLConsoleText();
	void generate();
	void print(string text);
	void setConsDim(GLdouble breite, GLdouble hoehe);
	void clear();
    SGLVektor getCenter();

private:
	int buffer_width, buffer_height;
	GLdouble breite,hoehe;
};

/**
  *@author Enrico Reimer
  */
class SGLConsole : public SGLMetaObj
{
public:
	SGLConsole(GLdouble breite,GLdouble hoehe, char fontname[]="");
	~SGLConsole();
	SGLVektor getCenter();
	SGLQuader *background;
	SGLConsoleText *text;

    void compileSubObjects();
    void print(string text);

    public: void clear();
    bool empty;
};

#endif
