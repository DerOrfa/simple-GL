//
// C++ Implementation: sglpointcloud
//
// Description:
//
//
// Author:  <enni@Willow.Reimer>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sglpointcloud.h"
#include <time.h>
#include <stdlib.h>
#include "../sglobj.h"


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
		qsort(punkte,PktCnt,sizeof(SGLVektor),sortZ);
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
	for(int i=cnt;i>0;i--)
	{
		double x=make_rand(seed++)*(breite/(double)RAND_MAX);
		double y=make_rand(seed++)*(hoehe/(double)RAND_MAX);
		double z=make_rand(seed++)*(tiefe/(double)RAND_MAX);
		punkte[i]=SGLVektor(x-breite/2,y-hoehe/2,z-tiefe/2);
	}
	pktCnt=cnt;
}

void subSGLPointCloud::generate()
{
	for(int i=0;i<pktCnt;i++)punkte[i].DrawPkt(.1);
}

SGLVektor subSGLPointCloud::getCenter()
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}

subSGLPointCloud::~subSGLPointCloud(){}

int subSGLPointCloud::sortX(const void * P1,const void *P2)
{
	if(((SGLVektor*)P1)->SGLV_X<((SGLVektor*)P2)->SGLV_X)return -1;
	else if(((SGLVektor*)P1)->SGLV_X>((SGLVektor*)P2)->SGLV_X)return 1;
	else return 0;
}

int subSGLPointCloud::sortY(const void * P1,const void *P2)
{
	if(((SGLVektor*)P1)->SGLV_Y<((SGLVektor*)P2)->SGLV_Y)return -1;
	else if(((SGLVektor*)P1)->SGLV_Y>((SGLVektor*)P2)->SGLV_Y)return 1;
	else return 0;
}

int subSGLPointCloud::sortZ(const void * P1,const void *P2)
{
	if(((SGLVektor*)P1)->SGLV_Z<((SGLVektor*)P2)->SGLV_Z)return -1;
	else if(((SGLVektor*)P1)->SGLV_Z>((SGLVektor*)P2)->SGLV_Z)return 1;
	else return 0;
}

SGLVektor * subSGLPointCloud::splitList(SGLVektor List[],unsigned int cnt,unsigned int &cnt1,unsigned int &cnt2)
{
	cnt1=cnt/2;
	cnt2=cnt-cnt1;
	return List+cnt1;
}


SGLPointCloud::SGLPointCloud(int PktCnt,GLdouble breite,GLdouble hoehe,GLdouble tiefe,GLdouble PosX,GLdouble PosY,GLdouble PosZ):
SGLMetaObj(PosX,PosY,PosZ,1)
{
	cloud=new subSGLPointCloud(PktCnt,breite,hoehe,tiefe,PosX,PosY,PosZ);
	quader=new SGLQuader(NULL,breite,hoehe,tiefe,PosX,PosY,PosZ);
	marker=new SGLQuader(NULL,tiefe,hoehe);
	grenze=new SGLRechtEck(tiefe,hoehe);

	quader->DrahtGitter(true);
	marker->DrahtGitter(true);

//	marker->setDiag(SGLVektor(-breite/3,-hoehe/3),SGLVektor(breite/5,hoehe/5,tiefe/6),false);
	marker->Mat->SetColor(0,0,255);
	grenze->Mat->Transparenz=.9;
	grenze->Mat->SetColor(0,255,0);

	divide(cloud->punkte,cloud->pktCnt);
	compileSubObjects();
}

SGLPointCloud::~SGLPointCloud()
{
	if(cloud)delete cloud;
	if(quader)delete quader;
}

void SGLPointCloud::compileSubObjects()
{
	Objs.clear();
	TrObjs.clear();
	Objs.push_back(cloud->Compile());
	Objs.push_back(quader->Compile());
	Objs.push_back(marker->Compile());
	for(int i=hilfsLinien.size();i;i--)
	{
		Objs.push_back(hilfsLinien[i-1].Compile());
		cout << "Hilfslinie " << i << " Länge " << hilfsLinien[i-1].Len() << endl;
	}
	TrObjs.push_back(grenze->Compile());
}

SGLVektor SGLPointCloud::getCenter()
{
	return cloud->getCenter();
}

SGLStrecke SGLPointCloud::divide(SGLVektor *Liste,unsigned int cnt)
{
	if(cnt<=1)return SGLStrecke(SGLVektor(HUGE_VAL,HUGE_VAL),SGLVektor(HUGE_VAL,HUGE_VAL));
	else if(cnt==2)
	{
		SGLStrecke ret(Liste[0],Liste[1]);
		ret.punkt1->SetColor(0,255,0);
		ret.punkt2->SetColor(0,255,0);
		hilfsLinien.push_back(ret);
		return ret;
	}

	unsigned int ListLen1,ListLen2;
	SGLVektor *tVekt=subSGLPointCloud::splitList(Liste,cnt,ListLen1,ListLen2);

	if(cnt<=4)hilfsLinien.clear();

	double median=tVekt->SGLV_Z;
	SGLStrecke seg1=divide(Liste,ListLen1);
	SGLStrecke seg2=divide(tVekt,ListLen2);
	SGLStrecke seg=seg1.Len()<seg2.Len() ? seg1:seg2;
	double D=seg.Len();

	if(cnt<=4)
	{
		grenze->MoveTo(0,0,median);
		marker->setDepth(D);
		marker->MoveTo(0,0,median-D/2);
	}

	SGLVektor *S_ = new SGLVektor[cnt];
	unsigned int cnt_=0;

	for(int i=0;i<cnt;i++)
	{
		double myD=Liste[i].SGLV_Z - median;
		if(myD < D)
		{
			S_[cnt_++]=Liste[i];
			if(cnt<=4)
			{
				SGLStrecke proj(Liste[i],SGLVektor(Liste[i].SGLV_X,Liste[i].SGLV_Y,median));
				proj.punkt1->SetColor(255,0,0);
				hilfsLinien.push_back(proj);
			}
		}
	}

	return SGLStrecke(SGLVektor(HUGE_VAL,HUGE_VAL,HUGE_VAL),SGLVektor(HUGE_VAL,HUGE_VAL,HUGE_VAL));
}
