/***************************************************************************
                          sglvektor.cpp  -  description
                             -------------------
    begin                : Sun Sep 23 2001
    copyright            : (C) 2001 by Enrico Reimer
    email                : enni_@T-Online.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include	"sglvektor.h"
#include	"sglmisc.h"
#include	"util/sgltextur.h"
#include	"util/sglmaterial.h"
#include	<boost/numeric/ublas/matrix.hpp>
#include	<boost/numeric/ublas/io.hpp>
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else 
	#include <GL/glu.h>
#endif

/**
 * Konstruktor für einzelne Koordinaten.
 * @param X X-Position des Vektors
 * @param Y Y-Position des Vektors
 * @param Z Z-Position des Vektors
 */
SGLVektor::SGLVektor(GLdouble X,GLdouble Y,GLdouble Z):dvector(3)
{
	SGLV_X= X;
	SGLV_Y= Y;
	SGLV_Z= Z;
	SGLV_R=SGLV_G=SGLV_B=-1;
}

/**
 * Konstruktor für Koordinaten aus Array.
 * @param Array[] dreistelliges Array mit den X-, Y- und Z- Koordinaten.
 * @return 
 */
SGLVektor::SGLVektor(GLdouble Array[3]):dvector(3)
{
	SGLV_X= Array[0];
	SGLV_Y= Array[1];
	SGLV_Z= Array[2];
	SGLV_R=SGLV_G=SGLV_B=-1;
}

SGLVektor::SGLVektor(GLfloat Array[3]):dvector(3)
{
	SGLV_X= Array[0];
	SGLV_Y= Array[1];
	SGLV_Z= Array[2];
	SGLV_R=SGLV_G=SGLV_B=-1;
}

/**
 * Kopierkonstrukor zur Erzeugung aus EVektor\<GLdouble\> 
 * @param src der EVektor, der kopiert werden soll.
 */
SGLVektor::SGLVektor(const dvector &src):dvector(src)
{
	assert(src.size()==3);
	SGLV_R=SGLV_G=SGLV_B=-1;
}

/**
 * Kopierkonstrukor zur Erzeugung aus SGLVektor 
 * @param src der SGLVektor, der kopiert werden soll.
 */
SGLVektor::SGLVektor(const SGLVektor &src):dvector(src)
{
	SGLV_R=src.SGLV_R;
	SGLV_G=src.SGLV_G;
	SGLV_B=src.SGLV_B;
}

dvector SGLVektor::cross_prod(const vector< GLdouble >& b)
{
	dvector ret(3);
	const dvector &a=*this;
	ret[0]=a[1]*b[2]-a[2]*b[1];
	ret[1]=a[2]*b[0]-a[0]*b[2];
	ret[2]=a[0]*b[1]-a[1]*b[0];
	return ret;
}

//@todo erst checken, dann dokumentieren
SGLVektor SGLVektor::Rotate(GLdouble Yrot, GLdouble Xrot, GLdouble Zrot)
{
	return SGLVektor(	COS(Yrot)*SGLV_X+COS(Zrot+90)*SGLV_Y+COS(Yrot+90)*SGLV_Z,
						COS(Zrot+90)*SGLV_X+COS(Zrot)*SGLV_Y+COS(Xrot+90)*SGLV_Z,
						COS(90-Yrot)*SGLV_X+COS(90-Xrot)*SGLV_Y+COS(Xrot)*SGLV_Z
					);
}

/**
 * Rotiert den Vektor um die angegebe Achse.
 * Dabei wird nicht der eigentliche Vektor, sondern eine Kopie rotiert, die zurückgegeben wird.
 * @param RotVekt der Richtungsvektor der Rotationsachse
 * @param Amount der Winkel, um den rotiert werden soll (in Grad)
 * @return der rotierte Vektor
 */
SGLVektor SGLVektor::Rotate(dvector RotVekt, GLdouble Amount)
{
	RotVekt/=boost::numeric::ublas::norm_2(RotVekt);

	dmatrix RotMat(3,3);
	GLdouble x=RotVekt.SGLV_X,y=RotVekt.SGLV_Y,z=RotVekt.SGLV_Z;
	GLdouble c=COS(Amount),s=SIN(Amount);

	RotMat(0,0)=x*x*(1-c)+c;
	RotMat(0,1)=x*y*(1-c)-z*s;
	RotMat(0,2)=x*z*(1-c)+y*s;

	RotMat(1,0)=y*x*(1-c)+z*s;
	RotMat(1,1)=y*y*(1-c)+c;
	RotMat(1,2)=y*z*(1-c)-x*s;

	RotMat(2,0)=z*x*(1-c)-y*s;
	RotMat(2,1)=z*y*(1-c)+x*s;
	RotMat(2,2)=z*z*(1-c)+c;

	return dvector(boost::numeric::ublas::prod(RotMat, *this));
}

/**
 * Der Winkel zur Z-Achse bei Rotation um Y-Achse
 * @return der Winkel, um den der Vektor um die Y-Achse rotiert werden müsste, um zur Z-Achse koliniar zu sein.
 */
GLdouble SGLVektor::Ywink()
{
	GLdouble teiler=sqrt(SGLV_X*SGLV_X+SGLV_Z*SGLV_Z);
	if(SGLV_X>=0)
	{
		if(SGLV_Z>=0)return ACOS(SGLV_X/teiler);
		else return 360-ACOS(SGLV_X/teiler);
	}
	else
	{
		if(SGLV_Z>=0)return 180-ACOS(SGLV_X/teiler);
		else return 180+ACOS(SGLV_X/teiler);
	}
}

/**
 * Der Winkel zu Z-Achse bei Rotation um X-Achse
 * @return der Winkel, um den der Vektor um die X-Achse rotiert werden müsste, um zur Z-Achse koliniar zu sein.
 */
GLdouble SGLVektor::Xwink()
{
	GLdouble teiler=sqrt(SGLV_Z*SGLV_Z+SGLV_Y*SGLV_Y);
	if(SGLV_Z>=0)
	{
		if(SGLV_Y>=0)return ACOS(SGLV_Z/teiler);
		else return 360-ACOS(SGLV_Z/teiler);
	}
	else
	{
		if(SGLV_Y>=0)return 180-ACOS(SGLV_Z/teiler);
		else return 180+ACOS(SGLV_Z/teiler);
	}
	return 0;
}

SGLVektor SGLVektor::operator =(const dvector &VektPtr)
{
	*this=SGLVektor(VektPtr);
	return *this;
}

/**
 * Ruft glVertex mit den Eigenschaften dieses Vektors auf.
 * Wenn Normale definiert ist, wird sie gesetzt.
 * Danach DrawVertex() aufgerufen.
 * @param Normale die Normale die für diesen Vektor angenommen werden soll.
 */
void SGLVektor::DrawVertex(SGLVektor* Normale)
{
	if(Normale)Normale->SetNormale();
	else{SGLprintWarning("Zeichne Vertex ohne Normale");}
	DrawVertex();
}

/**
 * Zeichnet einen Vertex (glVertex) mit Texturkoordinaten und gegebenfalls Farbinformationen aber ohne Normale.
 */
void SGLVektor::DrawVertex()
{
/*
	Setzt wenn gegeben Texturkoordinaten für alle Texturrenderer vom der aktiven bis zu  GL_TEXTURE0_ARB herunter.
	(Aber immer nur die Selben)
 */
	std::stringstream buff;
	bool texOK=false;
#ifndef WIN32
	if(SGLTextur::TexLoaded)
	{
		short coord=texKoord.size();
		if(coord<SGLTextur::TexLoaded)
		{
			buff << *this;
			SGLprintWarning("Die geladene Textur hat %d Dimensionen, die Texturkoordinaten des Vertex \"%s\" sind aber nur %d-Dimensional",SGLTextur::TexLoaded,buff.str().c_str(),coord);
		}
		int i=GL_TEXTURE0_ARB+SGLTextur::multitex_layer ;//@todo dirty Hack
		switch(SGLTextur::TexLoaded > coord ? coord:SGLTextur::TexLoaded )
		{
		case 1:
			for(;i>=GL_TEXTURE0_ARB;i--)
				glMultiTexCoord1f(i,texKoord[0]);
			break;
		case 2:
			for(;i>=GL_TEXTURE0_ARB;i--)
				glMultiTexCoord2f(i,texKoord[0], texKoord[1]);
			break;
		case 3:
			for(;i>=GL_TEXTURE0_ARB;i--)
				glMultiTexCoord3f(i,texKoord[0], texKoord[1],texKoord[2]);
			break;
		default:{
			buff << *this;
			SGLprintError("Texturtyp (%d) passt nicht zu den verfügbaren Texturkoordinaten beim Zeichnen des Vertex \"%s\"",SGLTextur::TexLoaded, coord,buff.str().c_str());}break;
		}
		texOK=true;//@todo naja nich immer
	}
#else
// "Texture loading is currently not supportet for Windows"
#endif
	if(!SGLMaterial::MatLoaded && !texOK)
	{
		if(SGLV_R>=0 || SGLV_G>=0 || SGLV_B>=0)glColor3dv(Color);
		else{
			buff << *this;
			SGLprintWarning("Keine Farbinformationen verfgbar beim Zeichnen des Vertex \"%s\"",buff.str().c_str());
		}
	}
	DrawPureVertex();
}

/**
 * Setzt die Zeichenfarbe des Vektors in Fließkommaformat.
 * Werte zwischen 0 und 1 sind zulässig.
 */
void SGLVektor::SetColor(GLdouble R, GLdouble G, GLdouble B)
{
	SGLV_R=R;
	SGLV_G=G;
	SGLV_B=B;
}

/**
 * Setzt die Zeichenfarbe des Vektors in Ganzzahligen Werten.
 * Negative Werte werden invertiert.
 * Werte über 255 werden auf 255 gekürzt.
 */
void SGLVektor::SetColor(int R, int G, int B)
{
	R=abs(R);
	G=abs(G);
	B=abs(B);
	R= R > 255 ? 255:R;
	G= G > 255 ? 255:G;
	B= B > 255 ? 255:B;
	SetColor((unsigned char)R,(unsigned char)G,(unsigned char)B);
}

/**
 * Setzt die Zeichenfarbe des Vektors in Ganzzahligen Werten.
 * Die Parameter werden nicht geprüft oder angepasst bevor sie gesetzt werden.
 */
void SGLVektor::SetColor(unsigned char R,unsigned char G,unsigned char B)
{SetColor(GLdouble(R)/GLdouble(255),GLdouble(G)/GLdouble(255),GLdouble(B)/GLdouble(255));}

/**
 * Bestimmt Rotationswinkel und Anstieg des Vektors im Verhältniss zu dem Koordinatenebenen.
 * @param ebene_wink Puffer für den Roatationswinkel der Projektion des Vektors auf die X-Y-Ebene zur X-Achse
 * @param steigung_wink Puffer für den Anstiegswinkel des Vektos
 * @return 
 */
GLdouble SGLVektor::toWink(GLdouble &ebene_wink,GLdouble &steigung_wink)
{
	assert(size()==3);
	struct BasisWink{
		static GLdouble recurse(const dvector &vec,GLdouble buff[]){
			GLdouble a,b,c;
			c=boost::numeric::ublas::norm_2(vec); //Länge des Vektors
			
			if(vec.size()>2)
			{
				dvector vec2(2);
				vec2[0]=vec[0];vec2[1]=vec[2];
				a=recurse(vec2,buff+1); //Länge des Vektors auf die size()-1-Ebene projeziert (und sein Winkel auf dieser Ebene)
			}
			else a=vec[0];

			b=vec[1];//Höhe
			*buff= a ? ATAN(b/a):90;
			if(a<0)*buff+=180;//2. & 3. Quadranten gibts nur im 2dim Raum, bei höherdim. Räumen wird das auf eine 180°-Rotation auf der darunter-liegenden "Ebene" zurï¿½ckgefï¿½hrt
			else if(b<0)*buff+=360;
			return c;
		}
	};

	
	GLdouble tWink[2];	
	GLdouble ret=BasisWink::recurse(*this,tWink);
	ebene_wink=tWink[1];
	steigung_wink=tWink[0];
	return ret;
}

/**
 * Erzeugt im Renderer eine Transformation an die Position des Vektors
 * Das folgende Beispiel zeichnet einen Punkt an der Position \<5,5,5\>
 * \code 
 * SGLVektor(5,5,5).translateTo();
 * SGLVektor(0,0,0).DrawPkt(1);
 * \endcode
 */
void SGLVektor::translateTo()
{
	glTranslated(SGLV_X,SGLV_Y,SGLV_Z);
}

/**
 * Zeichnet den Vektor.
 * Zeichnet einen Strahl von den angegeben Koordinaten zum Vektor.
 * Dabei geht die Farbe des Strahls von weiß in die des Vektors über.
 * @param from der Ursprung des Strahls
 */
void SGLVektor::DrawVektor(SGLVektor from)
{
  //	bool Texture2D=glIsEnabled(GL_TEXTURE_2D);
  //	bool Light=glIsEnabled(GL_LIGHTING);
/*	Bringt nur was, wenn diese Flags IN der GL-Maschine gesetzt sind
	ein glEnable weiter ober in dieser oder einer anderen Liste steht zwar dort,
	aber es kann nicht garantiert werden, daï¿½es schon "wirkt".
	Man sollte deshalb in der aufrufenden Funktion selbst, mit unbedingten glEnable/glDisable vorbeugen.
	Und denkt daran, nvidia optimiert "obselote" Flag-Setzungen weg => Reihenfolge beibehalten
	if(Texture2D)glDisable(GL_TEXTURE_2D);
	if(Light)glDisable(GL_LIGHTING);
	*/

	glBegin(GL_LINES);
		DrawVertex();
		glColor3f(1,1,1);
		from.DrawVertex();
	glEnd();
/*	if(Texture2D)glEnable(GL_TEXTURE_2D);
	if(Light)glEnable(GL_LIGHTING);
	s.O. richtet mehr schaden an, als es bringt ("reaktiviert" xyz obwohl xyz gar nicht an war)
*/
}

/**
 * Zechnet den Vektor mit At als Stützvektor.
 * Es wird eine Strecke von At nach this+At gezeichnet.
 * @param At der Stützvektor des zu zeichnenden Vektors.
 */
void SGLVektor::DrawVektorAt(SGLVektor At)
{
	SGLVektor DrawV=*this;//Um alle Attribute (z.B. Farbe in DrawV zu übertr.)
	DrawV +=(At);
	DrawV.DrawVektor(At);
}

/**
 * Zeichnet ein dreidimensionales Kreuz an den Positionskoordinaten des Vektors.
 * @param size die Große des Kreuzes.
 */
void SGLVektor::DrawPkt(double size)
{
	glBegin(GL_LINES);
		if(SGLV_R>=0 || SGLV_G>=0 || SGLV_B>=0)
			glColor3dv(Color);
		glVertex3d(SGLV_X-size/2,SGLV_Y,SGLV_Z);
		glVertex3d(SGLV_X+size/2,SGLV_Y,SGLV_Z);

		glVertex3d(SGLV_X,SGLV_Y-size/2,SGLV_Z);
		glVertex3d(SGLV_X,SGLV_Y+size/2,SGLV_Z);

		glVertex3d(SGLV_X,SGLV_Y,SGLV_Z-size/2);
		glVertex3d(SGLV_X,SGLV_Y,SGLV_Z+size/2);
	glEnd();
}

GLdouble SGLVektor::spatprod(const vector< GLdouble >& b, const vector< GLdouble >& c) const
{
	assert(this->size()==3);
	//Sarrus
	return	 (*this)[0]*b[1]*c[2] - (*this)[0]*b[2]*c[1]
		+(*this)[1]*b[2]*c[0] - (*this)[1]*b[0]*c[2]
		+(*this)[2]*b[0]*c[1] - (*this)[2]*b[1]*c[0];
}

