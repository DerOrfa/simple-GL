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

#define X_AXE	1
#define Y_AXE	2
#define Z_AXE	4

#include <GL/gl.h>

#ifndef GLAPI
# ifdef _WIN32
#  define GLAPI __stdcall
# else
#  define GLAPI
# endif
# define __DEFINED_GLAPI
#endif

#include <GL/glext.h>

#ifdef __DEFINED_GLAPI
# undef GLAPI
# undef __DEFINED_GLAPI
#endif

#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#include <stdarg.h>
#endif

#define SGLprintError	if(SGLshowErrors)fprintf(stderr,"simpleGL-Error %s Line %d: ",__FILE__,__LINE__);_SGLprintError
#define SGLprintWarning	if(SGLshowWarnings)fprintf(stdout,"simpleGL-Warning %s Line %d: ",__FILE__,__LINE__);_SGLprintWarning

#ifndef __OPTIMIZE__ 
#define SGLprintDebug	if(SGLshowWarnings)fprintf(stdout,"simpleGL-Warning %s Line %d: ",__FILE__,__LINE__);_SGLprintWarning
#else
#define SGLprintDebug
#endif

#define SGLprintInfo	if(SGLshowInfos)fprintf(stdout,"simpleGL-Info %s Line %d: ",__FILE__,__LINE__);_SGLprintInfo
#define SGLprintState	if(SGLshowState)fprintf(stdout,"State: ");_SGLprintState

#define SGLcheckGLError	\
{																\
	GLuint gluerr;												\
	while(gluerr= glGetError())									\
		{SGLprintError("%s [GLerror]",gluErrorString(gluerr));}	\
}

#ifdef __cplusplus
extern "C" {
#endif

void _SGLprintError(const char text[], ...);
void _SGLprintWarning(const char text[], ...);
void _SGLprintInfo(const char text[], ...);
void _SGLprintState(const char text[], ...);

void vwriteOut(FILE *out,const char text[], va_list argList);

extern short SGLshowErrors,SGLshowInfos,SGLshowWarnings,SGLshowState;

short sglChkExt(const char* name,const char *msg,unsigned short vital);

inline GLdouble sglGetd(GLenum pname)
{
	GLdouble ret;
	glGetDoublev( pname, &ret);
	return ret;
}

inline GLfloat sglGetf(GLenum pname)
{
	GLfloat ret;
	glGetFloatv( pname, &ret);
	return ret;
}

inline GLint sglGeti(GLenum pname)
{
	GLint ret;
	glGetIntegerv( pname, &ret);
	return ret;
}

#ifdef __cplusplus
};
#endif

//lt. SGI wird der Bereich 0x6000-0x7FFF nie offziell für enums verwendet werden
#define NON_GL_ENUM_BLOCK_BEGIN 0x6000
#define NON_GL_ENUM_BLOCK_END 0x7FFF

#define SGL_TEX_MODE_TINT NON_GL_ENUM_BLOCK_BEGIN+1

#endif


