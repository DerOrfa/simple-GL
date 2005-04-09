/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLTEXTBACKEND_H
#define SGLTEXTBACKEND_H

#include <string>
#include "sgltext.h"

/**
@author Enrico Reimer
*/
class SGLTextBackend
{
protected:
	int myFontID;
public:
	virtual ~SGLTextBackend();
	void replace(string &myText,string findText,string replString);
	virtual bool loadFont(char fontname[])=0;
	virtual void generate_3DText(
		GLdouble tiefe,
		string myText,
		GLenum VisMode,
		SGLText::TextAlign align=SGLText::left
		)=0;
	virtual void generate_consoleText(
		string myText,
		int buffer_width,
		int buffer_height,
		GLdouble breite,
		GLdouble hoehe
		)=0;
    virtual void validateText(string &text) = 0;
	virtual void getDim(
		string myText,
		GLdouble *width=NULL,
		GLdouble *height=NULL,
		GLdouble *depth=NULL,
		SGLVektor *mitte=NULL,
		SGLText::TextAlign align=SGLText::left
		)const=0;
	virtual void consoleClear()=0;

};

#endif
