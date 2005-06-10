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
	//Hat seine eigene referenz nich, soll auch keine neu anlegen - kann beides zu unangenehmen Effekten führen, lassen wir lieber
	//1: auto-löschen von SGLshPtr geht nat nich, wenn das obj selbst einen SGLshPtr hält (deshalb auch ein "echter Ptr" in "list")
	//2: wenn destruktor durch SGLshPtr ausgelöst wird, und selbst einen SGLshPtr erz, haben wir nen SGLshPtr auf ein nicht mehr ex. obj => KABUM
	//3: Obj selbst kann keinen SGLshPtr von sich erz, wenn der seinen Kontext verlässt hat sich das Ob damit selbst gekillt
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
