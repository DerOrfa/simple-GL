/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SGLPLANET_H
#define SGLPLANET_H

#include "../sglobj.h"
#include "../primitives/sglquadricobj.h"

/**
@author Enrico Reimer
*/
class SGLPlanet : public SGLFlObj
{
public:
    SGLPlanet(GLdouble radius,GLdouble efakt=0);
    ~SGLPlanet();

    virtual SGLVektor getCenter();
    virtual void generate();
    SGLVektor grad2Pos(GLdouble grad);
	SGLSphere Planet;
	SGLVektor upVekt;
protected:
    double e;
	double radius_a,radius_b;
	double neigung;
	int gradPos;
};

#endif
