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

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else 
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "sgltextur.h"
//#include "sglshader.h"
#include "../sglshptr.h"

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
	bool SetTex(SGLshPtr<SGLBaseTex> TexPtr);
//	bool SetShader(SGLshPtr<SGLShaderProgram> ShPtr);
	struct faerbung
	{
		struct{GLfloat Glanz[3],Glow[3],Difus[3],Umgebung[3];}Farbe;
		GLfloat	GlanzFaktor;
	}Innen,Aussen;
	SGLshPtr<SGLBaseTex> tex;
//	SGLshPtr<SGLShaderProgram> shader;
	GLfloat Transparenz;
	bool InnenGleichAussen,UmgebGleichDifus;
	void MergeColor(int r, int g, int b, bool doGlanz=false);
    void resetMat(faerbung &f);
    void setMatParams(GLenum FACE,faerbung &f);
	static bool MatLoaded;
	private: // Private methods
	void SetMat(GLfloat col[3],GLfloat Transp,GLenum Face=GL_FRONT,GLenum ColorID=GL_SHININESS);
};

typedef	SGLshPtr<SGLMaterial> MaterialPtr;

#endif
