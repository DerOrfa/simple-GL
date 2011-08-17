/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLTEXTBACKEND_GLF_H
#define SGLTEXTBACKEND_GLF_H

#include "../sgltextbackend.h"

#ifdef WIN32
	#include <windows.h>								// Header File For Windows
	#include <gl\gl.h>								// Header File For The OpenGL32 Library
	#include <gl\glu.h>								// Header File For The GLu32 Library
//	#include <gl\glaux.h>								// Header File For The GLaux Library
#else
//gl.h ist für glf.h nötig
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else 
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#endif

#include "glf.h"
#include "../../sglvektor.h"

/**
@author Enrico Reimer
*/
class SGLTextBackend_glf : public SGLTextBackend
{
public:
	SGLTextBackend_glf(const char fontname[]="");
	~SGLTextBackend_glf();
	void validateText(string &text);
	bool loadFont(const char fontname[]);
	static void backendInit();
	bool isReady()const;
    void generate_3DText(
		GLdouble tiefe,
		string myText,
		GLenum VisMode,
		SGLText::TextAlign align=SGLText::left
		);
    void generate_consoleText(
		string myText,
		int buffer_width,
		int buffer_height,
		GLdouble breite,
		GLdouble hoehe
		);
	virtual void getDim(
		string myText,
		GLdouble *width=NULL,
		GLdouble *height=NULL,
		GLdouble *depth=NULL,
		SGLVektor *mitte=NULL,
		SGLText::TextAlign align=SGLText::left
		)const;
	void consoleClear();
};

#endif
