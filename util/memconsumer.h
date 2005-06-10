//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MEMCONSUMER_H
#define MEMCONSUMER_H

#include <list>
#include "../sglsignal.h"

/**
@author Enrico Reimer,,,
*/
class MemConsumer
{
	static SGLSignal<void (const MemConsumer&)> sigCreate,sigDelete;
	//Hat seine eigene referenz nich, soll auch keine neu anlegen - kann beides zu unangenehmen Effekten f�hren, lassen wir lieber
	//1: auto-l�schen von SGLshPtr geht nat nich, wenn das obj selbst einen SGLshPtr h�lt (deshalb auch ein "echter Ptr" in "list")
	//2: wenn destruktor durch SGLshPtr ausgel�st wird, und selbst einen SGLshPtr erz, haben wir nen SGLshPtr auf ein nicht mehr ex. obj => KABUM
	//3: Obj selbst kann keinen SGLshPtr von sich erz, wenn der seinen Kontext verl�sst hat sich das Ob damit selbst gekillt
public:
	template<class realClass> class NotifyCreateSlot :public SGLSlot{
	public:
		NotifyCreateSlot()
		{
			MemConsumer::sigCreate.connect(*((realClass*)this));
		}
		virtual void operator()(const MemConsumer &newob)const =0;
		virtual ~NotifyCreateSlot(){}
	};
	template<class realClass> class NotifyDeleteSlot :public SGLSlot{
	public:
		NotifyDeleteSlot()
		{
			MemConsumer::sigDelete.connect(*((realClass*)this));
		}
		virtual void operator()(const MemConsumer &newob)const =0;
		virtual ~NotifyDeleteSlot(){}
	};

	MemConsumer();
	virtual ~MemConsumer();
	static std::list<MemConsumer*> list;
};

#endif
