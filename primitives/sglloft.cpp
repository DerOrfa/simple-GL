// SGLLoft.cpp: Implementierung der Klasse SGLLoft.
//
//////////////////////////////////////////////////////////////////////

#include "sglloft.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

SGLLoft::SGLLoft(SGLPolygon &BasisFl,SGLVektor loft,SGLMaterial *Material,unsigned short mode):SGLPolygonObj(Material)
{
	int ecken=BasisFl.EckVektoren.Cnt;

	SGLVektor	SeitenVekt[4],*BodenVekt= new SGLVektor[ecken],*DeckVekt= new SGLVektor[ecken];
	SGLPolygon	*Flaechen= new SGLPolygon[ecken+1];

	SGLVektor oldOben,oldUnten,b=*BasisFl.EckVektoren.Vekt[0];

	DeckVekt[0]=b+(loft*.5);
	BodenVekt[ecken-1]=b-(loft*.5);

	for(int i=1;i<ecken;i++)
	{
		oldOben=b+(loft*.5);
		oldUnten=b-(loft*.5);

		b=*BasisFl.EckVektoren.Vekt[i];

		DeckVekt[i]=b+(loft*.5);
		BodenVekt[ecken-i-1]=b-(loft*.5);

		SGLVektor SeitenVekt[]={oldUnten,BodenVekt[ecken-i-1],DeckVekt[i],oldOben};
		Flaechen[i]= SGLPolygon(SeitenVekt,4);
	}

	oldOben=b+(loft*.5);
	oldUnten=b-(loft*.5);
	b=*BasisFl.EckVektoren.Vekt[0];

	SGLVektor nSeitenVekt[]={oldUnten,BodenVekt[ecken-1],DeckVekt[0],oldOben};//M$ ist soooo scheisse
	Flaechen[0]= SGLPolygon(nSeitenVekt,4);

	int flCnt=ecken;
	SGLPolygon Decke(DeckVekt,ecken);
	SGLPolygon Boden(BodenVekt,ecken);

	if(mode&1)Flaechen[flCnt++]=Boden;
	if(mode&2)Flaechen[flCnt++]=Decke;

	SetFl(Flaechen,flCnt);
}

SGLLoft::~SGLLoft()
{

}
