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

/**
@author Enrico Reimer,,,
*/
template<typename Signature> 
class SGLSignal : public boost::signal<Signature>
{
public:
    const SGLSignal & operator=( const SGLSignal & ){SGLprintDebug("Signale werden nicht kopiert");}
};

#endif
