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
#include	<GL/glu.h>
#include	"util/sgltextur.h"
#include	"util/sglmaterial.h"

SGLVektor::SGLVektor(GLdouble X,GLdouble Y,GLdouble Z):EVektor<GLdouble>(3)
{
	SGLV_X= X;
	SGLV_Y= Y;
	SGLV_Z= Z;
	SGLV_R=SGLV_G=SGLV_B=-1;
}
SGLVektor::SGLVektor(GLdouble Array[3]):EVektor<GLdouble>(3)
{
	SGLV_X= Array[0];
	SGLV_Y= Array[1];
	SGLV_Z= Array[2];
	SGLV_R=SGLV_G=SGLV_B=-1;
}

SGLVektor::SGLVektor(GLfloat Array[3]):EVektor<GLdouble>(3)
{
	SGLV_X= Array[0];
	SGLV_Y= Array[1];
	SGLV_Z= Array[2];
	SGLV_R=SGLV_G=SGLV_B=-1;
}

SGLVektor::SGLVektor(const EVektor<GLdouble> &src):EVektor<GLdouble>(src)
{
	SGLV_R=SGLV_G=SGLV_B=-1;
}

SGLVektor::SGLVektor(const SGLVektor &src):EVektor<GLdouble>(src)
{
	SGLV_R=src.SGLV_R;
	SGLV_G=src.SGLV_G;
	SGLV_B=src.SGLV_B;
}


SGLVektor::~SGLVektor(){}

SGLVektor SGLVektor::Rotate(GLdouble Yrot, GLdouble Xrot, GLdouble Zrot)
{
	return SGLVektor(	COS(Yrot)*SGLV_X+COS(Zrot+90)*SGLV_Y+COS(Yrot+90)*SGLV_Z,
						COS(Zrot+90)*SGLV_X+COS(Zrot)*SGLV_Y+COS(Xrot+90)*SGLV_Z,
						COS(90-Yrot)*SGLV_X+COS(90-Xrot)*SGLV_Y+COS(Xrot)*SGLV_Z
					);
}

SGLVektor SGLVektor::Rotate(SGLVektor RotVekt, GLdouble Amount)
{
	RotVekt.Normalize();

	EMatrix<GLdouble> RotMat(3,3);
	GLdouble x=RotVekt.SGLV_X,y=RotVekt.SGLV_Y,z=RotVekt.SGLV_Z;
	GLdouble c=COS(Amount),s=SIN(Amount);

	RotMat[0][0]=x*x*(1-c)+c;
	RotMat[0][1]=x*y*(1-c)-z*s;
	RotMat[0][2]=x*z*(1-c)+y*s;

	RotMat[1][0]=y*x*(1-c)+z*s;
	RotMat[1][1]=y*y*(1-c)+c;
	RotMat[1][2]=y*z*(1-c)-x*s;

	RotMat[2][0]=z*x*(1-c)-y*s;
	RotMat[2][1]=z*y*(1-c)+x*s;
	RotMat[2][2]=z*z*(1-c)+c;

	return SGLVektor(RotMat * (*this));
}

//Der Winkel zu Z-Achse bei Rotation um Y-achse
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

//Der Winkel zu Z-Achse bei Rotation um X-achse
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

SGLVektor SGLVektor::operator =(EVektor<GLdouble> &VektPtr)
{
	*this=SGLVektor(VektPtr);
	return *this;
}

void SGLVektor::DrawVertex(SGLVektor* Normale)
{
	if(Normale)Normale->SetNormale();
	else{SGLprintWarning("Zeichne Vertex ohne Normale");}
	DrawVertex();
}

/**
Setzt wenn gegeben Texturkoordinaten für alle Texturrenderer vom der aktiven bis zu  GL_TEXTURE0_ARB herunter.
(Aber immer nur die Selben)
*/
void SGLVektor::DrawVertex()
{
	char buff[50];sprint(buff);//@todo Ressourcenverschwendung wenn kein Fehler auftritt, brauch ich auch keinen String
	bool texOK=false;
	if(SGLTextur::TexLoaded)
	{
		short coord=texKoord.size();
		if(coord<SGLTextur::TexLoaded)
		{
			SGLprintWarning("Die geladene Textur hat %d Dimensionen, die Texturkoordinaten des Vertex \"%s\" sind aber nur %d-Dimensional",SGLTextur::TexLoaded,buff,coord);
		}
		switch(SGLTextur::TexLoaded > coord ? coord:SGLTextur::TexLoaded )
		{
		case 1:
			for(unsigned int i=sglGeti(GL_ACTIVE_TEXTURE_ARB);i>=GL_TEXTURE0_ARB;i--)
				glMultiTexCoord1f(i,texKoord[0]);
			break;
		case 2:
			for(unsigned int i=sglGeti(GL_ACTIVE_TEXTURE_ARB);i>=GL_TEXTURE0_ARB;i--)
				glMultiTexCoord2f(i,texKoord[0], texKoord[1]);
			break;
		case 3:
			for(unsigned int i=sglGeti(GL_ACTIVE_TEXTURE_ARB);i>=GL_TEXTURE0_ARB;i--)
				glMultiTexCoord3f(i,texKoord[0], texKoord[1],texKoord[2]);
			break;
		default:{
		SGLprintError("Texturtyp (%d) passt nicht zu den verfügbaren Texturkoordinaten beim Zeichnen des Vertex \"%s\"",SGLTextur::TexLoaded, coord,buff);}break;
		}
		texOK=true;//@todo naja nich immer
	}
	if(!SGLMaterial::MatLoaded && !texOK)
	{
		if(SGLV_R>=0 || SGLV_G>=0 || SGLV_B>=0)glColor3dv(Color);
		else{SGLprintWarning("Keine Farbinformationen verfgbar beim Zeichnen des Vertex \"%s\"",buff);}
	}
	DrawPureVertex();
}

void SGLVektor::SetColor(GLdouble R, GLdouble G, GLdouble B)
{
	SGLV_R=R;
	SGLV_G=G;
	SGLV_B=B;
}

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

void SGLVektor::SetColor(unsigned char R,unsigned char G,unsigned char B)
{SetColor(GLdouble(R)/GLdouble(255),GLdouble(G)/GLdouble(255),GLdouble(B)/GLdouble(255));}

/*!
    \fn SGLVektor::toWink(GLdouble &ebene__wink,GLdouble &steigung__wink)
 */
GLdouble SGLVektor::toWink(GLdouble &ebene_wink,GLdouble &steigung_wink)
{
	GLdouble tWink[2];
	GLdouble ret=BasisWink(tWink);
	ebene_wink=tWink[1];
	steigung_wink=tWink[0];
	return ret;
}

/*!
    \fn SGLVektor::translateTo()
 */
void SGLVektor::translateTo()
{
	glTranslated(SGLV_X,SGLV_Y,SGLV_Z);
}

/*!
    \fn SGLVektor::DrawVektor(SGLVektor from)
 */
void SGLVektor::DrawVektor(SGLVektor from)
{
	bool Texture2D=glIsEnabled(GL_TEXTURE_2D);
	bool Light=glIsEnabled(GL_LIGHTING);
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


/*!
    \fn SGLVektor::DrawVektorAt(SGLVektor At)
 */
void SGLVektor::DrawVektorAt(SGLVektor At)
{
	SGLVektor DrawV=*this;//Um alle Attribute (z.B. Farbe in DrawV zu bertr.)
	DrawV +=(At);
	DrawV.DrawVektor(At);
}

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
