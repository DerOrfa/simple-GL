/***************************************************************************
                          sglmaterial.h  -  description
                             -------------------
    begin                : Sun Oct 14 2001
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

#ifndef SGLMATERIAL_H
#define SGLMATERIAL_H

#ifdef WIN32
	#include <windows.h>								// Header File For Windows
	#include <gl\gl.h>								// Header File For The OpenGL32 Library
	#include <gl\glu.h>								// Header File For The GLu32 Library
	#include <gl\glaux.h>								// Header File For The GLaux Library
#else
	#include <GL/gl.h>
#endif

#include "sgltextur.h"

/**
  *@author Enrico Reimer
  */
class SGLMaterial
{
public:
	SGLMaterial();
	SGLMaterial(GLfloat R,GLfloat G, GLfloat B,GLenum Face=GL_FRONT_AND_BACK,bool selbstLeuchtend=false);
	SGLMaterial(const char *imageFile);
	~SGLMaterial();
	void loadMat();
	void unloadMat();
	void SetColor(GLfloat R,GLfloat G, GLfloat B,GLenum Face=GL_FRONT_AND_BACK,bool selbstLeuchtend=false);
	bool SetTex(const char *imageFile);
	bool SetTex(SGLTextur *TexPtr);
	struct faerbung
	{
		struct{GLfloat Glanz[3],Glow[3],Difus[3],Umgebung[3];}Farbe;
		GLfloat	GlanzFaktor;
	}Innen,Aussen;
	bool isMyTex;
	SGLTextur* tex;
	GLfloat Transparenz;
	bool InnenGleichAussen,UmgebGleichDifus;
	void MergeColor(int r, int g, int b, bool doGlanz=false);
    void resetMat(faerbung &f);
    void setMatParams(GLenum FACE,faerbung &f);
	private: // Private methods
	void SetMat(GLfloat col[3],GLfloat Transp,GLenum Face=GL_FRONT,GLenum ColorID=GL_SHININESS);
};

#endif
