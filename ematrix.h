/***************************************************************************
                          EMatrix.h  -  description
                             -------------------
    begin                : Wed Aug 7 2002
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

// n ist die anzahl der Variablen => Anzahl der Spalten
// m ist die Anzahl der Gleichungen => Anzahl der Zeilen

#ifndef EMATRIX_H
#define EMATRIX_H

#define TEST_MAT_SIZE	4

#include "evektor.h"
#include <time.h>
#include <stdlib.h>

namespace emath
{

template <class T> class EMatrix : public EVektor< EVektor<T> >
{
public:
	void resize(typename vector<T>::size_type zeilen,typename vector<T>::size_type spalten)
	{
		((EVektor< EVektor<T> >*)this)->resize(zeilen);//GCC erkennt hier die Vererbung nicht ??
		for(typename EMatrix<T>::size_type i=mDim();i;i--)
			operator[](i-1).resize(spalten);
	}

	EMatrix<T>(){}
	EMatrix<T>(typename vector<T>::size_type zeilen,typename vector<T>::size_type spalten,const T& value = T())
	:EVektor< EVektor<T> > ( zeilen, EVektor<T>(spalten,value))	{}

	size_t fromArray(typename vector<T>::size_type zeilen,typename vector<T>::size_type spalten, T *values)
	{
		resize(zeilen,spalten);

		for(typename EMatrix<T>::size_type zeile=zeilen;zeile;zeile--)
			operator[](zeile-1).fromArray(spalten,&values[(zeile-1)*spalten]);
		return zeilen;
	}

	virtual ~EMatrix<T>(){}

	template <class sT> EMatrix<T>(const EMatrix<sT> &src)
	{
		resize(src.mDim(),src.nDim());
		for(typename EMatrix<T>::size_type i=src.mDim();i;i--)
			operator[](i-1)=EVektor<T>(src[i-1]);
	}

	static EMatrix<T>	transp(EVektor<T> &Vekt)
	{
		EMatrix ret(Vekt.size(),1);
		for(typename EMatrix<T>::size_type i=Vekt.size();i;i--)
			ret[i-1][0]=Vekt[i-1];
		return ret;
	}
	static EMatrix<T>	transp(EMatrix<T> &Matr)
	{
		EMatrix ret(Matr[0].size(),Matr.size());
		for(typename EMatrix<T>::size_type m=Matr.size();m;m--)
			for(typename EMatrix<T>::size_type n=Matr[m-1].size();n;n--)
				ret[n-1][m-1]=Matr[m-1][n-1];
		return ret;
	}
	EMatrix<T> transp(){return transp(*this);}
	void settransp(){*this=transp();}

	EMatrix<T> operator*(EMatrix<T> Matr)//Fehler
	{
		Matr.settransp();
		EMatrix ret(mDim(),Matr.mDim());
		for(typename EMatrix<T>::size_type m=mDim();m;m--)
			for(typename EMatrix<T>::size_type n=Matr.mDim();n;n--)
			{
				T erg=operator[](m-1).scalarprod(Matr[n-1]);
				ret[m-1][n-1]=erg;
			}
		return ret;
	}
	EVektor<T> operator*(EVektor<T> Vekt)//Fehler
	{
		EVektor<T> ret(mDim(),1);
		for(typename EMatrix<T>::size_type m=mDim();m;m--)
		{
			ret[m-1]=operator[](m-1).skalarprod(Vekt);
		}
		return ret;
	}
	inline typename EMatrix<T>::size_type mDim()const{return this->size();}
	inline typename EMatrix<T>::size_type nDim()const{return (*this)[0].size();}//Fehler


	const EVektor<T> getzvekt(typename EMatrix<T>::size_type nr){return operator[](nr);}
	void setzvekt(EVektor<T> &Vekt,typename EMatrix<T>::size_type nr){operator[](nr)=Vekt;}

	const EVektor<T> getsvekt(typename EMatrix<T>::size_type nr)
	{
		EVektor<T>	ret(mDim());
		for(typename EMatrix<T>::size_type i=mDim();i;i--)
			ret[i-1]=operator[](i-1)[nr];
		return ret;
	}
	void setsvekt(EVektor<T> &Vekt,typename EMatrix<T>::size_type nr)
	{
		for(typename EMatrix<T>::size_type i=mDim();i;i--)
			operator[](i-1).operator[](nr)=Vekt[i-1];
	}

	virtual int elemtostr(char* buf,typename EMatrix<T>::size_type pos,const char* vparter="|")
	{
		return this->at(pos).sprint(buf,vparter);
	}
	int sprint(char* buf,const char* vparter="|",const char* mparter="\n")
	{
		int ret=0;
		typename EMatrix<T>::size_type s=mDim();

		for(typename EMatrix<T>::size_type i=0;i<s;i++)
		{
			ret+=elemtostr(&buf[ret],i,vparter);
			if(i<s-1)ret+=sprintf(&buf[ret],"%s",mparter);
		}
		return ret;
	}

	bool isNullVectorH(int m)
	{
		for(int n=0;n<nDim();n++)
			if((*this)[m][n]!=NULL)return false;
		return true;
	}

	bool isNullVectorV(int n)
	{
		for(int m=0;m<mDim();m++)
			if((*this)[m][n]!=NULL)return false;
		return true;
	}

	EMatrix<T> SubMatr(typename EMatrix<T>::size_type  Zeile,typename EMatrix<T>::size_type  Spalte)
	{
		EMatrix<T> ret(mDim(),nDim());

		for(typename EMatrix<T>::size_type  n=0;n<nDim();n++)
			if(n!=Spalte)for(typename EMatrix<T>::size_type  m=0;m<mDim();m++)
						if(m!=Zeile)ret[m][n] = (*this)[m][n];
		return ret;
	}
	EMatrix<T> RestMatrix(typename EMatrix<T>::size_type  Zeile,typename EMatrix<T>::size_type  Spalte)
	{
		EMatrix<T> ret(nDim()-Spalte,mDim()-Zeile);

		for(typename EMatrix<T>::size_type  n=Spalte;n<nDim();n++)
			for(typename EMatrix<T>::size_type  m=Zeile;m<mDim();m++)
				ret[m][n] = (*this)[m][n];
		return ret;
	}

	T ZeilenSumme(typename EMatrix<T>::size_type Zeile,bool Betrag=false,typename EMatrix<T>::size_type FirstSpalte=0)
	{return getzvekt(Zeile).summe(Betrag,FirstSpalte);	}

	T SpaltenSumme(typename EMatrix<T>::size_type Spalte,bool Betrag=false,typename EMatrix<T>::size_type FirstZeile=0)
	{return getsvekt(Spalte).summe(Betrag,FirstZeile);	}

};

}//namespace

#endif

