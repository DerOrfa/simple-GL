/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#define NOMINMAX // yep, that nonsense is still there

#include "sgldisplist.h"
#include "../sglmisc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits>


/**
 * Standartkonstruktor.
 * Initialsiert die Objketarrays Objects_CW und Objects_CCW für den Anfang auf leere Arrays mit 5 freien Zellen.
 * @param transp flag, ob diese Liste Transparente Objekte beinhaltet (muss anders gerendert werden)
 */
SGLObjList::SGLObjList(bool transp)
{
	Objects_CW=Objects_CCW=NULL;
	ObjCnt_CW=ObjCnt_CCW=0;
	ObjSize_CW=ObjSize_CCW=0;
	grow(Objects_CW,ObjSize_CW,5);
	grow(Objects_CCW,ObjSize_CCW,5);
	Clear();
	renderTransparent=transp;
	render_non_native=true;
}

SGLObjList::SGLObjList(const SGLObjList &src)
{
	operator=(src);
}

/**
 * Destruktor.
 * Trägt alle Objekte aus der Liste aus. Sie gehören daher danach keiner Liste mehr an.
 * Wenn sie noch an anderer Stelle (z.B. von anderen Spaces) verwendet werden werden diese Waisen noch nicht automatisch gelöscht.
 */
SGLObjList::~SGLObjList()
{
	for(std::list<shared_obj>::iterator i=Objects.begin();i!=Objects.end();i++)
	{
		if(*i){if((*i)->myList==this)(*i)->myList=NULL;}//Alle Objekte trennen
		else{SGLprintError("Huch, da is ein Ungültiges Objekt in der Liste an Stelle %d von %d",distance(i, Objects.begin()),Objects.size());}
	}
	//@todo was passiert mit den getrennten - bringt das irgendwen durcheinander, wenn Obs zu keiner Liste gehören ? - und was is mit "obj->is_free"
}

/**
 * Kopierkonstruktor.
 * Es werden alle Objektlisten samt Inhalt kopiert.
 * Die Objekte bleiben jedoch trotzdem im Besitz des Originals.
 * @param src die Liste, die kopiert werden soll.
 * @return eine Referenz auf die Kopie
 */
SGLObjList& SGLObjList::operator=(const SGLObjList &src)
{
	Objects_CW=Objects_CCW=NULL;
	ObjSize_CW=ObjSize_CCW=0;
	grow(Objects_CW,ObjSize_CW,src.ObjSize_CW);
	grow(Objects_CCW,ObjSize_CCW,src.ObjSize_CCW);
	renderTransparent=src.renderTransparent;
	render_non_native=src.render_non_native;

	ObjCnt_CW=src.ObjCnt_CW;
	ObjCnt_CCW=src.ObjCnt_CCW;
	memcpy(Objects_CW,src.Objects_CW,ObjCnt_CW*sizeof(GLuint));
	memcpy(Objects_CCW,src.Objects_CCW,ObjCnt_CCW*sizeof(GLuint));
	Objects=src.Objects;
	check_recompile=src.check_recompile;
	check_sorting=src.check_sorting;
	return *this;
}

/**
 * Löst das Zeichnen aller Objekte der Liste aus.
 * Wenn nötig wird Compile() ausgeführt.
 * Es werden alle CW- und CCW- Objekte entsprechend ihrer Priorität aus dem Renderercache gezeichnet.
 */
void SGLObjList::CallAllLists()
{
	if(Objects.empty())return;

	if(renderTransparent)
		glEnable(GL_BLEND);//Ab hier transparent

	if(check_recompile)
		Compile();

	glFrontFace(GL_CW);
	for(int i=ObjCnt_CW;i;i--) //@todo sollte schon noch geprüft werden (wenigstens im Debug)
		if(!glIsList(Objects_CW[i-1]))
		{
			SGLprintError("Es existiert kein Objekt mit der ID %d",Objects_CW[i-1]);
		}
	glCallLists(ObjCnt_CW,GL_UNSIGNED_INT,Objects_CW);

	glFrontFace(GL_CCW);
	for(int i=ObjCnt_CCW;i;i--)
		if(!glIsList(Objects_CCW[i-1]))
		{
			SGLprintError("Es existiert kein Objekt mit der ID %d",Objects_CCW[i-1]);
		}
	glCallLists(ObjCnt_CCW,GL_UNSIGNED_INT,Objects_CCW);
	if(renderTransparent)
		glDisable(GL_BLEND);//Ab hier nicht mehr transparent
}


/**
 * Trägt ein neues Objekt in der Liste ein.
 * Ist das Objekt schon Teil der Liste. wird es vor dem erneuten Eintragen entfernt.
 * Das Flag is_free des Objektes wird false gesetzt und die Liste wird als Besitzer des Objektes im Objekt vermerkt.
 * Hat das Objekt schon einen Besitzer wird stattdessen im Objekt das Flag shared true gesetzt.
 * Es wird außerdem vermerkt, daß die Liste beim nächsten Aufruf von CallAllLists() Compile() ausführen und ihre Sortierung prüfen muss.
 * @param obj das Objekt, das in die Liste aufgenommen werden soll.
 * @return true wenn das Objekt aufgenommen wrude, sonst false.
 */
bool SGLObjList::AddOb(shared_obj obj)
{
	if(removeOb(obj))
	{SGLprintWarning("Das %s-Objekt hat schon in der Liste existiert",obj->guesType());}//@todo nur bei debug

	Objects.push_back(obj);
	obj->is_free=false;
	//Wir Kompilieren hier noch nicht, markieren nur das Obj als "zu kompilieren"
	if(obj->myList)obj->shared=true;
	else
	{
		obj->myList=this;
//		obj->compileNextTime();//@todo macht das überhaupt sinn - wenn das Ob neu is, wird es eh generiert
		//Wenn das obj schon ner Liste angehört, gehen wir mal von aus, daß es schon kompiliert ist
	}
	check_sorting=check_recompile=true;
	return true;
}

bool SGLObjList::AddOb(GLuint ListID,GLenum Face)
{
	if(!glIsList(ListID))
	{
		SGLprintError("Es existiert kein Objekt mit der ID %d",ListID);
	}

	switch(Face)
	{
	case GL_CW:
		if(ObjCnt_CW==std::numeric_limits<unsigned short>::max())
		{SGLprintError("Sorry, zu viele Objekte");return false;}
		if(ObjCnt_CW>=ObjSize_CW)
			grow(Objects_CW,ObjSize_CW,ObjCnt_CW*2);
		if(Objects_CW)
		{
			Objects_CW[ObjCnt_CW++]=ListID;
			return true;
		}
	case GL_CCW:
		if(ObjCnt_CCW==std::numeric_limits<unsigned short>::max())
		{SGLprintError("Sorry, zu viele Objekte");return false;}
		if(ObjCnt_CCW>=ObjSize_CCW)
			grow(Objects_CCW,ObjSize_CCW,ObjCnt_CCW*2);
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

/**
 * Entfernt ein Objekt aus der Liste.
 * Es wird außerdem vermerkt, daß die Liste beim nächsten Aufruf von CallAllLists() Compile() ausführen und ihre Sortierung prüfen muss.
 * @param obj das zu entfernende Objekt.
 * @return false, wenn das Objekt nicht gefunden wurde, sonst true wenn es entfernt wurde.
 */
bool SGLObjList::removeOb(shared_obj obj)
{
	if(Objects.empty())return false;
	std::list<shared_obj>::iterator i=find(Objects.begin(), Objects.end(), obj);
	if(i!=Objects.end())Objects.erase(i);
	else return false; //Objekt wurde NICHT gefunden

	if(obj->myList==this)obj->myList=NULL;
	switch(obj->FrontFace)
	{
	case GL_CW:return removeOb_CW(obj->ID);
	case GL_CCW:return removeOb_CCW(obj->ID);
	}
	return false;
}

bool SGLObjList::removeOb_CW(GLuint ListID)
{
	unsigned short i=0;
	if(!ObjCnt_CW)return false;
	while(Objects_CW[i]!=ListID)
		if(Objects_CW[i])i++;
		else return false;
	for(i++;i<ObjCnt_CW;i++)
		Objects_CW[i-1]=Objects_CW[i];
	Objects_CW[i-1]=0;
	ObjCnt_CW--;
	return true;
}

bool SGLObjList::removeOb_CCW(GLuint ListID)
{
	unsigned short i=0;
	if(!ObjCnt_CCW)return false;
	while(Objects_CCW[i]!=ListID)
		if(Objects_CCW[i])i++;
		else return false;
	for(i++;i<ObjCnt_CCW;i++)
		Objects_CCW[i-1]=Objects_CCW[i];
	Objects_CCW[i-1]=0;
	ObjCnt_CCW--;
	return true;
}

void SGLObjList::Clear()
{
	for(unsigned short	i=0;i<ObjCnt_CW;i++)
		Objects_CW[i] =0;
	for(unsigned short	i=0;i<ObjCnt_CCW;i++)
		Objects_CCW[i]=0;
	// @todo wozu eig. ?
	ObjCnt_CW=ObjCnt_CCW=0;
}

template<class T> bool SGLObjList::grow(T *&liste,unsigned short &oldsize,unsigned short newsize)
{
	if(oldsize>=newsize){SGLprintWarning("kein Resize nötig");return false;}
	if(!(liste=(T*)realloc(liste,newsize*sizeof(T))))
	{
		SGLprintError("realloc-Fehler beim Vergößern der Objektliste, versuche calloc-Fallback");
		liste=(T*)calloc(newsize,sizeof(T));
		//@todo hier müssten ALLE Objekte neu kompiliert werden, wenn das die Objekt-Pointerliste ist, haben wir ein "Problem" => nich das gelbe vom Ei
		if(!liste){SGLprintError("*argn* calloc konnte's auch nicht richten");return false;}
	}
	oldsize=newsize;
	return true;
}

/**
 * Ruft für alle Objekte in der Liste SGLObjBase::metaCompile() entsprechend ihrer Priorität auf.
 * Die Operation setzt bei ihrer Ausführung das Flag check_recompile auf false.
 * Hat die Liste daher einmal Compile() ausgeführt, wird dies erst wieder tun, wenn das entsprechende Flag von außen neu auf true gesetzt wurde.
 * @param force wenn true, wird für alle Objekte der Renderercache neu generiert. Unabhängig davon, ob sie geändert wurden.
 */
void SGLObjList::Compile(bool force)
{
	if(Objects.empty())return;
	Clear();
	if(check_sorting)
	{
		Objects.sort(SortFunc());
		check_sorting=false;
		ListInfo();
	}
	for(std::list<shared_obj>::iterator i=Objects.begin();i!=Objects.end();i++)
		if(render_non_native || (*i)->myList==this)
			AddOb((*i)->metaCompile(force),(*i)->FrontFace);
	check_recompile=false;
}

/*!
    \fn SGLObjList::ListInfo()
 */
void SGLObjList::ListInfo()
{
	//@todo gaaanz böse, chrasht immer auf xf86 ... ???
/*	if(SGLshowInfos)
	{
		char *elemente=(char*)malloc(sizeof(char));elemente[0]=0;
		unsigned int size=0;
		char FormatStr[]="\n\t%d.\t\"%s\"\tPrio %d ID %d";
		unsigned int FormatStr_Len=strlen(FormatStr)-3*2;

		unsigned int cnt=0;
		for(list<shared_obj>::iterator i=Objects.begin();i!=Objects.end();i++)
		{
			shared_obj ob=*i;
			unsigned int oldlen=strlen(elemente);
			unsigned int newnlen=oldlen+FormatStr_Len+strlen(cnt)+strlen(ob->guesType())+strlen(ob->priority)+strlen(ob->ID);
			if(newnlen>size)
			{
				elemente=(char*)realloc(elemente,(newnlen+1)*sizeof(char));
				size=newnlen;
			}
			sprintf(elemente+oldlen,FormatStr,cnt,ob->guesType(),ob->priority,ob->ID);
		}
		SGLprintInfo("Liste 0x%X:%s\n",this,elemente);*/
		//@todo valgrind sagt hier was von "invalid write of size 1"
		//@todo und das realloc passt ihm auch nich (im 2+n ten durchlauf)
		//@todo und es hat recht, wenn das hier aktiv is, fällt alles Folgende auseinander
//	}
}


/**
 * Sucht in der Liste nach einen bestimmnten Objekt.
 * @param obj das Objekt, nach dem gesucht werden soll
 * @return 0 wenn das Objekt nicht gefunden wurde, sonst die Anzahl der derzeitigen Verwendungen
 */
unsigned short SGLObjList::isThere(const SGLObjBase * const obj)
{
	for(std::list<shared_obj>::iterator i=Objects.begin();i!=Objects.end();i++)
	{
		if(*i){if(i->get()==obj)return i->use_count();}
		else{SGLprintError("Huch, da is ein Ungültiges Objekt in der Liste an Stelle %d von %d",distance(i, Objects.begin()),Objects.size());}
	}
	return 0;
}
