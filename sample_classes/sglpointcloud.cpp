
#include <iostream>
#include <fstream>
#include <ctime>            // std::time

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_on_sphere.hpp>
#include <boost/random/uniform_real.hpp>

// Sun CC doesn't handle boost::iterator_adaptor yet
#if !defined(__SUNPRO_CC) || (__SUNPRO_CC > 0x530)
#include <boost/generator_iterator.hpp>
#endif

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std {
  using ::time;
}
#endif

#include "sglpointcloud.h"
#include <time.h>
#include <stdlib.h>
#include "../sglobj.h"
#include "../sglmisc.h"


QHDreiEck::QHDreiEck(SGLVektor *eins,SGLVektor *zwei,SGLVektor *drei):SGLDreiEck(eins,zwei,drei)
{
	Nachbar[0]=Nachbar[1]=Nachbar[2]=NULL;
	I=NULL;
	twoSideRender=false;
	DrahtGitter(true);
}

list<QHDreiEck>::iterator
QHDreiEck::newDreiEck(list<SGLVektor*> &punkte,list<QHDreiEck> *List,SGLVektor *eins,SGLVektor *zwei,SGLVektor *drei)
{
	list<QHDreiEck>::iterator ret=newDreiEck(List,eins,zwei,drei);
	ret->checkInPoints(punkte);
	return ret;
}

void QHDreiEck::checkInPoints(list<SGLVektor*> &punkte)
{
	for(list<SGLVektor*>::iterator it=punkte.begin();it!=punkte.end();it++)
	{
		SGLVektor* pkt=*it;
		if(	EckVektoren.Vekt[0]==pkt ||
			EckVektoren.Vekt[1]==pkt ||
			EckVektoren.Vekt[2]==pkt)
		continue;

		if(canSee(*pkt))
			myPunkte.push_back(pkt);
	}
}

list<QHDreiEck>::iterator
QHDreiEck::newDreiEck(list<QHDreiEck> *List,SGLVektor *eins,SGLVektor *zwei,SGLVektor *drei)
{
	list<QHDreiEck>::iterator it=List->insert(List->end(),QHDreiEck(eins,zwei,drei));
	it->I=it;
	return it;
}

bool QHDreiEck::addNachbar(QHDreiEck *Nachbar)
{
	unsigned int Kante;
	for(int i=0;i<3;i++)
	{
		if(Kante=Nachbar->hasKante(EckVektoren.Vekt[i],EckVektoren.Vekt[(i+1)%3]))
		{
			if(Nachbar->Nachbar[Kante-1]){SGLprintWarning("Der Nachbar %d ist schon belegt",Kante-1);}
			this->Nachbar[i]=Nachbar;
			Nachbar->Nachbar[Kante-1]=this;
			return true;
		}
	}
	return false;
}

bool QHDreiEck::delNachbar(QHDreiEck *Nachbar)
{
	for(int i=0;i<3;i++)
	{
		if(this->Nachbar[i]==Nachbar)
		{
			for(int n=0;n<3;n++)
			{
				if(Nachbar->Nachbar[n]==this)
					Nachbar->Nachbar[n]=NULL;
			}
			this->Nachbar[i]=NULL;
		}
	}
	return false;
}

unsigned int QHDreiEck::hasKante(SGLVektor *eins,SGLVektor *zwei)
{
	for(int i=0;i<3;i++)
	{
		if(EckVektoren.Vekt[i]==eins)
		{
			if(EckVektoren.Vekt[(i+1)%3]==zwei)
				return i+1;
			if(EckVektoren.Vekt[(i+2)%3]==zwei)
				return i?(i+4)%4:3;
		}
	}
	return 0;
}

QHDreiEck::~QHDreiEck()
{
	for(int i=0;i<3;i++)
		if(Nachbar[i])Nachbar[i]->delNachbar(this);
}

subSGLPointCloud::subSGLPointCloud(int PktCnt,GLdouble breite,GLdouble hoehe,GLdouble tiefe,GLdouble PosX,GLdouble PosY,GLdouble PosZ):
SGLFlObj(NULL,PosX,PosY,PosZ,1)
{
	this->breite=breite;
	this->hoehe=hoehe;
	this->tiefe=tiefe;
	IgnoreLight=true;

	DrahtGitter(true);
	if(PktCnt)make_rand_pts(PktCnt);
}

//macht zuf�lige Liste
void subSGLPointCloud::make_rand_pts(int cnt)
{
	if(punkte.size())punkte.clear();

	boost::minstd_rand generator(42u);
	boost::uniform_on_sphere<boost::minstd_rand,GLdouble,SGLVektor> uni(generator,3);

	for(int i=cnt;i;i--)
	{
		SGLVektor erg=uni();
		erg.SGLV_X*=breite;
		erg.SGLV_Y*=hoehe;
		erg.SGLV_Z*=tiefe;
		punkte.push_back(erg*(double(i)/double(cnt)));
	}
}

void subSGLPointCloud::generate()
{
	for(list<SGLVektor>::iterator it=punkte.begin();it!=punkte.end();it++)
		it->DrawPkt(.1);
}

/*!
    \fn subSGLPointCloud::getExtrema(SGLVektor &maxX, SGLVektor &minX,SGLVektor &maxY,SGLVektor &minY, SGLVektor &maxZ, SGLVektor &minZ)
 */
void subSGLPointCloud::getExtrema(SGLVektor *&maxX, SGLVektor *&minX,SGLVektor *&maxY,SGLVektor *&minY, SGLVektor *&maxZ, SGLVektor *&minZ)
{
	maxX=minX=maxY=minY=maxZ=minZ=&punkte.front();
	for(list<SGLVektor>::iterator it=punkte.begin();it!=punkte.end();it++)
	{
		if(it->SGLV_X<minX->SGLV_X)minX=&(*it);
		if(it->SGLV_X>maxX->SGLV_X)maxX=&(*it);

		if(it->SGLV_Y<minY->SGLV_Y)minY=&(*it);
		if(it->SGLV_Y>maxY->SGLV_Y)maxY=&(*it);

		if(it->SGLV_Z<minZ->SGLV_Z)minZ=&(*it);
		if(it->SGLV_Z>maxZ->SGLV_Z)maxZ=&(*it);
	}

}

void subSGLPointCloud::getTetraeder(SGLVektor *&eins, SGLVektor *&zwei,SGLVektor *&drei,SGLVektor *&vier)
{
	SGLVektor *maxX,*minX;
	SGLVektor *maxY,*minY;
	SGLVektor *maxZ,*minZ;

	getExtrema(maxX, minX,maxY,minY, maxZ, minZ);

	SGLVektor X(*maxX - *minX);
	SGLVektor Y(*maxY - *minY);
	SGLVektor Z(*maxZ - *minZ);

	SGLVektor *maxV;

	if(X.QLen() > Y.QLen()){maxV=&X;eins=minX;zwei=maxX;}
	else{maxV=&Y;eins=minY;zwei=maxY;}
	if(maxV->QLen() < Z.QLen()){maxV=&Z;eins=minZ;zwei=maxZ;}

	double maxSize=0;
	for(list<SGLVektor>::iterator it=punkte.begin();it!=punkte.end();it++)
	{
		if(&(*it) == eins || &(*it)==zwei)continue;
		double fl=maxV->kreuzprod(*it- *eins).QLen();
		if(fl>maxSize)
		{
			maxSize=fl;
			drei=&(*it);
		}
	}

	maxSize=0;
	for(list<SGLVektor>::iterator it=punkte.begin();it!=punkte.end();it++)
	{
		if(&(*it) == eins || &(*it)==zwei || &(*it)==drei)continue;
		double fl=maxV->spatprod(*drei- *eins, (*it)- *eins);
		if(fl>maxSize)
		{
			maxSize=fl;
			vier=&(*it);
		}
	}
}

SGLVektor subSGLPointCloud::getCenter()
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}

subSGLPointCloud::~subSGLPointCloud(){}

SGLPointCloud::SGLPointCloud(int PktCnt,GLdouble breite,GLdouble hoehe,GLdouble tiefe,short int mode):
SGLMetaObj(0,0,0,1)
{
	cloud=new subSGLPointCloud(PktCnt,breite,hoehe,tiefe);
	this->mode=mode;
	growCnt=0;
	rdy=false;
	step=true;
	Mat1;
	Mat2.SetColor(0,255,0,GL_FRONT);
	Mat3.SetColor(0,0,255,GL_FRONT);
	mode==2 ? init2():init();
	compileSubObjects();
}

SGLPointCloud::~SGLPointCloud()
{
	if(cloud)delete cloud;
}

void SGLPointCloud::compileSubObjects()
{
	Objs.clear();
	TrObjs.clear();
	Objs.push_back(cloud->Compile());

	cout << distance(seiten.begin(), seiten.end())<<" Dreiecke" << endl;
	for_each(seiten.begin(), seiten.end(), CompObj<SGLFlObj>(&Objs));
}

SGLVektor SGLPointCloud::getCenter()
{
	return cloud->getCenter();
}


/*!
    \fn SGLPointCloud::init()
 */
void SGLPointCloud::init()
{
	SGLVektor *pt1,*pt2,*pt3,*pt4;
	//vier m�lichst weit voneinander entfernte Punkte finden
	cloud->getTetraeder(pt1,pt2,pt3,pt4);

	seiten.clear();
	//Wenn Seite aus p1,p2 und p3 den vierten Punkt sehen kann
	if(QHDreiEck(pt1,pt2,pt3).canSee(*pt4))
		QHDreiEck::newDreiEck(&seiten,pt1,pt3,pt2);//sie anders herum eintragen
	else
		QHDreiEck::newDreiEck(&seiten,pt1,pt2,pt3);


	if(QHDreiEck(pt1,pt2,pt4).canSee(*pt3))//zweite Seite
		QHDreiEck::newDreiEck(&seiten,pt1,pt4,pt2);
	else
		QHDreiEck::newDreiEck(&seiten,pt1,pt2,pt4);

	list<QHDreiEck>::iterator it=seiten.begin();
	it->addNachbar(&seiten.back()); //erste und zweite Seite "bekannt machen"

	if(QHDreiEck(pt1,pt3,pt4).canSee(*pt2))//dritte Seite
		QHDreiEck::newDreiEck(&seiten,pt1,pt4,pt3);
	else
		QHDreiEck::newDreiEck(&seiten,pt1,pt3,pt4);

	it->addNachbar(&seiten.back());//erste und dritte Seite "bekannt machen"
	(++it)->addNachbar(&seiten.back());//zweite und dritte Seite "bekannt machen"

	if(QHDreiEck(pt2,pt3,pt4).canSee(*pt1))//vierte Seite
		QHDreiEck::newDreiEck(&seiten,pt2,pt4,pt3);
	else
		QHDreiEck::newDreiEck(&seiten,pt2,pt3,pt4);

	(it=seiten.begin())->addNachbar(&seiten.back());//erste und vierte Seite "bekannt machen"
	(++it)->addNachbar(&seiten.back());//zweite und vierte Seite "bekannt machen"
	(++it)->addNachbar(&seiten.back());//dritte und vierte Seite "bekannt machen"

	growBase=seiten.begin();
	compileNextTime();
}

void SGLPointCloud::init2()
{
	init();
	list<SGLVektor*> tList;
	for(list<SGLVektor>::iterator it=cloud->punkte.begin();it!=cloud->punkte.end();it++)
		tList.push_back(&*it);

	for(list<QHDreiEck>::iterator it=seiten.begin();it!=seiten.end();it++)
		it->checkInPoints(tList);
}

/*!
    \fn SGLPointCloud::grow
 */
double SGLPointCloud::grow()
{
	if(step)for(list<QHDreiEck>::iterator it=seiten.begin();it!=seiten.end();it++)
	{
		it->resetMaterial(&Mat1);
		it->DrahtGitter(false);
		it->twoSideRender=true;
	}

	QHDreiEck *dEck=&(*growBase);
	SGLprintInfo("%d ter grow-Aufruf",++growCnt);

	SGLVektor *maxPkt=NULL;
	queue<QHDreiEck*> offen;
	queue<kante> kanten;
	//Punkt mit m�lichst gro�r Entf zur Fl�he suchen
	double maxEntf=0;
	bool sawn;
	list<SGLVektor>::iterator it=cloud->punkte.begin();
	int itCnt=0;
	while(it!=cloud->punkte.end())
	{
		itCnt++;
		SGLVektor *pkt=&*it;
		sawn=false;
		//Punke die zum dreieck geh�en sind unintressant
		if(	dEck->EckVektoren.Vekt[0]==pkt ||
			dEck->EckVektoren.Vekt[1]==pkt ||
			dEck->EckVektoren.Vekt[2]==pkt)sawn=true;
		else
		{
			double entf=dEck->spat(*pkt);
			if(mode == 1 && entf<0)
			//Punkt liegt auf Falscher Seite - k�nte m�licherweise weg
			{
				for(list<QHDreiEck>::iterator itDr=seiten.begin();itDr!=seiten.end();itDr++)
				{
					if(dEck==&(*itDr))continue;
					if(itDr->EckVektoren.Vekt[0]==pkt ||
						itDr->EckVektoren.Vekt[1]==pkt ||
						itDr->EckVektoren.Vekt[2]==pkt)
					{
						sawn=true;
						break;
					}
					if(itDr->canSee(*pkt))
					{
						sawn=true;
						break;
					}
				}
			}
			else
			{
				if(entf>maxEntf)
				{
					maxEntf=entf;
					maxPkt=pkt;
				}
				sawn=true;
			}
		}
		if(mode!=1)it++;
		else
		{
			if(sawn)it++;
			else it=cloud->punkte.erase(it);
		}
		cloud->compileNextTime();
	}

	if(maxPkt)
	{
		DreiEck2Kante(dEck,maxPkt,dEck->EckVektoren.Vekt[0],&kanten);

		QHDreiEck *lastDr=NULL;
		QHDreiEck *firstDr=NULL;

		SGLprintInfo("%d Kanten gefunden",kanten.size());
		for(;!kanten.empty();kanten.pop())
		{
			kante kt=kanten.front();
			QHDreiEck::newDreiEck(&seiten,kt.start,kt.end,maxPkt);
			QHDreiEck *inserted=&seiten.back();
			SGLprintInfo("Dreieck %d: hinzugef",inserted->ID);

			inserted->addNachbar(kt.nachbar);
			if(lastDr)
				inserted->addNachbar(lastDr);
			else firstDr=inserted;
			lastDr=inserted;
		}
		firstDr->addNachbar(lastDr);
	}
/*	for(list<QHDreiEck>::iterator it=seiten.begin();it!=seiten.end();it++)
	{
		for(int i=0;i<3;i++)if(!it->Nachbar[i])
		{
			SGLprintError("Dem Dreieck %d fehlt der %dte Nachbar",it->ID,i);
		}
	}*/
	return maxPkt ? maxEntf:0;
}

/*!
    \fn SGLPointCloud::grow
 */
double SGLPointCloud::grow2()
{
	if(step)for(list<QHDreiEck>::iterator it=seiten.begin();it!=seiten.end();it++)
	{
		it->resetMaterial(&Mat1);
		it->DrahtGitter(false);
		it->twoSideRender=true;
	}

	QHDreiEck *dEck=&(*growBase);
	SGLprintInfo("%d ter grow-Aufruf",++growCnt);

	SGLVektor *maxPkt=NULL;
	queue<QHDreiEck*> offen;
	queue<kante> kanten;
	//Punkt mit m�lichst gro�r Entf zur Fl�he suchen
	double maxEntf=0;


	for(list<SGLVektor*>::iterator it=dEck->myPunkte.begin();
		it!=dEck->myPunkte.end();
		it++)
	{
		SGLVektor *pkt=*it;
		//Punke die zum dreieck geh�en sind unintressant
		double entf=dEck->spat(*pkt);
		if(entf>maxEntf)
		{
			maxEntf=entf;
			maxPkt=pkt;
		}
	}

	if(maxPkt)
	{
		list<SGLVektor*> newPktList;
		DreiEck2Kante(newPktList,dEck,maxPkt,dEck->EckVektoren.Vekt[0],&kanten);

		QHDreiEck *lastDr=NULL;
		QHDreiEck *firstDr=NULL;

		SGLprintInfo("%d Kanten gefunden",kanten.size());
		for(;!kanten.empty();kanten.pop())
		{
			kante kt=kanten.front();
			QHDreiEck::newDreiEck(newPktList,&seiten,kt.start,kt.end,maxPkt);
			QHDreiEck *inserted=&seiten.back();
			SGLprintInfo("Dreieck %d mit %d beachteten Punkten hinzugef",inserted->ID,inserted->myPunkte.size());

			inserted->addNachbar(kt.nachbar);
			if(lastDr)
				inserted->addNachbar(lastDr);
			else firstDr=inserted;
			lastDr=inserted;
		}
		firstDr->addNachbar(lastDr);
	}
	return maxPkt ? maxEntf:0;
}



void SGLPointCloud::DreiEck2Kante(QHDreiEck *dEck,SGLVektor *maxPkt,SGLVektor *start,queue<kante> *kanten)
{
	int shift;
	for(shift=0;shift<3;shift++)
		if(dEck->EckVektoren.Vekt[shift]==start)break;
	if(shift>=3)
	{
		SGLprintError("Der Startpunkt 0x%X fehlt in dem Dreieck %d",start,dEck->ID);
	}
	for(int i=0;i<3;i++)
	{
		QHDreiEck *kantenNachbar=dEck->Nachbar[(i+shift)%3];
		if(kantenNachbar)
			kantenNachbar->delNachbar(dEck);
		else continue;
		kante kt=
		{
			dEck->EckVektoren.Vekt[(i+shift)%3],
			dEck->EckVektoren.Vekt[(i+shift+1)%3],
			kantenNachbar
		};
		if(kantenNachbar->canSee(*maxPkt))
			DreiEck2Kante(kantenNachbar,maxPkt,kt.start,kanten);
		else
		{
			SGLprintInfo("Kante 0x%X => 0x%X neu",kt.start,kt.end );
			kanten->push(kt);
		}
	}
	if(growBase==dEck->I)growBase++;
	seiten.erase(dEck->I);
}

void SGLPointCloud::DreiEck2Kante(list<SGLVektor*> &punkte,QHDreiEck *dEck,SGLVektor *maxPkt,SGLVektor *start,queue<kante> *kanten)
{
	int shift;
	for(shift=0;shift<3;shift++)
		if(dEck->EckVektoren.Vekt[shift]==start)break;
	if(shift>=3)
	{
		SGLprintError("Der Startpunkt 0x%X fehlt in dem Dreieck %d",start,dEck->ID);
	}
	for(int i=0;i<3;i++)
	{
		QHDreiEck *kantenNachbar=dEck->Nachbar[(i+shift)%3];
		if(kantenNachbar)
			kantenNachbar->delNachbar(dEck);
		else continue;
		kante kt=
		{
			dEck->EckVektoren.Vekt[(i+shift)%3],
			dEck->EckVektoren.Vekt[(i+shift+1)%3],
			kantenNachbar
		};
		if(kantenNachbar->canSee(*maxPkt))
			DreiEck2Kante(kantenNachbar,maxPkt,kt.start,kanten);
		else
		{
			SGLprintInfo("Kante 0x%X => 0x%X neu",kt.start,kt.end );
			kanten->push(kt);
		}
	}
	if(growBase==dEck->I)growBase++;
	SGLprintInfo("L�che Dreieck %d, es hatte %d Punkte zu beachten",dEck->ID,dEck->myPunkte.size());
	punkte.insert(punkte.end(), dEck->myPunkte.begin(), dEck->myPunkte.end());
	SGLprintInfo("Es gibt jetzt %d \"Waisen\"",punkte.size());

	seiten.erase(dEck->I);
}

/*!
    \fn SGLPointCloud::on_mouse(SGLObjBase *target,SDL_Event event)
 */
void SGLPointCloud::do_grow(SGLObjBase *target,SDL_Event event)
{
	SGLPointCloud* T=((SGLPointCloud*)target);
	if(event.key.keysym.sym==SDLK_SPACE && !T->rdy)
	{
		do
		{
			cout << "==========================================================="<<endl;
			cout << T->seiten.size() << " Dreiecke, " << T->cloud->punkte.size() << " Punkte" << endl;
			double gef= (T->mode == 2) ? T->grow2():T->grow();
			if(gef && T->step)cout << "Punkt mit dem \"Abstand\" " << gef << " gefunden"<< endl;
			else
			{
				if(T->step)cout << "nix gefunden"<< endl;
				T->growBase++;
				if(T->growBase==T->seiten.end())
				{
					cout << "Das wars ..." << endl;
					T->rdy=true;
				}
				else cout << "gehe zu " << distance(T->seiten.begin(),T->growBase) << "tem Dreieck" << endl;
			}
		}while(!T->step && !T->rdy);

		if(!T->rdy)
		{
			T->growBase->resetMaterial(&T->Mat2);
			for(int i=0;i<3;i++)
				T->growBase->Nachbar[i]->resetMaterial(&T->Mat3);
		}
		else if(!T->step)
		{
			for(list<QHDreiEck>::iterator it=T->seiten.begin();it!=T->seiten.end();it++)
			{
				it->resetMaterial(&T->Mat1);
				it->DrahtGitter(false);
				it->twoSideRender=true;
			}
		}
		T->compileSubObjects();
		T->compileNextTime();
	}
}
