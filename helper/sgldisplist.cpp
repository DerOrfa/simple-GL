/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "sgldisplist.h"
#include "../sglmisc.h"
#include <stdlib.h>
#include <stdio.h>


SGLObjList::SGLObjList()
{
	ObjPtr=NULL;
	Objects_CW=Objects_CCW=NULL;
	ObjCnt_CW=ObjCnt_CCW=ObjCnt_Ptr=0;
	grow(Objects_CW,ObjCnt_CW,5);
	grow(Objects_CCW,ObjCnt_CCW,5);
	grow(ObjPtr,ObjCnt_Ptr,5);
	Clear();
	renderTransparent=false;
}
SGLObjList::~SGLObjList ()
{
}

void SGLObjList::CallAllLists()
{
	if(ObjCnt_Ptr<=0)return;

	if(renderTransparent)
		glEnable(GL_BLEND);//Ab hier transparent
	if(check_recompile)Compile();
	
	glFrontFace(GL_CW);
	for(int i=ObjCnt_CW;i;i--) @todo sollte schon noch geprüft werden (wenigstens im Debug)
		if(!glIsList(Objects_CW[i]))
		{
			SGLprintError("Es existiert kein Objekt mit der ID %d",Objects_CW[i]);
		}
	glCallLists(ObjCnt_CW,GL_UNSIGNED_INT,Objects_CW);
	
	glFrontFace(GL_CCW);
	for(int i=ObjCnt_CCW;i;i--)
		if(!glIsList(Objects_CW[i]))
		{
			SGLprintError("Es existiert kein Objekt mit der ID %d",Objects_CW[i]);
		}
	glCallLists(ObjCnt_CCW,GL_UNSIGNED_INT,Objects_CCW);
	if(renderTransparent)
		glDisable(GL_BLEND);//Ab hier nicht mehr transparent
}


/*!
    \fn SGLObjList::AddOb(SGLObjBase* obj)
 */
bool SGLObjList::AddOb(SGLObjBase* obj)
{
	if(ObjCnt_Ptr>=getListSize(ObjPtr))
		grow(ObjPtr,ObjCnt_Ptr,ObjCnt_Ptr*2);
	if(Objects_CW)
	{
		ObjPtr[ObjCnt_Ptr++]=obj;
		//Wir Kompilieren hier noch nicht, markieren nur das Obj als "zu kompilieren"
		obj->myList=this;
		obj->compileNextTime();
		check_sorting=check_recompile=true;
		return true;
	}
	else return false;
}

bool SGLObjList::AddOb(GLuint ListID,GLenum Face)
{
	switch(Face)
	{
	case GL_CW:
		if(ObjCnt_CW>=getListSize(Objects_CW))
			grow(Objects_CW,ObjCnt_CW,ObjCnt_CW*2);
		if(Objects_CW)
		{
			Objects_CW[ObjCnt_CW++]=ListID;
			return true;
		}
	case GL_CCW:
		if(ObjCnt_CCW>=getListSize(Objects_CCW))
			grow(Objects_CCW,ObjCnt_CCW,ObjCnt_CCW*2);
		if(Objects_CCW)
		{
			Objects_CCW[ObjCnt_CCW++]=ListID;
			return true;
		}
	}
	return false;
}

bool SGLObjList::removeOb(GLuint ListID)
{return (removeOb_CW(ListID) || removeOb_CCW(ListID));}

bool SGLObjList::removeOb(SGLObjBase *obj)
{
	if(!removeOb_Ptr(obj))return false;
	switch(obj->FrontFace)
	{
	case GL_CW:return removeOb_CW(obj->ID);
	case GL_CCW:return removeOb_CCW(obj->ID);
	}
	return false;
}

bool SGLObjList::removeOb_Ptr(SGLObjBase *obj)
{
	int i=0;
	while(ObjPtr[i]!=obj)
		if(ObjPtr[i])i++;
		else break;
	for(i++;i<ObjCnt_Ptr;i++)
		ObjPtr[i-1]=ObjPtr[i];
	ObjCnt_Ptr--;
	obj->myList=NULL;
	return true;
}

bool SGLObjList::removeOb_CW(GLuint ListID)
{
	int i=0;
	while(Objects_CW[i]!=ListID)
		if(Objects_CW[i])i++;
		else break;
	for(i++;i<ObjCnt_CW;i++)
		Objects_CW[i-1]=Objects_CW[i];
	ObjCnt_CW--;
	return true;
}

bool SGLObjList::removeOb_CCW(GLuint ListID)
{
	int i=0;
	while(Objects_CCW[i]!=ListID)
		if(Objects_CCW[i])i++;
		else break;
	for(i++;i<ObjCnt_CCW;i++)
		Objects_CCW[i-1]=Objects_CCW[i];
	ObjCnt_CCW--;
	return true;
}

void SGLObjList::Clear()
{
	for(int i=0;i<sizeof(Objects_CW)/sizeof(GLuint);i++)
	{
		Objects_CW[i]=0;
		Objects_CCW[i]=0;
	}// @todo wozu eig. ?
	ObjCnt_CW=ObjCnt_CCW=0;
}

/*!
    \fn template<class T> bool SGLObjList::grow(T *&liste,unsigned int &cnt,unsigned int newsize)
 */
template<class T> bool SGLObjList::grow(T *&liste,unsigned int &cnt,unsigned int newsize)
{
	short elsize=sizeof(T);
	int oldsize=getListSize(liste);
	if(oldsize>=newsize){SGLprintWarning("kein Resize nötig");return false;}
	if(!(liste=(T*)realloc(liste,newsize*sizeof(T))))
	{
		SGLprintError("realloc-Fehler beim Vergößern der Objektliste, versuche calloc-Fallback");
		liste=(T*)calloc(newsize,sizeof(T));
		//@todo hier müssten ALLE Objekte neu kompiliert werden, wenn das die Objekt-Pointerliste ist, haben wir ein "Problem" => nich das gelbe vom Ei
		if(!liste){SGLprintError("*argn* calloc konnte's auch nicht richten");return false;}
	}
	return true;
}

template<class T> unsigned int SGLObjList::getListSize(T *liste)
{
	unsigned int size=liste ? sizeof(liste):0;
	return size ? size/sizeof(T):0;
}


/*!

    \fn SGLObjList::Compile(bool force=false)
 */
void SGLObjList::Compile(bool force)
{
	if(ObjCnt_Ptr<=0)return;
	Clear();
	if(check_sorting)
	{
		qsort(ObjPtr,ObjCnt_Ptr,sizeof(SGLObjBase*),compareObj);
		check_sorting=false;
		ListInfo();
	}
	for(unsigned int i=0;i<ObjCnt_Ptr;i++)
		AddOb(ObjPtr[i]->metaCompile(force),ObjPtr[i]->FrontFace);
	check_recompile=false;
}
int SGLObjList::compareObj(const void *elem1,const void *elem2)
{return (*(SGLObjBase**)elem2)->priority-(*(SGLObjBase**)elem1)->priority;}


/*!
    \fn SGLObjList::ListInfo()
 */
void SGLObjList::ListInfo()
{
	char *elemente=new char[1];elemente[0]=0;
	unsigned int size=0;
	char FormatStr[]="\n\t%d.\t\"%s\"\tPrio %d";
	unsigned int FormatStr_Len=strlen(FormatStr)-3*2;

	for(unsigned int i=0;i<ObjCnt_Ptr;i++)
	{
		int oldlen=strlen(elemente);
		int newnlen=oldlen+FormatStr_Len+strlen(i)+strlen(ObjPtr[i]->guesType())+strlen(ObjPtr[i]->priority);
		if(newnlen>size)
		{
			elemente=(char*)realloc(elemente,(newnlen+1)*sizeof(char));
			size=newnlen;
		}
		sprintf(elemente+oldlen,FormatStr,i,ObjPtr[i]->guesType(),ObjPtr[i]->priority);
	}
	SGLprintInfo("Liste 0x%X:%s\n",this,elemente);
}
