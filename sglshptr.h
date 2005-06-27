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

/**
@author Enrico Reimer,,,
*/
template<class T> class SGLshPtr : public boost::shared_ptr<T>
{
public:
	typedef T basetype;
	SGLshPtr():boost::shared_ptr<T>(){}
	SGLshPtr(T *ob):boost::shared_ptr<T>(ob){}
	SGLshPtr(const boost::shared_ptr<T> &ob):boost::shared_ptr<T>(ob){}
	template<class D> SGLshPtr(const SGLshPtr<D> &ob):boost::shared_ptr<T>(ob){}

	template<class D> bool dcast(SGLshPtr<D> &ob,const bool check=true)const
	{
		boost::dynamic_pointer_cast<D>(*this).swap(ob);
		if(check && !ob){SGLprintWarning("SGLshPtr: dynamic_cast von %s nach %s gescheitert",typeid(T).name(),typeid(D).name());}
		return bool(ob);
	}
	template<class D> void scast(SGLshPtr<D> &ob)const
	{
		boost::static_pointer_cast<D>(*this).swap(ob);
	}
};

template<class T> SGLshPtr<T> SGLshPtr_new()
{
	return SGLshPtr<T>(new T);
}

template<class T,typename param1> SGLshPtr<T> SGLshPtr_new(param1 p1)
{
	return SGLshPtr<T>(new T(p1));
}

template<class T,typename param1,typename param2> SGLshPtr<T> SGLshPtr_new(param1 p1,param2 p2)
{
	return SGLshPtr<T>(new T(p2));
}

template<class D,class T> SGLshPtr<D> dcast(const SGLshPtr<T> &src,const bool check=true)
{
	SGLshPtr<D> ret;
	boost::dynamic_pointer_cast<D>(src).swap(ret);
	if(check && !ret){SGLprintWarning("SGLshPtr: dynamic_cast von %s nach %s gescheitert",typeid(T).name(),typeid(D).name());}
	return ret;
}
template<class D,class T> SGLshPtr<D> scast(const SGLshPtr<T> &src)
{
	SGLshPtr<D> ret;
	boost::static_pointer_cast<D>(src).swap(ret);
	return ret;
}

#endif
