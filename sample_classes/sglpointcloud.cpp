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
	for(int i=hilfsLinien.size();i;i--)
		Objs.push_back(hilfsLinien[i-1].Compile());
	for(int i=seiten.size();i;i--)
		Objs.push_back(seiten[i-1].Compile());
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
	cloud->getTetraeder(pt1,pt2,pt3,pt4);
	
	seiten.push_back(SGLDreiEck(pt1,pt3,pt2));
	seiten.push_back(SGLDreiEck(pt1,pt2,pt4));
	seiten.push_back(SGLDreiEck(pt1,pt4,pt3));
	seiten.push_back(SGLDreiEck(pt2,pt3,pt4));
	compileNextTime();
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
