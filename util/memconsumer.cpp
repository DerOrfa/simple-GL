//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "memconsumer.h"

MemConsumer::MemConsumer()
{
	MemConsumer::list.push_back(this);
	sigCreate(boost::shared_ptr<MemConsumer>(this));
}


MemConsumer::~MemConsumer()
{
	MemConsumer::list.remove(this);
	sigDelete(this);
}


std::list<MemConsumer*> MemConsumer::list;
boost::signal<void (boost::shared_ptr<MemConsumer>)> MemConsumer::sigCreate;
boost::signal<void (MemConsumer*)> MemConsumer::sigDelete;//Hat seine eigene referenz nich, soll auch keine neu anlegen - kann beides zu unangenehmen Effekten führen, lassen wir lieber
