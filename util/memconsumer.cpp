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
	sigCreate(*this);
}


MemConsumer::~MemConsumer()
{
	MemConsumer::list.remove(this);
	sigDelete(*this);
}

std::list<MemConsumer*> MemConsumer::list;
SGLSignal<void (const MemConsumer&)> MemConsumer::sigCreate,MemConsumer::sigDelete;
//Hat seine eigene referenz nich, soll auch keine neu anlegen - kann beides zu unangenehmen Effekten führen, lassen wir lieber
