/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SGLDISPLIST_H
#define SGLDISPLIST_H

#ifdef WIN32
	#include <windows.h>
	#include <gl\gl.h>
	#include <gl\glu.h>
	#include <gl\glaux.h>
#else
	#include <GL/gl.h>
#endif

#include "../sglobjbase.h"

/**
  *@author Enrico Reimer
  */
class SGLObjList
{
public:
	SGLObjList(bool transp=false);
	SGLObjList(const SGLObjList &src);
	SGLObjList& operator=(const SGLObjList &src);
	~SGLObjList();
	bool	removeOb(GLuint ListID);
	bool	removeOb(SGLObjBase *obj);
	bool	AddOb(GLuint ListID,GLenum Face);
    bool	AddOb(SGLObjBase* obj);
	void	Clear();
	SGLObjBase	**ObjPtr;
	GLuint		*Objects_CW;
	GLuint		*Objects_CCW;
	unsigned int	ObjCnt_CW,ObjCnt_CCW,ObjCnt_Ptr;
	unsigned int	ObjSize_CW,ObjSize_CCW,ObjSize_Ptr;
	void	CallAllLists();
	void	Compile(bool force=false);
	bool	check_recompile,check_sorting;
	bool	renderTransparent;
    void ListInfo();

private:
	bool	removeOb_CW(GLuint ListID);
	bool	removeOb_CCW(GLuint ListID);
	bool	removeOb_Ptr(SGLObjBase *obj);
	template<class T> bool grow(T *&liste,unsigned int &size,unsigned int newsize);
	static int compareObj(const void *elem1,const void *elem2);
};

#endif
