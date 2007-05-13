//
// C++ Interface: sglshobj
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLSHOBJ_H
#define SGLSHOBJ_H

#include <boost/shared_ptr.hpp>
#include "sglmisc.h"

/**
@file sglshptr.h
@author Enrico Reimer,,,
*/
template<class T> class SGLshPtr : public boost::shared_ptr<T>
{
public:
	typedef T basetype;
	/**
	 * Defaultkonstruktor
	 * Erzeugt leeren Zeiger. 
	 * Als bool interpretiert [ \code if(*this) \endcode ] wird er false sein, bis ihm eine neue Adresse zugewiesen wird
	 */
	SGLshPtr():boost::shared_ptr<T>(){}
	/**
	 * Allgemeiner Konstruktor.
	 * Erzeugt Zeiger auf durch angegebenen C-Zeiger referenziertes Objekt vom Typ T.
	 * Sollte idR. als \code SGLshPtr<typ>(new typ()) \endcode verwendet werden.
	 * @param ob C-Zeiger auf ein Objekt vom Typ T. Dieser Zeiger sollte nur hier verwedet werden.
	 */
	SGLshPtr(T *ob):boost::shared_ptr<T>(ob){}
	/**
	 * Kopierkostruktor für boost::shared_ptr.
	 * @param ob Zu kopierender Zeiger.
	 */
	SGLshPtr(const boost::shared_ptr<T> &ob):boost::shared_ptr<T>(ob){}
	/**
	 * Kopierkostruktor für SGLshPtr.
	 * @param ob Zu kopierender Zeiger.
	 */
	template<class D> SGLshPtr(const SGLshPtr<D> &ob):boost::shared_ptr<T>(ob){}

	/**
	 * Automatisches dynamisches Cast. (Laufzeit-Cast)
	 * Versucht den Zeiger in einen neuen Zeiger mit anderem Typ zu casten.
	 * Scheitert der Cast, ist  src ein leerer Zeiger.
	 * @param ob der Ziel-Zeiger
	 * @param check wenn true, wird überprüft ob der cast erfolgreich war und gegebenenfalls eine Warnung ausgegeben.
	 * @return Wahrheitswert, ob Cast erfolgreich war (ob Ziel-Zeiger nicht-leer ist)
	 */
	template<class D> bool dcast(SGLshPtr<D> &ob,const bool check=true)const
	{
		boost::dynamic_pointer_cast<D>(*this).swap(ob);
		if(check && !ob){SGLprintWarning("SGLshPtr: dynamic_cast von %s nach %s gescheitert",typeid(T).name(),typeid(D).name());}
		return bool(ob);
	}
	/**
	 * Automatisches statisches Cast. (Kompilezeit-Cast)
	 * Versucht den Zeiger in einen neuen Zeiger mit anderem Typ zu casten.
	 * @param ob der Ziel-Zeiger
	 */
	template<class D> void scast(SGLshPtr<D> &ob)const
	{
		boost::static_pointer_cast<D>(*this).swap(ob);
	}
};

/**
 * \defgroup shPtr_new SGLshPtr_new
 * SGLshPtr_new(...) ist eine Abkürzung für \code SGLshPtr<Type>(new Type,..) \endcode
 */
/*@{*/
/**
 * Objektkonstruktor für Objekte deren Konstruktor 0 Parameter erwartet
 */
template<class T> SGLshPtr<T> SGLshPtr_new(){return SGLshPtr<T>(new T);}
/**
 * Objektkonstruktor für Objekte deren Konstruktor 1 Parameter erwartet
 * @param p1 erster Parameter des Kostruktors
 */
template<class T,typename param1> SGLshPtr<T> SGLshPtr_new(param1 p1){return SGLshPtr<T>(new T(p1));}
/**
 * Objektkonstruktor für Objekte deren Konstruktor 1 Parameter erwartet
 * @param p1 erster Parameter des Kostruktors
 * @param p2 zweiter Parameter des Kostruktors
 */
template<class T,typename param1,typename param2> SGLshPtr<T> SGLshPtr_new(param1 p1,param2 p2){return SGLshPtr<T>(new T(p1,p2));}
/*@}*/

/**
 * \defgroup global_cast Globale Cast-Operatoren
 * SGLshPtr_new(...) ist eine Abkürzung für \code SGLshPtr\<Type\>(new Type,..) \endcode
 */
/*@{*/
/**
 * Explizites dynamisches Cast. (Laufzeit-Cast)
 * Versucht den Zeiger in einen neuen Zeiger mit anderem angegebenem Typ zu casten.
 * @param src der Zeiger vom dem gecastet werden soll.
 * @param check wenn true, wird überprüft ob der cast erfolgreich war und gegebenenfalls eine Warnung ausgegeben.
 * @return das Ergebniss des Casts (Ein Zeiger \code SGLshPtr<D> \endcode, der an die selbe Adresse wie src zeigt), oder ein leerer Zeiger.
 */
template<class D,class T> SGLshPtr<D> dcast(const SGLshPtr<T> &src,const bool check=true)
{
	SGLshPtr<D> ret;
	boost::dynamic_pointer_cast<D>(src).swap(ret);
	if(check && !ret){SGLprintWarning("SGLshPtr: dynamic_cast von %s nach %s gescheitert",typeid(T).name(),typeid(D).name());}
	return ret;
}
/**
 * Explizites statisches Cast. (Kompilezeit-Cast)
 * Versucht den Zeiger in einen neuen Zeiger mit anderem angegebenem Typ zu casten.
 * @param src der Zeiger vom dem gecastet werden soll.
 * @return das Ergebniss des Casts, (Ein Zeiger \code SGLshPtr<D> \endcode, der an die selbe Adresse wie src zeigt)
 */
template<class D,class T> SGLshPtr<D> scast(const SGLshPtr<T> &src)
{
	SGLshPtr<D> ret;
	boost::static_pointer_cast<D>(src).swap(ret);
	return ret;
}
/*@}*/
#endif
