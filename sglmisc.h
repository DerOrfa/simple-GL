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



#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#include <stdarg.h>
#endif

#define SGLprintError	if(SGLshowErrors)fprintf(stderr,"simpleGL-Error %s Line %d: ",__FILE__,__LINE__);_SGLprintError
#define SGLprintWarning	if(SGLshowWarnings)fprintf(stdout,"simpleGL-Warning %s Line %d: ",__FILE__,__LINE__);_SGLprintWarning
#define SGLprintInfo	if(SGLshowInfos)fprintf(stdout,"simpleGL-Info %s Line %d: ",__FILE__,__LINE__);_SGLprintInfo



#ifdef __cplusplus
extern "C" {
#endif

void _SGLprintError(const char text[], ...);
void _SGLprintWarning(const char text[], ...);
void _SGLprintInfo(const char text[], ...);

void vwriteOut(FILE *out,const char text[], va_list argList);
void writeOut(FILE *out,const char text[], ...);

extern short SGLshowErrors,SGLshowInfos,SGLshowWarnings;

#ifdef __cplusplus
};
#endif

#endif


