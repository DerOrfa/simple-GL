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
#include <stdarg.h>

short SGLshowInfos=1;
short SGLshowWarnings=1;
short SGLshowErrors=1;
char *lastMsg=NULL;

void _SGLprintError(const char text[], ...)
{
  if(SGLshowErrors)
    {
      va_list argList;
      va_start(argList,text);

      vwriteOut(stderr,text, argList);

      va_end(argList);
    }
}

void _SGLprintWarning(const char text[], ...)
{
  if(SGLshowWarnings)
    {
      va_list argList;
      va_start(argList,text);

      vwriteOut(stdout,text, argList);

      va_end(argList);
    }
}

void _SGLprintInfo(const char text[], ...)
{
  if(SGLshowInfos)
    {
      va_list argList;
      va_start(argList,text);

      vwriteOut(stdout,text, argList);

      va_end(argList);
    }
}

void vwriteOut(FILE *file,const char text[], va_list argList)
{
/*	if(lastMsg)
	{
		if(strcmp(lastMsg,text)==0)return;
		free(lastMsg);
	}
	lastMsg=malloc((strlen(text)+1)*sizeof(char));
	strcpy(lastMsg,text);*/ //@todo Das geht so nich (es wird voher noch printf gemacht)
	
	vfprintf(file,text,argList);
	fprintf(file,"\n");
}
