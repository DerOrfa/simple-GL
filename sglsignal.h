//
// C++ Interface: sglsignal
//
// Description: 
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLSIGNAL_H
#define SGLSIGNAL_H

#include <boost/signal.hpp>
#include "sglmisc.h"

/**
@author Enrico Reimer,,,
*/

#ifdef signals
#define QT_KONFLIKT
#endif
#ifdef slots
#define QT_KONFLIKT
#endif
#ifdef QT_KONFLIKT
#warning QT versucht gerade (warscheinlich erfolgreich) SGLSignals "kaputtzumachen"
#warning du solltest libsgl-header immer vor QT headern laden - das hilft MANCHMAL
#endif

// MOC_SKIP_BEGIN
using namespace boost::signals;
class SGLSlot:public trackable
{
public:
    const SGLSlot & operator=( const SGLSlot & ){
		SGLprintDebug("Achtung, kopiere Slot. Wenn der kopierte Slot einen Zeiger hält kann das seeeehr unangenehm werden.");
		return *this;
	}
};
typedef connection SGLConnection; //@todo dürfen connections kopiert werden??

template<typename Signature> 
class SGLSignal : public boost::signal<Signature>
{
	class forwarder : public SGLSlot{
		public:
			forwarder(const SGLSignal<Signature> &Sig):mySig(Sig){};
			const SGLSignal<Signature> &mySig;
			void operator()() const{mySig();};
	}fwd;
public:
	SGLSignal():fwd(*this){};
    const SGLSignal & operator=( const SGLSignal & ){SGLprintDebug("Signale werden nicht kopiert");return *this;}
	template<typename T> SGLConnection forward(SGLSignal<T> &src){//@todo geht forwarden einer anderen Signatur ?!
		return src.connect(fwd);
	}
};
// MOC_SKIP_END
#endif
