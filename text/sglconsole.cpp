/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sglconsole.h"

#include  "backend_glf/sgltextbackend_glf.h"

SGLConsole::SGLConsole(GLdouble breite,GLdouble hoehe, char fontname[])
{
	background=new SGLQuader(NULL,breite,hoehe,.03);
	text = new SGLConsoleText(breite,hoehe, fontname);
	background->Mat->Transparenz=.5;
	background->Mat->SetColor(.3,.3,.3);
	background->IgnoreLight=true;
	IgnoreClip=true;

	compileSubObjects();
}

SGLConsole::~SGLConsole()
{
	delete background;
	delete text;
}

/*!
    \fn SGLConsoleText::getCenter()
 */
SGLVektor SGLConsole::getCenter()
{
	return background->getCenter();
}

/*!
    \fn SGLConsole::compileSubObjects
 */
void SGLConsole::compileSubObjects()
{
	Objs.clear();
	TrObjs.clear();
	Objs.push_back(text->Compile());
	TrObjs.push_back(background->Compile());
}

/*!
    \fn SGLConsole::print(string text)
 */
void SGLConsole::print(string text)
{
	if(!text.length())return;
	empty=false;
	this->text->print(text);
	this->text->Compile();
}

SGLConsoleText::SGLConsoleText(GLdouble breite,GLdouble hoehe, char fontname[])
:SGLText(fontname)
{
	Mat->SetColor(0,1,0,GL_FRONT_AND_BACK,true);
	setConsDim(breite,hoehe);
	this->breite=breite;
	this->hoehe=hoehe;
	IgnoreLight=true;
}

SGLConsoleText::~SGLConsoleText()
{
}

/*!
    \fn SGLConsoleText::generate()
 */
void SGLConsoleText::generate()
{
	Backend->generate_consoleText(myText, buffer_width, buffer_height,breite,hoehe);
}

/*!
    \fn SGLConsoleText::print(string text)
 */
void SGLConsoleText::print(string text)
{
	Backend->validateText(text);
	myText+=text;
	Compile();
}

/*!
    \fn SGLConsoleText::setConsDim(GLdouble breite, GLdouble hoehe)
 */
void SGLConsoleText::setConsDim(GLdouble breite, GLdouble hoehe)
{
	buffer_width=int(breite*9.2);
	buffer_height=int(hoehe*5.19);
}

/*!
    \fn SGLConsoleText::clear()
 */
void SGLConsoleText::clear()
{
	myText="";
	Backend->consoleClear();
}

/*!
    \fn SGLConsole::clear()
 */
void SGLConsole::clear()
{
	text->clear();
	empty=true;
}

/*!
    \fn SGLConsoleText::getCenter()
 */
SGLVektor SGLConsoleText::getCenter()
{
    //Dummy for overloading true Virtual of parent (never used)
	return SGLVektor(0,0,0);
}
