#ifndef SGLREKGEB_H
#define SGLREKGEB_H

#include "../primitives/sglvieleck.h"
#include <vector>

/**Ein Fraktales Gebirge aus Dreiecken
  *@author Enrico Reimer
  */
class SGLRekGeb : public SGLFlObj
{
public:
	SGLRekGeb(int Ecken=6,SGLMaterial *Material=0);
	~SGLRekGeb();
public: // Public attributes
	int Mode;
	SGLVektor *BodenVekt;
	bool Export(char* file);
	double Height;
  	void generate();
  	void generate(FILE* ExpFile);
//  	vector<SGLDreiEck*>	BasisEck;

	void generateRek(SGLVektor a,SGLVektor b,SGLVektor c,double High,int Rek,FILE* ExpFile=NULL);
	int ecken,RekDepth;
	unsigned int SurfCnt;
	double size;

GLfloat ctlpoints[4][4][3];

void init_surface(void);

private:
	void generateDreieck(SGLVektor a,SGLVektor b,SGLVektor c,FILE* ExpFile);
	void generateCone(SGLVektor m,SGLVektor a,SGLVektor b,SGLVektor c,FILE* ExpFile);
	void writeDreieckToFile(SGLVektor a,SGLVektor b,SGLVektor c,FILE *ExpFile);

    public: SGLVektor getCenter();
};

#endif
