/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sglplanet.h"

/*!
    \fn SGLPlanet::SGLPlanet(GLdouble radius,GLdouble efakt)
 */
SGLPlanet::SGLPlanet(GLdouble radius,GLdouble efakt)
{
	radius_b=radius;
	e=efakt*radius;
	radius_a= e ? sqrt(e*e+radius_b*radius_b):radius;
	this->e=e;
	neigung=10;

	upVekt=SGLVektor(1,10);
	Planet.size=.02;
	if(Planet.Mat.use_count()<2)//@todo geht das auch klar ??
	{
		Planet.Mat->Aussen.GlanzFaktor=0;
		Planet.Mat->Aussen.Farbe.Glanz[0]=0;
		Planet.Mat->Aussen.Farbe.Glanz[1]=0;
		Planet.Mat->Aussen.Farbe.Glanz[2]=0;
	}
	gradPos=int(double(rand())/RAND_MAX*360);
}


/*!
    \fn SGLPlanet::~SGLPlanet()
 */
SGLPlanet::~SGLPlanet()
{
}


/*!
    \fn SGLPlanet::getCenter()
 */
SGLVektor SGLPlanet::getCenter()
{
    return SGLVektor(0,0,0); // @todo not implemented yet
}

/*!
    \fn SGLPlanet::generate()
 */
void SGLPlanet::generate()
{

	glRotated(neigung,0,0,1);
	glTranslated(-e,0,0);

//	return SGLVektor(radius).Rotate(SGLVektor(0,1),grad);

	glBegin(GL_LINE_LOOP);
		for(int i=0;i<360;i++)
		{
			grad2Pos(i).DrawVertex();
		}
	glEnd();

	grad2Pos(gradPos++).translateTo();
	Planet.generate();
}

/*!
    \fn SGLPlanet::grad2Pos(GLdouble grad)
 */
SGLVektor SGLPlanet::grad2Pos(GLdouble grad)
{
	return SGLVektor(radius_a*COS(grad),0,radius_b*SIN(grad));
}
