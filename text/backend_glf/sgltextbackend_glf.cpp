/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sgltextbackend_glf.h"
#include <string.h>
#include <stdio.h>
#include "../../sglmisc.h"

bool glf_inited=false;

SGLTextBackend_glf::SGLTextBackend_glf(char fontname[])
{
	if(!glf_inited)SGLTextBackend_glf::backendInit();
	myFontID=-1;
	if(strlen(fontname))loadFont(fontname);
	#ifdef DATA_DIR
	else loadFont(DATA_DIR "/fonts/arial1.glf");
	#else
		#warning DATA_DIR ist nicht def.
	else loadFont("/opt/prog/simpleGL/libsgl/data/fonts/arial1.glf");
	#endif
}

SGLTextBackend_glf::~SGLTextBackend_glf()
{
	glfUnloadFontD(myFontID);
}

bool SGLTextBackend_glf::loadFont(char fontname[])
{
  if((myFontID=glfLoadFont(fontname))==GLF_ERROR) 
    {
      SGLprintError("Konnte Schriftdatei \"%s\" nicht laden",fontname);
      myFontID=-1;
      return false;
    }
  else return true;
}

bool  SGLTextBackend_glf::isReady()
{
	if(!glf_inited || myFontID==-1)
	{
		SGLprintError("Das glf-Backend ist nicht funktionsbereit");
		return false;
	}
	else return true;
}


/*!
    \fn SGLTextBackend_glf::validateText(string &text)
 */
void SGLTextBackend_glf::validateText(string &text)
{
	replace(text,"Ö","Oe");
	replace(text,"Ä","Ae");
	replace(text,"Ü","Ue");

	replace(text,"ö","oe");
	replace(text,"ä","ae");
	replace(text,"ü","ue");

	replace(text,"ß","ss");
}


/*!
    \fn SGLTextBackend_glf::backendInit()
 */
void SGLTextBackend_glf::backendInit()
{
	glfInit();
	glf_inited=true;
}


/*!
    \fn SGLTextBackend_glf::generate_3DText(GLdouble tiefe,string myText,GLenum VisMode,SGLText::TextAlign align)
 */
void SGLTextBackend_glf::generate_3DText(GLdouble tiefe,string myText,GLenum VisMode,SGLText::TextAlign align)
{
	if(!isReady())return;

	int error;
	int len=myText.size();

	glfSetCurrentFont(myFontID);
	glfSetSymbolDepth(tiefe);

	glFrontFace(GL_CW);

	if(len>1)
	{
		SGLVektor moveVekt;
		GLdouble width;
		char *tmp=new char[len+1];

		strcpy(tmp,myText.c_str());
		getDim(myText,&width,NULL,&tiefe,&moveVekt,align);
		cout << moveVekt << endl;
		moveVekt=-moveVekt;
		glPushMatrix();
		glTranslated(moveVekt.SGLV_X,moveVekt.SGLV_Y,moveVekt.SGLV_Z);
		if(VisMode==GL_FILL)
		{
			if(tiefe)glfDraw3DSolidString(tmp);
			else glfDrawSolidString(tmp);
		}
		else
		{
			if(tiefe)glfDraw3DWiredString(tmp);
			else glfDrawWiredString(tmp);
		}
		delete tmp;
		glPopMatrix();
	}
	else if(len==1)
	{
		if(VisMode==GL_FILL)glfDraw3DSolidSymbol(myText[0]);
		else glfDraw3DWiredSymbol(myText[0]);
	}
	glFrontFace(GL_CCW);
}

/*!
    \fn SGLTextBackend_glf::generate_consoleText(
	string myText,
	int buffer_width,
	int buffer_height,
	GLdouble breite,
	GLdouble hoehe
	)
 */
void SGLTextBackend_glf::generate_consoleText(
	string myText,
	int buffer_width,
	int buffer_height,
	GLdouble breite,
	GLdouble hoehe
	)
{
	if(!isReady())return;
	int len=myText.length();
	char *t=new char[len+1];

	glfSetConsoleParam(buffer_width, buffer_height);
	glfDisable(GLF_CONSOLE_CURSOR);
	glfConsoleClear();

	strcpy(t,myText.c_str());
	glfPrint(t,len);

	glPushMatrix();
	glTranslated(-breite/2,hoehe/2,0);
	glScaled(1./10,1./10,1./10);
	glTranslated(1.2,-1,0);

	glfConsoleDraw();
	glPopMatrix();

	delete t;
}

/*!
    \fn SGLTextBackend_glf::getDim(string myText,GLdouble *width,GLdouble *height,GLdouble *depth,SGLVektor *mitte,SGLText::TextAlign align)
 */
void SGLTextBackend_glf::getDim(string myText,GLdouble *width,GLdouble *height,GLdouble *depth,SGLVektor *mitte,SGLText::TextAlign align)
{
	if(!isReady())return;
	float minx, miny, maxx, maxy;
	char temp[255];
	strcpy(temp,myText.c_str());
	glfGetStringBoundsF(myFontID,temp, &minx, &miny, &maxx, &maxy);
	GLfloat underline_correction=.3;//für Buchstaben die unter die Basislinie gehen
	minx-=.1;

	float int_width=maxx-minx;
	float int_height=maxy-miny+underline_correction;

	int_width*=.91;

	if(width)*width=fabs(int_width);
	if(height)*height=fabs(int_height);
	if(mitte)
	{
		mitte->SGLV_Y=int_height/2+miny;
		if(depth)mitte->SGLV_Z=*depth/2+1;
		else {SGLprintWarning("SGLTextBackend_glf::getDim wurde, bei der Berechnung des Mittelpunktes keine Tiefe übergeben");}
		switch(align)
		{
		case SGLText::center:mitte->SGLV_X=int_width/2+minx+.3;break;
		case SGLText::left:mitte->SGLV_X=minx+.3;break;
		case SGLText::right:mitte->SGLV_X=minx+.3+int_width;break;
		}
		if(myText.size()>1)mitte->SGLV_X-=1;
		else mitte->SGLV_X-=.3;
	}
}


/*!
    \fn SGLTextBackend_glf::consoleClear()
 */
void SGLTextBackend_glf::consoleClear()
{
	glfConsoleClear();
}
