/***************************************************************************
                          misc.cpp  -  description
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

#include "sglmisc.h"
#include <stdio.h>
#include <signal.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <string.h>


short SGLshowInfos=1;
short SGLshowWarnings=1;
short SGLshowErrors=1;
short SGLshowState=1;
char *lastMsg=NULL;
short rendering=0;

void _SGLprintError(const char file[], int line, const char text[], ...)
{
	fprintf(stderr,"simpleGL-Error %s Line %d: ",file,line);
	va_list argList;
	va_start(argList,text);

	vwriteOut(stderr,text, argList);

	va_end(argList);
	debugSig();
}

void _SGLprintWarning(const char file[], int line, const char text[], ...)
{
	fprintf(stdout,"simpleGL-Warning %s Line %d: ",file,line);
	va_list argList;
	va_start(argList,text);

	vwriteOut(stdout,text, argList);

	va_end(argList);
	debugSig();
}

void _SGLprintInfo(const char file[], int line, const char text[], ...)
{
	fprintf(stdout,"simpleGL-Info %s Line %d: ",file,line);
	va_list argList;
	va_start(argList,text);

	vwriteOut(stdout,text, argList);

	va_end(argList);
}

void _SGLprintState(const char file[], int line, const char text[], ...)
{
	fprintf(stdout,"State: ");
	va_list argList;
	va_start(argList,text);

	vwriteOut(stdout,text, argList);

	va_end(argList);
}

void vwriteOut(FILE *file,const char text[], va_list argList)
{
	vfprintf(file,text,argList);
	fprintf(file,"\n");
}

short sglChkExt(const char* name,const char *msg,unsigned short vital)
{
	const GLubyte *exts=glGetString(GL_EXTENSIONS);	
	if(strstr(exts,name)==NULL)
	{
		if(vital>1){
			SGLprintError("Dieser Renderer (%s) unterstützt \"%s\" nicht. %s", glGetString(GL_RENDERER),name,msg);
		} else if(vital){
			SGLprintWarning("Dieser Renderer (%s) unterstützt \"%s\" nicht. %s", glGetString(GL_RENDERER),name,msg);
		} else {
			SGLprintInfo("Dieser Renderer (%s) unterstützt \"%s\" nicht. %s", glGetString(GL_RENDERER),name,msg);
		}
		return 0;
	}
	else return -1;
}

void debugSig()
{
#ifndef _OPTIMIZE
    char *sig=getenv("DEBUG");
    if(sig)
    {
		int isig=atoi(sig);
		if(!isig)isig=SIGINT;
		SGLprintInfo("Löse in Umgebungsvariable DEBUG vereinbartes Signal %d aus\n",isig);
		kill(getpid(),isig);
    }
#endif
}
