
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
    void getExtrema(SGLVektor *&maxX, SGLVektor *&minX,SGLVektor *&maxY,SGLVektor *&minY, SGLVektor *&maxZ, SGLVektor *&minZ);
    void getTetraeder(SGLVektor *&eins, SGLVektor *&zwei,SGLVektor *&drei,SGLVektor *&vier);
};

class QHDreiEck : public SGLDreiEck
{
public:
	QHDreiEck(SGLVektor *eins,SGLVektor *zwei,SGLVektor *drei);
	~QHDreiEck();
	QHDreiEck *Nachbar[3];
	unsigned int hasKante(SGLVektor *eins,SGLVektor *zwei);
	bool addNachbar(QHDreiEck *Nachbar);
	bool delNachbar(QHDreiEck *Nachbar);
};

class SGLPointCloud : public SGLMetaObj
{
public:
	SGLPointCloud(int PktCnt=128,GLdouble breite=1,GLdouble hoehe=1,GLdouble tiefe=1,GLdouble PosX=0,GLdouble PosY=0,GLdouble PosZ=0);
	~SGLPointCloud();

	subSGLPointCloud *cloud;
	void compileSubObjects();
	SGLVektor getCenter();
	void init();
	int grow(list<QHDreiEck>::iterator seite);
	static void on_mouse(SGLObjBase *target,SDL_Event event);
	list<QHDreiEck> seiten;
};
