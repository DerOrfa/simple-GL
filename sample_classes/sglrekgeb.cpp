#include "sglrekgeb.h"
// @todo den headernamen in Emath ändern
#include "../sglmisc.h"

#define HIGHFAKTOR	.6

SGLRekGeb::SGLRekGeb(int Ecken,MaterialPtr Material):SGLFlObj(Material)
{
	ecken=Ecken;
	RekDepth=6;
	size=5;
	Height=2;
	BodenVekt =new SGLVektor[ecken+1];
	Mode=2;
	RekDepth=5;

	init_surface();
}
SGLRekGeb::~SGLRekGeb()
{
	delete BodenVekt;
}

void SGLRekGeb::init_surface()
{
    int u, v;
    for (u = 0; u < 4; u++)
	{
        for (v = 0; v < 4; v++)
		{
            ctlpoints[u][v][0] = 2.0*((GLfloat)u - 1.5);
            ctlpoints[u][v][1] = 2.0*((GLfloat)v - 1.5);

        if ( (u == 1 || u == 2) && (v == 1 || v == 2))
            ctlpoints[u][v][2] = 3.0;
        else
            ctlpoints[u][v][2] = -3.0;
        }
    }
}

void SGLRekGeb::generate(){generate(NULL);}
void SGLRekGeb::generate(FILE* ExpFile)
{
	SGLVektor Mitte;
	double unterteilg=360/ecken;
	SGLVektor a,b(size,0);
	SurfCnt=0;

	BodenVekt[ecken-1]=b;
	BodenVekt[ecken-1].SGLV_Z-=Height;

	SGLMaterial Mat(.3,.3,1);


	Mat.Transparenz=.3;

	for(int i=1;i<ecken;i++)
	{
		a=b;
		b=SGLVektor(COS(i*unterteilg)*size,SIN(i*unterteilg)*size);

		generateRek(a,b,SGLVektor(),Height,RekDepth,ExpFile);


		SGLVektor d=b,e=a;e.SGLV_Z-=Height;d.SGLV_Z-=Height;
		BodenVekt[ecken-i-1]=d;
		SGLVektor SeitenVekt[]={e,d,b,a};
		SGLPolygon Seite(SeitenVekt,4);Seite.generate();
	}
	generateRek(b,SGLVektor(size,0),SGLVektor(),Height,RekDepth,ExpFile);

	SGLVektor d(size,0),e=b;e.SGLV_Z-=Height;d.SGLV_Z-=Height;
	SGLVektor SeitenVekt[]={e,d,SGLVektor(size,0),b};

	SGLPolygon Seite(SeitenVekt,4);
	SGLPolygon Boden(BodenVekt,ecken);

	Seite.generate();
	Boden.generate();


#ifndef WIN32
	SGLprintInfo("Gebirge besteht aus %d Einzelflächen",SurfCnt);
#endif
}

void SGLRekGeb::generateCone(SGLVektor m,SGLVektor a,SGLVektor b,SGLVektor c,FILE* ExpFile)
{
	if(ExpFile)
	{
		writeDreieckToFile(m,a,b,ExpFile);
		writeDreieckToFile(m,b,c,ExpFile);
		writeDreieckToFile(m,c,a,ExpFile);
	}
	else
	{
		if(glIsEnabled(GL_LIGHTING))
		{
			SGLVektor SpitzenNormale=Normale(a,b,c);
			SGLVektor NormaleA=Normale(a,b,m);
			SGLVektor NormaleB=Normale(b,c,m);
			SGLVektor NormaleC=Normale(c,a,m);

			glBegin(GL_TRIANGLE_FAN);
				m.DrawVertex(&SpitzenNormale);
				a.DrawVertex(&NormaleA);
				b.DrawVertex(&NormaleB);
				c.DrawVertex(&NormaleC);
				a.DrawVertex(&NormaleA);
			glEnd();
		}
		else
		{
			glBegin(GL_TRIANGLE_FAN);
				m.DrawVertex();
				b.DrawVertex();
				a.DrawVertex();
				c.DrawVertex();
				b.DrawVertex();
			glEnd();
		}
	}
}
void SGLRekGeb::generateDreieck(SGLVektor a,SGLVektor b,SGLVektor c,FILE* ExpFile)
{
#define SET_COLOR(VEKT)	{VEKT.SetColor(.02,.5,.02);}

		if(ExpFile)writeDreieckToFile(a,b,c,ExpFile);
		else
		{
			if(glIsEnabled(GL_LIGHTING))
			{
				SGLVektor N=SGLObj::Normale(a,b,c);
				glBegin(GL_TRIANGLES);
					a.DrawVertex(&N);
					b.DrawVertex(&N);
					c.DrawVertex(&N);
				glEnd();
			}
			else
			{
/*				SET_COLOR(a);
				SET_COLOR(b);
				SET_COLOR(c);
				b.SetColor(0,int(b.SGLV_Z),0);
				c.SetColor(0,int(c.SGLV_Z),0);*/
				glBegin(GL_TRIANGLE_FAN);
					a.DrawVertex();
					b.DrawVertex();
					c.DrawVertex();
				glEnd();
			}
		}
}

void SGLRekGeb::generateRek(SGLVektor a,SGLVektor b,SGLVektor c,double High,int Rek,FILE* ExpFile)
{
	SGLVektor m=EVektor<GLdouble>((a+b+c)*(1./3.));
//	srand(Rek);
	m.SGLV_Z+=(rand()-RAND_MAX/2) /double(RAND_MAX)*High;

	if(Rek>=1)
	{
		generateRek(a,b,m,High*HIGHFAKTOR,Rek-1,ExpFile);
		generateRek(b,c,m,High*HIGHFAKTOR,Rek-1,ExpFile);
		generateRek(c,a,m,High*HIGHFAKTOR,Rek-1,ExpFile);
		generateRek(a,b,m,High*HIGHFAKTOR,Rek-1,ExpFile);
	}
	else
	{
		SurfCnt+=3;
		switch(Mode)
		{
		case 1:
		{
			generateDreieck(m,a,b,ExpFile);
			generateDreieck(m,b,c,ExpFile);
			generateDreieck(m,c,a,ExpFile);
		}break;
		case 2:generateCone(m,a,b,c,ExpFile);break;
		}
	}
	if(Mode==3)
	{

    }
}

bool SGLRekGeb::Export(char *fileName)
{
	FILE *file;
	if(fileName && strlen(fileName))
		if(file=fopen(fileName,"w"))
		{
			generate(file);
			fclose(file);
			return true;
		}
	return false;
}

void SGLRekGeb::writeDreieckToFile(SGLVektor a, SGLVektor b, SGLVektor c, FILE *ExpFile)
{
			fprintf(ExpFile,"3dfläche\n");
			fprintf(ExpFile,"%f,%f,%f ",c.SGLV_X,c.SGLV_Y,c.SGLV_Z);
			fprintf(ExpFile,"%f,%f,%f ",a.SGLV_X,a.SGLV_Y,a.SGLV_Z);
			fprintf(ExpFile,"%f,%f,%f ",b.SGLV_X,b.SGLV_Y,b.SGLV_Z);
			fprintf(ExpFile," \n");
}

/*!
    \fn SGLRekGeb::getCenter()
 */
SGLVektor SGLRekGeb::getCenter()
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}

