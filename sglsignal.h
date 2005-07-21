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

/**
 * simpleGL-Slot-Klasse.
 * Die Klasse entspricht der Spezifikation f�r Slots in boost::signals.
 * Von dieser Klasse abgeleitete Klassen lassen sich mittels \code SGLSignal::connect \endcode an ein Objekt binden, das von \code SGLSignal \endcode abgeleitet ist.
 * Sie m�ssen den \code operator(...) \endcode �berladen, dieser wird ausgel�st, wenn ein verbundenes Signal ausgel�st wird.
 * Objekte dieser Klasse sollten nicht kopiert werden, da sie oft Zeiger halten. (\code SGLSlot \endcode beinhaltet jedoch keine Zeiger)
 */
class SGLSlot:public trackable
{
public:
	virtual const SGLSlot& operator=( const SGLSlot & ){
		SGLprintDebug("Achtung, kopiere Slot. Wenn der kopierte Slot einen Zeiger h�lt kann das seeeehr unangenehm werden.");
		debugSig();
		return *this;
	}
	virtual ~SGLSlot(){};
};

/**
 * Container f�r eine Signal-Slot-Verbindung
 */
typedef connection SGLConnection; //@todo d�rfen connections kopiert werden??

/**
 * simpleGL-Signal-Klasse.
 * Diese Klasse erbt von boost::signal\<Signature\> . 
 * Sie l�sst sich mittels SGLSignal::connect an ein Objekt binden, das von SGLSlot abgeleitet ist.
 * SGLSignal-Objekte werden nicht kopiert. Sind sie Member eines kopierten Objektes wird die Kopie des Objektes ein neu initialisiertes SGLSignal halten.
 */
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
	/**
	 * Standardkonstruktor.
	 * Erzeugt ein Signalobjekt.
	 */
	SGLSignal():fwd(*this){};
	const SGLSignal & operator=( const SGLSignal & ){
		SGLprintDebug("Signale werden nicht kopiert");
		debugSig();
		return *this;
	}
	/**
	 * Forwarder f�r einfache Signale. Das Objekt leitet Signale von \code src \endcode an seine eigenen Slots weiter.
	 * Parameter k�nnen nicht weitergeleitet werden, d.H. steht diese Funktion nur f�r parameterlose \code void() \endcode Signale zur Verf�gung.
	 * @param src SGLSignal-Objekt, dessen Signale weitergeleitet werden sollen.
	 * @return 
	 */
	template<typename T> SGLConnection forward(SGLSignal<T> &src){//@todo geht forwarden einer anderen Signatur ?!
		return src.connect(fwd);
	}
};
// MOC_SKIP_END
#endif
