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
#include <boost/shared_ptr.hpp>
#include "../sglsignal.h"

/**
@author Enrico Reimer,,,
*/
class MemConsumer
{
	static SGLSignal<void (const MemConsumer&)> sigCreate,sigDelete;
	//Hat seine eigene referenz nich, soll auch keine neu anlegen - kann beides zu unangenehmen Effekten führen, lassen wir lieber
	//1: auto-löschen von shared_ptr geht nat nich, wenn das obj selbst einen shared_ptr hält (deshalb auch eine shared_ptr in "list")
	//2: wenn destruktor durch shared_ptr ausgelöst wird, und selbst einen shared_ptr erz, haben wir nen shred_ptr auf ein nicht mehr ex. obj => KABUM
	//3: Obj selbst kann keinen shared_ptr von sich erz, wenn der seinen Kontext verlässt hat sich das Ob damit selbst gekillt
public:
	template<class realClass> class NotifyCreateSlot :public SGLSlot{
	public:
		NotifyCreateSlot()
		{
			MemConsumer::sigCreate.connect(*((realClass*)this));
		}
		virtual void operator()(const MemConsumer &newob) const =NULL;
		virtual ~NotifyCreateSlot(){}
	};
	template<class realClass> class NotifyDeleteSlot :public SGLSlot{
	public:
		NotifyDeleteSlot()
		{
			MemConsumer::sigDelete.connect(*((realClass*)this));
		}
		virtual void operator()(const MemConsumer &newob) const =NULL;
		virtual ~NotifyDeleteSlot(){}
	};

	MemConsumer();
	virtual ~MemConsumer();
	static std::list<MemConsumer*> list;
};

#endif
