/***************************************************************************
                          sgllight.cpp  -  description
                             -------------------
    begin                : Fri Oct 5 2001
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

#include "sgllight.h"

SGLLight::SGLLight(int LID,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):SGLObj(PosX,PosY,PosZ,SizeFact)
{
	IsSpot=false;
	if(LID>=0)LightID=GLenum(LID);
	else
	{
		GLint MaxLights;
		glGetIntegerv(GL_MAX_LIGHTS,&MaxLights);
		for(GLint i=0;i<MaxLights;i++)
			if(!glIsEnabled(GLenum(GL_LIGHT0+i)))
			{
				LightID=GLenum(GL_LIGHT0+i);
				On();
				break;
			}
	}
	bool old_free=is_free;
	is_free=true;
	SetColor(1,1,1);
	is_free=old_free;
	Farbe.Umgebung[3]=Farbe.Difus[3]=Farbe.Glanz[3]=1;
	Spot.Richtung=SGLVektor(0,0,-1);
	Spot.Ausleuchtung=1;
	Spot.Winkel=30;
	Abnahme.Linear=0;
	Abnahme.Quadratisch=.05;
}

SGLLight::~SGLLight()
{
}

bool SGLLight::On(bool on)
{
	bool ret=IsOn();
	if(!ret && on)glEnable(LightID);
	else if(ret && !on)glDisable(LightID);
	return ret;
}
bool SGLLight::IsOn(){return glIsEnabled(LightID);}

void SGLLight::generate()
{
	const GLint Pos[4]={0,0,0,1};
	glLightiv(LightID,GL_POSITION,Pos);
	if(isVisible())
	{
		 // @todo implement me
	}
}

void SGLLight::CamLight()
{
	beginList(false);
	endList();//Die Liste leeren (Cameralichter brauchen nicht jedesmal neugezeichnet werden)
	is_free=true;
	const GLint Pos[4]={0,0,1,1};
	SetUp();
	SetParam(GL_SPOT_CUTOFF,30);
	glPushMatrix();
	glLoadIdentity();
	glLightiv(LightID,GL_POSITION,Pos);
	glPopMatrix();
}

GLuint SGLLight::Compile(bool draw,bool free)
{
	SetUp();
	return SGLObj::Compile();
}

void SGLLight::SetParam(GLenum pname,GLfloat param)
{glLightf(LightID,pname,param);}

void SGLLight::SetParam(GLenum pname,const GLfloat *params)
{glLightfv(LightID,pname,params);}

void SGLLight::SetUp()
{
	const GLfloat Richt[3]={Spot.Richtung.SGLV_X,Spot.Richtung.SGLV_Y,Spot.Richtung.SGLV_Z};

	SetParam(GL_AMBIENT,Farbe.Umgebung);
	SetParam(GL_DIFFUSE,Farbe.Difus);
	SetParam(GL_SPECULAR,Farbe.Glanz);
	SetParam(GL_LINEAR_ATTENUATION,Abnahme.Linear);
	SetParam(GL_QUADRATIC_ATTENUATION,Abnahme.Quadratisch);

	if(IsSpot)
	{
		SetParam(GL_SPOT_DIRECTION,Richt);
		SetParam(GL_SPOT_EXPONENT,1.0/Spot.Ausleuchtung);
		SetParam(GL_SPOT_CUTOFF,Spot.Winkel);
	}
	else SetParam(GL_SPOT_CUTOFF,180);
}

/*!
    \fn SGLLight::getCenter()
 */
SGLVektor SGLLight::getCenter()const
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}



/*!
    \fn SGLLight::isVisible()
 */
bool SGLLight::isVisible()
{
    return IsOn() && visible;
}


/*!
    \fn SGLLight::setVisible(bool visible=true)
 */
void SGLLight::setVisible(bool visible)
{
	if(visible)
	{
		this->visible=true;
		On();
	}
	else this->visible=false;
}


/*!
    \fn SGLLight::getFarbeAt(SGLVektor vekt)
 */
LichtFarbe SGLLight::getFarbeAt(SGLVektor vekt)
{
    LichtFarbe ret=Farbe;
	double len=fabs((this->getMyPos()-vekt).Len());
	double attenuation=1/(1+len*Abnahme.Linear+pow(len,2)*Abnahme.Quadratisch);
	ret.Difus[0]*=attenuation;
	ret.Difus[1]*=attenuation;
	ret.Difus[2]*=attenuation;

	ret.Glanz[0]*=attenuation;
	ret.Glanz[1]*=attenuation;
	ret.Glanz[2]*=attenuation;

	ret.Umgebung[0]*=attenuation;
	ret.Umgebung[1]*=attenuation;
	ret.Umgebung[2]*=attenuation;
	return ret;
}


/*!
    \fn SGLLight::SetColor(GLfloat r,GLfloat g,GLfloat b)
 */
void SGLLight::SetColor(GLfloat r,GLfloat g,GLfloat b)
{
	Farbe.Umgebung[0]=.0001*r;
	Farbe.Umgebung[1]=.0001*g;
	Farbe.Umgebung[2]=.0001*b;

	Farbe.Difus[0]=.8*r;
	Farbe.Difus[1]=.8*g;
	Farbe.Difus[2]=.8*b;

	Farbe.Glanz[0]=r;
	Farbe.Glanz[1]=g;
	Farbe.Glanz[2]=b;

	compileNextTime();
}
