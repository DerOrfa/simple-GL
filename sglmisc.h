/***************************************************************************
                          misc.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
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

#ifndef SGLMISC_H
#define SGLMISC_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>

#define X_AXE	1
#define Y_AXE	2
#define Z_AXE	4

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifndef GLAPI
# ifdef _WIN32
#  define GLAPI __stdcall
# else
#  define GLAPI
# endif
# define __DEFINED_GLAPI
#endif

#ifdef __APPLE__
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif


#ifdef __DEFINED_GLAPI
# undef GLAPI
# undef __DEFINED_GLAPI
#endif

#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#define SGLprintError(...)	if(SGLshowErrors)_SGLprintError(__FILE__,__LINE__,__VA_ARGS__)
#define SGLprintWarning(...)	if(SGLshowWarnings)_SGLprintWarning(__FILE__,__LINE__,__VA_ARGS__)

#ifndef __OPTIMIZE__
#define SGLprintDebug(...)	if(SGLshowWarnings)_SGLprintWarning(__FILE__,__LINE__,__VA_ARGS__)
#else
#define SGLprintDebug(...)	if(0)_SGLprintWarning(__FILE__,__LINE__,__VA_ARGS__)
#endif

#define SGLprintInfo(...)	if(SGLshowInfos)_SGLprintInfo(__FILE__,__LINE__,__VA_ARGS__)
#define SGLprintState(...)	if(SGLshowState)_SGLprintState(__FILE__,__LINE__,__VA_ARGS__)

#define SGLcheckGLError																				\
{																									\
	if(rendering){SGLprintDebug("glGetXX-Aufrufe zwischen glBegin und glEnd sind nicht zulässig");}	\
	GLuint gluerr;																					\
	while(gluerr= glGetError())																		\
		{SGLprintError("%s [GLerror]",gluErrorString(gluerr));}										\
}

#ifdef __cplusplus
extern "C" {
#endif

void debugSig();
void _SGLprintError(const char file[],int line,const char text[], ...);
void _SGLprintWarning(const char file[],int line,const char text[], ...);
void _SGLprintInfo(const char file[],int line,const char text[], ...);
void _SGLprintState(const char file[],int line,const char text[], ...);

void vwriteOut(FILE *out,const char text[], va_list argList);



extern short SGLshowErrors,SGLshowInfos,SGLshowWarnings,SGLshowState;
extern short rendering;

short sglChkExt(const char* name,const char *msg,unsigned short vital);

GLdouble sglGetd(GLenum pname);
GLfloat sglGetf(GLenum pname);
GLint sglGeti(GLenum pname);


#ifdef __cplusplus
};
#endif

//lt. SGI wird der Bereich 0x6000-0x7FFF nie offziell für enums verwendet werden
#define NON_GL_ENUM_BLOCK_BEGIN 0x6000
#define NON_GL_ENUM_BLOCK_END 0x7FFF

#endif


