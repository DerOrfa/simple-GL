/***************************************************************************
                          evektor.h  -  description
                             -------------------
    begin                : Tue Aug 6 2002
    copyright            : (C) 2002 by Enrico Reimer
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

#ifndef EVEKTOR_H
#define EVEKTOR_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "sglmisc.h"
#include <math.h>

#ifdef HAS_SSTREAM
	#include <sstream>
#else
	#include "sstream"
#endif

#define COS(X)   cos( (X) * M_PI/180.0 )
#define SIN(X)   sin( (X) * M_PI/180.0 )
#define TAN(X)   tan( (X) * M_PI/180.0 )

#define ACOS(X)   (acos(X) * 180.0/M_PI )
#define ASIN(X)   (asin(X) * 180.0/M_PI )
#define ATAN(X)   (atan(X) * 180.0/M_PI )

using namespace std;

namespace emath
{

template <class T> class EMatrix;

/*!
  *Fï¿½hrt Vektoren als generativen Datentyp ein.
  *@author Enrico Reimer
  */

template <class T, class B=vector<T> > class EVektor : public B
{
public:
	EVektor<T,B>(){}
	virtual ~EVektor<T,B>(){}
	EVektor<T,B>(size_t n, const T& value = T()):B(n,value){}
	template <class sT> EVektor<T,B>(const EVektor<sT>& src)
	{
		resize(src.size());
		for(size_t i=src.size();i;i--)
			(*this)[i-1]=src[i-1];
	}

/*!
	\fn size_t fromArray(size_t s, Z values[])
	\brief Fï¿½llt den Vektor mit Daten aus einem (eindimensionalen) Array.
	Die Grï¿½ï¿½e des Vektors wird dabei angepasst.
*/
	template <class Z> size_t fromArray(size_t s, Z values[])
	{
		if(this->size()>s){SGLprintError("Das Array ist kleiner als der Vektor");}
		this->resize(s);

		for(typename EVektor<T,B>::size_type zeile=this->size();zeile;zeile--)
			(*this)[zeile-1]=values[zeile-1];
		return s;
	}
	
	template <class Z> size_t toArray(size_t s, Z values[])const
	{
		if(this->size()<s)
		{
			SGLprintError("Der Vektor ist zu klein, fï¿½lle das Array nicht vollstï¿½ndig");
			s=this->size();
		}
		for(size_t i=0;i<s;i++)
			values[i]=(*this)[i];
		return s;
	}

/*!
	\fn EVektor<T,B> operator +(const EVektor<T,B> &Vekt)
	\brief Liefert die Summe zweier Vektoren.
*/
	EVektor<T,B> operator +(const EVektor<T,B> &Vekt)const
	{
		EVektor<T,B> ret( this->size() > Vekt.size() ? this->size() : Vekt.size() );
		for(size_t i = ret.size(); i;i--)
			ret[i-1]=(*this)[i-1]+Vekt[i-1];
		return ret;
	}
/*!
	\fn EVektor<T,B> operator +=(const EVektor<T,B> &Vekt)
	\brief Wie "+" weist aber zusï¿½tzlich das Ergebniss dem linken Wert zu.
*/
	EVektor<T,B> operator +=(const EVektor<T,B> &Vekt)
	{
		size_t s=this->size();
		if(s<Vekt.size())
		{
			s=Vekt.size();
			this->resize(s);
		}
		for(size_t i = 0; i<s;i++)
			(*this)[i]+=Vekt[i];
		return *this;
	}
/*!
	\fn EVektor<T,B> operator -(const EVektor<T,B> &Vekt)
	\brief Liefert die Differnz zweier Vektoren.
*/
	EVektor<T,B> operator -(const EVektor<T,B> &Vekt)const
	{
		EVektor<T,B> ret( this->size() > Vekt.size() ? this->size() : Vekt.size() );
		for(size_t i = ret.size(); i;i--)
			ret[i-1]=(*this)[i-1]-Vekt[i-1];
		return ret;
	}

/*!
	\fn EVektor<T,B> operator -=(const EVektor<T,B> &Vekt)
	\brief Wie "-" weist aber zusï¿½tzlich das Ergebniss dem linken Wert zu.
*/
	EVektor<T,B> operator -=(const EVektor<T,B> &Vekt)
	{
		size_t s=this->size();
		if(s<Vekt.size())
		{
			s=Vekt.size();
			this->resize(s);
		}
		for(size_t i = 0; i<s;i++)
			(*this)[i]-=Vekt[i];
		return *this;
	}

/*!
	\fn EVektor<T,B> operator *(const Z Value)
	\brief Liefert das Produkt des Vektors mit einem Skalar.
*/
	template<class Z> EVektor<T,B> operator *(const Z Value)const
	{
		size_t s=this->size();
		EVektor<T,B> ret(*this);
		for(size_t i=0;i<s;i++)
			ret[i]*=Value;
		return ret;
	}
/*!
	\fn EVektor<T,B> operator *=(const Z Value)
	\brief Wie "*" weist aber zusï¿½tzlich das Ergebniss dem linken Wert zu.
*/
	template<class Z> EVektor<T,B> operator *=(const Z Value)
	{
		size_t s=this->size();
		for(size_t i=0;i<s;i++)
			(*this)[i]*=Value;
		return *this;
	}

/*!
	\fn EVektor<T,B> operator /(const Z Value)
	\brief Liefert das Produkt des Vektors mit dem Reziproken eines Skalars.
*/
	template<class Z> EVektor<T,B> operator /(const Z Value)const
	  // @todo Funktioniert nicht mit Ganzzahlen -> castet trotz "1." NICHT automatisch auf Flieï¿½komma
	  {return operator *(1./Value);}
/*!
	\fn EVektor<T,B> operator /=(const Z Value)
	\brief Wie "/" weist aber zusï¿½tzlich das Ergebniss dem linken Wert zu.
*/
	template<class Z> EVektor<T,B> operator /=(const Z Value)
	{return operator *=(1/Value);}

/*!
	\fn T skalarprod(const EVektor<T,B> &Vekt)
	\brief Liefert das Skalarprodukt des Vektors mit dem ï¿½bergeben Vektor.
*/
	T skalarprod(const EVektor<T,B> &Vekt)const
	{
		T ret=(*this)[0]*Vekt[0];
		for(size_t i=Vekt.size()-1;i;i--)
			ret=ret+((*this)[i]*Vekt[i]);
		return ret;
	}

/*!
	\fn EVektor<T,B> kreuzprod(const EVektor<T,B> &Vekt)
	\brief Liefert das Kreuzprodukt des Vektors mit dem ï¿½bergeben Vektor.
*/
	EVektor<T,B> kreuzprod(const EVektor<T,B> &Vekt)const
	{
		size_t max_i=this->size() > Vekt.size() ? this->size() : Vekt.size();
		EVektor<T,B> ret(max_i);

		for(size_t i=0;i<max_i;i++)
		{
			T Fakt11,Fakt12,Fakt21,Fakt22;
			Fakt11=(*this)[(i+1)%max_i];
			Fakt12=Vekt[(i+2)%max_i];
			Fakt21=(*this)[(i+2)%max_i];
			Fakt22=Vekt[(i+1)%max_i];
			ret[i]=Fakt11 * Fakt12 - Fakt21 * Fakt22;
		}
		return ret;
	}

/*!
	\fn T linearprod(const EVektor<T,B> &Vekt)
	\brief Multipliziert die korrespondierenden Elemente zweier Vektoren miteinander. Der Ergebniss hat die Lï¿½nge des kï¿½rzeren Operators.
*/
	EVektor<T,B> linearprod(const EVektor<T,B> &Vekt)const
	{
		size_t max_i=std::min(this->size(), Vekt.size());
		EVektor<T,B> ret(max_i);

		for(size_t i=0;i<max_i;i++)
			ret[i]=(*this)[i] * Vekt[i];
		return ret;
	}

/*!
	\fn T spatprod(const EVektor<T,B> &b,const EVektor<T,B> &c)
	\brief Liefert das Spatprodukt des Vektors und der ï¿½bergeben Vektoren.
*/
	T spatprod(const EVektor<T,B> &b,const EVektor<T,B> &c)const
	{
	  if(this->size()==3)
	    {//Sarrus
		 return	 (*this)[0]*b[1]*c[2] - (*this)[0]*b[2]*c[1]
			+(*this)[1]*b[2]*c[0] - (*this)[1]*b[0]*c[2]
			+(*this)[2]*b[0]*c[1] - (*this)[2]*b[1]*c[0];
	    }	  
	  else return this->kreuzprod(b).skalarprod(c);
	}

/*!
	\fn virtual int elemtostr(char* buf,typename EVektor<T,B>::size_type pos,const char* vparter="|")
	\brief Schreibt das durch pos Element als String in buf.

	Eigentlich ist vparter hier nicht nï¿½tig. Es ist aber mit angegeben, damit alle Compiler ï¿½berladungen
	in spï¿½teren Ableitungen, in denen vparter nï¿½tig ist, auch als solche erkennen.
*/
	virtual int elemtostr(char* buf,const typename EVektor<T,B>::size_type pos,const char* vparter="|")const
	  //der MIPSpro-Compiler ist seehr "vorsichtig" was das Ueberladen virtueller Funktionen mit
	  //neuen Parameterlisten angeht => deshalb hier die selben Parameter wie bei EMatrix::elemtostr(..)
	{
		ostringstream temp;
		temp << this->operator[](pos);
		return sprintf(buf,"%s",temp.str().c_str());
	}

/*!
	\fn int sprint(char* buf,const char* parter="|")
	\brief Schreibt den Vektor Formatiert in buf.
	Dabei wird parter als Trennzeichen zwischen den Elementen verwendet.
*/
	int sprint(char* buf,const char* parter="|")const
	{
		int ret=0;
		size_t s=this->size();

		for(size_t  i=0;i<s;i++)
		{
			ret+=elemtostr(&buf[ret],i);
			if(i<s-1)ret+=sprintf(&buf[ret],"%s",parter);
		}
		return ret;
	}

/*!
	\fn size_t Truncate()
	\brief Entfernt alle "Null-Elemente" am Ende des Vektors, und gibt seine neue Grï¿½ï¿½e zurï¿½ck.
*/
	size_t Truncate()
	{
		//alle Null- Elemente am Ende entfernen
		size_t s=this->size,newSize=0;

		for(size_t i=this->size;i;i--)
		{
			if((*this)[i-1])
			{
				newSize=i;
				break;
			}
		}
		this->resize(newSize);
		return newSize;//Anzahl der verbleibenden Elemente zurï¿½ckgeben
	}

/*!
	\fn size_t EMatrix<T,B,B2> getPMat(unsigned int size=0)
	\brief Liefert die "P-Matrix" des Vektors.

	Diese Funktion liefert eine Matrix bei der in der Zeile die dem Index des Vektoreintrages entspricht an der Spalte die dem Wert des Vektoreintrages entspricht eine "1" eingetragen wird.
*/
	EMatrix<T> getPMat(const unsigned int size=0)const
	{
		EMatrix<T> ret(size,size);
		for(size_t m=0;m<size;m++)
			ret[m][(*this)[m]]=T(1);
		return ret;
	}


/*!
	\fn T summe(bool Betrag=false,size_t first=0)
	\brief Liefert die Summe aus allen Elementen des Vektors

	Wenn Betrag=true ist, wird die Summe aus den Beträgen der Elemente gebildet.

	first gibt an bei welchen Element (von vorne) mit der Summierung angefangern werden soll.
*/
	T summe(const bool Betrag=false,const size_t first=0)const
	{
		T ret=T();
		for(size_t i=this->size;i>first;i--)
			ret+=Betrag ? autoabs((*this)[i-1]):(*this)[i-1];
	}

/*!
	\fn void swap(size_t index1, size_t index2)
	\brief Tauscht die Elemente an index1 und index2 untereinander aus.
*/
	void swap(size_t index1, size_t index2)
	{
		T temp=(*this)[index1];
		(*this)[index1]=(*this)[index2];
		(*this)[index2]=temp;
	}

/*!
	\fn void Normalize()
	\brief Normalisiert den Vektor.

	Kï¿½rzt oder Verlï¿½ngert den Vektor auf die Lï¿½nge 1. Die Richtung bleibt dabei erhalten.
*/
	void Normalize()
	{
		double d= Len();
		if(d==0){SGLprintError("Versuch Nullvektor zu Normalisieren\n");}
		else for(size_t i=this->size();i>0;i--)
		  (*this)[i-1] = T((*this)[i-1]/d);
	}

/*!
	\fn T Len()
	\brief Liefert die "quadratische Lï¿½nge" / das Quadrat des Betrages des Vektors.
*/
	inline double QLen()const
	{
		T QadSumm=0;
		for(size_t i=this->size();i>0;i--)QadSumm+=((*this)[i-1]*(*this)[i-1]);
		return QadSumm;
	}

/*!
	\fn T Len()
	\brief Liefert die  "Lï¿½nge" / den Betrag des Vektors.
*/
	inline double Len()const
	{
		return ::sqrt(QLen());
	}

/*!
	\fn T VektWink(EVektor<T,B> VektPtr)
	\brief Liefert den Winkel zwischen dem aufrufenden, und dem angegebenen Vektor.

	Das Ergebniss entspricht dem Winkel, um den der eine Vektor mindestens gedreht werden muss, um mit dem anderen kolinear zu sein.
*/
	T VektWink(const EVektor<T,B> &VektPtr)const
	{
		T Scal=skalarprod(VektPtr);
		T LenProd=VektPtr.Len()*Len();
		return(ACOS(Scal/LenProd));
	}

/*!
	\fn T B::iterator index2iterator(size_t index)
	\brief Liefert einen Iterator auf das indizierte Element zeigt.
*/

	typename B::iterator index2iterator(size_t index)
	{
		if(index>=this->size())
		{
		    printf("Index %d liegt hinter Ende\n",(int)index);
		    index=this->size()-1;
		}
		return this->begin()+index;
	}

	T BasisWink(T wink[])const
	{
		T a,b,c;
		int s=this->size();
		if(s<2)
		{
		     printf("Vesuch den aufgespannten Winkel eines %d-dimensionalen Vektors zu ermitteln\n",s);
		     return T();
		}

		c=Len(); //Lï¿½nge des Vektors

		if(s>2)
		{
			EVektor tVekt(*this);
			tVekt.erase(tVekt.index2iterator(1));
			a=tVekt.BasisWink(&wink[1]); //Lï¿½nge des Vektors auf die size()-1-Ebene projeziert (und sein Winkel auf dieser Ebene)
		}
		else a=(*this)[0];

		b=(*this)[1];//Hï¿½he
		wink[0]=ATAN(b/a);
		if(a<0)wink[0]+=180;//2. & 3. Quadranten gibts nur im 2dim Raum, bei hï¿½herdim. Rï¿½umen wird das auf eine 180ï¿½-Rotation auf der darunter-liegenden "Ebene" zurï¿½ckgefï¿½hrt
		else if(b<0)wink[0]+=360;
		return c;
	}
	
	inline T equality(const EVektor<T,B> &Vekt)const
	{
		return (*this-Vekt).summe(true);
	}

};

/*!
	\fn template <class T, class B> ostream &operator <<(ostream &o,EVektor<T,B> Vekt)
	\brief Schreibt die Elemente (unformatiert) in den ostream.
*/

template <class T, class B> ostream &operator <<(ostream &o,const EVektor<T,B> Vekt)
{
	typename EVektor<T,B>::size_type Vs=Vekt.size();
	if(Vs)
	{
		//wenn Vs==0 ist verursacht Vs-1 mï¿½glicherweise einen Unterlauf.
		//Dann ist das Ergebnis RICHTIG groï¿½, und hat sicher nix mehr
		//mit der Grï¿½ï¿½e des Vektors zu tun
		for(typename EVektor<T,B>::size_type i=0;i<Vs-1;i++)
			o << Vekt[i] << endl;
		o << Vekt[Vs-1];
	}
	return o;
}

/*!
	\fn template <class T, class B> EVektor<T,B> operator -(EVektor<T,B> &Vekt)
	\brief Liefert die Differenz aus den Nullvektor und dem angegeben Vektor.
*/

template <class T, class B> EVektor<T,B> operator -(const EVektor<T,B> &Vekt)
{
	EVektor<T,B> ret(Vekt.size());
	for(size_t i = ret.size(); i;i--)
		ret[i-1]=-Vekt[i-1];
	return ret;
}

/*!
	\fn template <class T, class B> istream &operator >>(istream &o,EVektor<T,B> &Vekt)
	\brief Fï¿½llt den Vektor mit dem "Inhalt" des Streams o.
*/
template <class T, class B> istream &operator >>(istream &o,EVektor<T,B> &Vekt)
{
	for(typename EVektor<T,B>::size_type i=0;i<Vekt.size();i++)
		o >> Vekt[i];
	return o;
}

}//namespace

#include "ematrix.h"

#endif
