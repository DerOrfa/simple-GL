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

#define MAX_OBJ_CNT	512

/**
  *@author Enrico Reimer
  */
class SGLObjList
{
public:
	SGLObjList();
	~SGLObjList();
	bool	removeOb(GLuint ListID);
	bool	removeOb(SGLObjBase *obj);
	bool	AddOb(GLuint ListID,GLenum Face);
    bool	AddOb(SGLObjBase* obj);
	void	Clear();
	SGLObjBase	**ObjPtr;
	GLuint	*Objects_CW;
	GLuint	*Objects_CCW;
	unsigned int	ObjCnt_CW,ObjCnt_CCW,ObjCnt_Ptr;
	void	CallAllLists();
	void	Compile(bool force=false);
	bool	check_recompile,check_sorting;
	bool	renderTransparent;

private:
	bool	removeOb_CW(GLuint ListID);
	bool	removeOb_CCW(GLuint ListID);
	bool	removeOb_Ptr(SGLObjBase *obj);
	template<class T> bool grow(T* &liste,unsigned int &cnt,unsigned int newsize);
	template<class T> unsigned int getListSize(T *liste);
	static int compareObj(const void *elem1,const void *elem2);
    void ListInfo();
};

#endif
