//
// C++ Interface: sglpointcloud
//
// Description:
//
//
// Author:  <enni@Willow.Reimer>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "../primitives/sglflobj.h"
#include "../sglmetaobj.h"
#include "../sglvektor.h"
#include "../primitives/sglquader.h"
#include "../primitives/sglvieleck.h"

class subSGLPointCloud : public SGLFlObj
{
	int _old_rnd;
	int make_rand(int seed);
public:
	SGLVektor *punkte;
	int pktCnt;
	GLdouble breite,hoehe,tiefe;

	subSGLPointCloud(int PktCnt=0,GLdouble breite=1,GLdouble hoehe=1,GLdouble tiefe=1,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0);
	~subSGLPointCloud();

	void make_rand_pts(int cnt);
	void generate();
	SGLVektor getCenter();
	static int sortX(const void * P1,const void *P2);
	static int sortY(const void * P1,const void *P2);
	static int sortZ(const void * P1,const void *P2);
	static SGLVektor *splitList(SGLVektor List[],unsigned int cnt,unsigned int &cnt1,unsigned int &cnt2);
};

class SGLPointCloud : public SGLMetaObj
{
public:
	SGLPointCloud(int PktCnt=128,GLdouble breite=1,GLdouble hoehe=1,GLdouble tiefe=1,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0);
	~SGLPointCloud();

	subSGLPointCloud *cloud;
	SGLQuader *quader;
	SGLQuader *marker;
	SGLRechtEck *grenze;
	void compileSubObjects();
	SGLVektor getCenter();
	vector<SGLStrecke> hilfsLinien;
	SGLStrecke divide(SGLVektor *Liste,unsigned int cnt);
};
