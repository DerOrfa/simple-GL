#include "sglpointcloud.h"
#include <time.h>
#include <stdlib.h>
#include "../sglobj.h"
#include <map>
#include <queue>
#include "../sglmisc.h"

struct kante{SGLVektor *start,*end;QHDreiEck *nachbar;};

QHDreiEck::QHDreiEck(SGLVektor *eins,SGLVektor *zwei,SGLVektor *drei):SGLDreiEck(eins,zwei,drei)
{
	Nachbar[0]=Nachbar[1]=Nachbar[2]=NULL;
	twoSideRender=false;
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

	DrahtGitter(true);
	punkte=NULL;
	if(PktCnt)
	{
		make_rand_pts(PktCnt);
//		qsort(punkte,PktCnt,sizeof(SGLVektor),sortZ);
	}
}

int subSGLPointCloud::make_rand(int seed)
{
	int ret;
	while((ret=rand())==_old_rnd)
		srand(seed);
	_old_rnd=ret;
	return ret;
}

//macht zufällige Liste
void subSGLPointCloud::make_rand_pts(int cnt)
{
	int seed=time(0);
	if(punkte)delete punkte;

	punkte= new SGLVektor[cnt];
	for(int i=cnt;i;i--)
	{
		double x=make_rand(seed++)*(breite/(double)RAND_MAX);
		double y=make_rand(seed++)*(hoehe/(double)RAND_MAX);
		double z=make_rand(seed++)*(tiefe/(double)RAND_MAX);
		punkte[i-1]=SGLVektor(x-breite/2,y-hoehe/2,z-tiefe/2);
	}
	pktCnt=cnt;
}

void subSGLPointCloud::generate()
{
	for(int i=0;i<pktCnt;i++)punkte[i].DrawPkt(.1);
}

/*!
    \fn subSGLPointCloud::getExtrema(SGLVektor &maxX, SGLVektor &minX,SGLVektor &maxY,SGLVektor &minY, SGLVektor &maxZ, SGLVektor &minZ)
 */
void subSGLPointCloud::getExtrema(SGLVektor *&maxX, SGLVektor *&minX,SGLVektor *&maxY,SGLVektor *&minY, SGLVektor *&maxZ, SGLVektor *&minZ)
{
	maxX=minX=maxY=minY=maxZ=minZ=&punkte[0];
	for(int i=1;i<pktCnt;i++)
	{
		if(punkte[i].SGLV_X<minX->SGLV_X)minX=&punkte[i];
		if(punkte[i].SGLV_X>maxX->SGLV_X)maxX=&punkte[i];
	
		if(punkte[i].SGLV_Y<minY->SGLV_Y)minY=&punkte[i];
		if(punkte[i].SGLV_Y>maxY->SGLV_Y)maxY=&punkte[i];
	
		if(punkte[i].SGLV_Z<minZ->SGLV_Z)minZ=&punkte[i];
		if(punkte[i].SGLV_Z>maxZ->SGLV_Z)maxZ=&punkte[i];
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
	for(int i=0;i<pktCnt;i++)
	{
		if(&punkte[i] == eins || &punkte[i]==zwei)continue;
		double fl=maxV->kreuzprod(punkte[i]- *eins).QLen();
		if(fl>maxSize)
		{
			maxSize=fl;
			drei=&punkte[i];
		}
	}
	
	maxSize=0;
	for(int i=0;i<pktCnt;i++)
	{
		if(&punkte[i] == eins || &punkte[i]==zwei || &punkte[i]==drei)continue;
		double fl=maxV->spatprod(*drei- *eins, punkte[i]- *eins);
		if(fl>maxSize)
		{
			maxSize=fl;
			vier=&punkte[i];
		}
	}
}

SGLVektor subSGLPointCloud::getCenter()
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}

subSGLPointCloud::~subSGLPointCloud(){}

SGLPointCloud::SGLPointCloud(int PktCnt,GLdouble breite,GLdouble hoehe,GLdouble tiefe,GLdouble PosX,GLdouble PosY,GLdouble PosZ):
SGLMetaObj(PosX,PosY,PosZ,1)
{
	cloud=new subSGLPointCloud(PktCnt,breite,hoehe,tiefe,PosX,PosY,PosZ);
	init();
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
	for_each(seiten.begin(), seiten.end(), compObj<SGLFlObj>(&Objs));
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
	//vier möglichst weit voneinander entfernte Punkte finden
	cloud->getTetraeder(pt1,pt2,pt3,pt4);
	
	seiten.clear();
	//Wenn Seite aus p1,p2 und p3 den vierten Punkt sehen kann
	if(QHDreiEck(pt1,pt2,pt3).canSee(*pt4))
		seiten.push_back(QHDreiEck(pt1,pt3,pt2));//sie anders herum eintragen
	else
		seiten.push_back(QHDreiEck(pt1,pt2,pt3));
	
	if(QHDreiEck(pt1,pt2,pt4).canSee(*pt3))//zweite Seite
		seiten.push_back(QHDreiEck(pt1,pt4,pt2));
	else
		seiten.push_back(QHDreiEck(pt1,pt2,pt4));
	
	list<QHDreiEck>::iterator it=seiten.begin();
	it->addNachbar(&seiten.back()); //erste und zweite Seite "bekannt machen"

	if(QHDreiEck(pt1,pt3,pt4).canSee(*pt2))//dritte Seite
		seiten.push_back(QHDreiEck(pt1,pt4,pt3));
	else
		seiten.push_back(QHDreiEck(pt1,pt3,pt4));

	it->addNachbar(&seiten.back());//erste und dritte Seite "bekannt machen"
	(++it)->addNachbar(&seiten.back());//zweite und dritte Seite "bekannt machen"
	
	if(QHDreiEck(pt2,pt3,pt4).canSee(*pt1))//vierte Seite
		seiten.push_back(QHDreiEck(pt2,pt4,pt3));
	else
		seiten.push_back(QHDreiEck(pt2,pt3,pt4));
	
	(it=seiten.begin())->addNachbar(&seiten.back());//erste und vierte Seite "bekannt machen"
	(++it)->addNachbar(&seiten.back());//zweite und vierte Seite "bekannt machen"
	(++it)->addNachbar(&seiten.back());//dritte und vierte Seite "bekannt machen"
	
	compileNextTime();
}

/*!
    \fn SGLPointCloud::grow
 */
int SGLPointCloud::grow(list<QHDreiEck>::iterator seite)
{
	QHDreiEck *dEck=&(*seite);
		
	SGLVektor *maxPkt=NULL;
	queue<QHDreiEck*> offen;
	queue<kante> kanten;
	//Punkt mit möglichst großer Entf zur Fläche suchen
	{
	double maxEntf=0;
	for(int i=0;i<cloud->pktCnt;i++)
	{
		//@todo Punke die zum dreieck gehören sind unintressant
		SGLVektor stuetzV(*dEck->EckVektoren.Vekt[0]);
		SGLVektor aim(cloud->punkte[i]-stuetzV);
		SGLVektor V1(*dEck->EckVektoren.Vekt[dEck->EckVektoren.Cnt-1]-stuetzV);
		SGLVektor V2(*dEck->EckVektoren.Vekt[1]-stuetzV);
		double entf=-V1.spatprod(V2,aim);
		
		if(entf>maxEntf)
		{
			maxEntf=entf;
			maxPkt=&cloud->punkte[i];
		}
		else if(entf<0)
		//Punkt liegt auf Falscher Seite - könnte möglicherweise weg
		{}
	}
	}
	
	if(maxPkt)
	{
		for(offen.push(dEck);!offen.empty();offen.pop())
		{
			dEck=offen.front();
			for(int i=0;i<3;i++)
			{
				QHDreiEck *kantenNachbar=dEck->Nachbar[i];
				if(!kantenNachbar)continue;
				if(kantenNachbar->canSee(*maxPkt))
					offen.push(kantenNachbar);
				else
				{
					kante kt=
					{
						dEck->EckVektoren.Vekt[i],
						dEck->EckVektoren.Vekt[(i+1)%3],
						kantenNachbar
					};
					kantenNachbar->delNachbar(dEck);
					kanten.push(kt);
				}
			}
			for(list<QHDreiEck>::iterator it=seiten.begin();it!=seiten.end();it++)
			{
				if(&(*it)==dEck)
				{
					seiten.erase(it);
					break;
				}
			}
		}
		
		QHDreiEck *lastDr=NULL;
		QHDreiEck *firstDr=NULL;
		for(;!kanten.empty();kanten.pop())
		{
			kante kt=kanten.front();
			seiten.push_back(QHDreiEck(kt.start,kt.end,maxPkt));
			QHDreiEck *inserted=&seiten.back();
			inserted->addNachbar(kt.nachbar);
			if(lastDr)inserted->addNachbar(lastDr);
			else firstDr=inserted;
			lastDr=inserted;
		}
		firstDr->addNachbar(lastDr);
		compileSubObjects();
		compileNextTime();
	}
	return maxPkt ? 1:0;
}

/*!
    \fn SGLPointCloud::on_mouse(SGLObjBase *target,SDL_Event event)
 */
void SGLPointCloud::on_mouse(SGLObjBase *target,SDL_Event event)
{
	cout << (((SGLPointCloud*)target)->grow(((SGLPointCloud*)target)->seiten.begin()) ? "Gefunden":"nicht Gefunden") << endl;
}
