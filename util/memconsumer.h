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
#include <boost/signal.hpp>

/**
@author Enrico Reimer,,,
*/
class MemConsumer
{
	static std::list<MemConsumer*> list;
	static boost::signal<void (boost::shared_ptr<MemConsumer>)> sigCreate;
	static boost::signal<void (MemConsumer*)> sigDelete;//Hat seine eigene referenz nich, soll auch keine neu anlegen - kann beides zu unangenehmen Effekten f�hren, lassen wir lieber
	//1: auto-l�schen von shared_ptr geht nat nich, wenn das obj selbst einen shared_ptr h�lt (deshalb auch eine shared_ptr in "list")
	//2: wenn destruktor durch shared_ptr ausgel�st wird, und selbst einen shared_ptr erz, haben wir nen shred_ptr auf ein nicht mehr ex. obj => KABUM
public:
	class NotifyCreateSlot :public boost::signals::trackable{
		virtual void operator()(boost::shared_ptr<MemConsumer> newob) const =NULL;
	};
	class NotifyDeleteSlot :public boost::signals::trackable{
		virtual void operator()(MemConsumer *newob) const =NULL;
	};

	MemConsumer();
	virtual ~MemConsumer();
};

#endif
