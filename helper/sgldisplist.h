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
//	#include <gl\glaux.h>
#else
	#ifdef __APPLE__
		#include <OpenGL/gl.h>
	#else
		#include <GL/gl.h>
	#endif
#endif

#include "../sglobjbase.h"
#include "../sglshptr.h"


#include <list>


/**
  *@author Enrico Reimer
  */
class SGLObjList
{
public:
	struct SortFunc : public binary_function<shared_obj &, shared_obj &, bool> {
		bool operator()(shared_obj &x, shared_obj &y) { return y->priority < x->priority; }
	};
	SGLObjList(bool transp=false);
	virtual ~SGLObjList();
	SGLObjList(const SGLObjList &src);
	SGLObjList& operator=(const SGLObjList &src);
	bool	removeOb(GLuint ListID);
	bool	removeOb(shared_obj obj);
	bool	AddOb(GLuint ListID,GLenum Face);
	bool 	AddOb(shared_obj obj);
	void	Clear();
	list<shared_obj>	Objects;
	GLuint		*Objects_CW;
	GLuint		*Objects_CCW;
	unsigned short	ObjCnt_CW,ObjCnt_CCW;
	unsigned short	ObjSize_CW,ObjSize_CCW;
	void	CallAllLists();
	void	Compile(bool force=false);
	bool	check_recompile,check_sorting;
	bool	renderTransparent,render_non_native;
	void	ListInfo();
	unsigned short isThere(const SGLObjBase*const obj);

private:
	bool	removeOb_CW(GLuint ListID);
	bool	removeOb_CCW(GLuint ListID);
	template<class T>
	bool	grow(T *&liste,unsigned short &size,unsigned short newsize);
};

#endif
