/***************************************************************************
                          eglicosahedron.cpp  -  description
                             -------------------
    begin                : Sat Sep 15 2001
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

#include "sglicosahedron.h"

SGLIcosahedron::SGLIcosahedron(unsigned short int SubDivisions,GLdouble PosX,GLdouble PosY,GLdouble PosZ,GLdouble SizeFact):
SGLFlObj(MaterialPtr(),PosX,PosY,PosZ,SizeFact)
{
	X= 0.525731112119133606;
	Z= 0.850650808352039932;
	SubDiv=SubDivisions;

}
SGLIcosahedron::~SGLIcosahedron()
{}

void SGLIcosahedron::generate()
{
	int i;
	static SGLVektor vdata[12]=
	{
		SGLVektor(-X, 0.0, Z), SGLVektor(X, 0.0, Z), SGLVektor(-X, 0.0, -Z), SGLVektor(X, 0.0, -Z),
		SGLVektor(0.0, Z, X), SGLVektor(0.0, Z, -X), SGLVektor(0.0, -Z, X), SGLVektor(0.0, -Z, -X),
		SGLVektor(Z, X, 0.0), SGLVektor(-Z, X, 0.0), SGLVektor(Z, -X, 0.0), SGLVektor(-Z, -X, 0.0)

	};

	for(i=0;i<12;i++)
	{
		vdata[i].SetColor(i*20,i*20,i*20);
	}

	static GLint tindices[20][3]=
	{
		{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
		{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
		{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
		{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
	};

	for(i=0;i<20;i++)
	{
/*		SGLVektor normV=Normale(vdata[tindices[i][0]],vdata[tindices[i][1]],vdata[tindices[i][2]]);
		normV.SetNormale();*/

		PolyCount=subdivide(vdata[tindices[i][0]],
							vdata[tindices[i][1]],
							vdata[tindices[i][2]],
							SubDiv);
	}
}

long int SGLIcosahedron::subdivide(SGLVektor v1,SGLVektor v2, SGLVektor v3,unsigned int depth)
{
	SGLVektor v12,v23,v31;

	if(!depth)
	{
		glBegin(GL_POLYGON);
			v3.DrawVertex();
			v2.DrawVertex();
			v1.DrawVertex();

		glEnd();
		return 1;
	}
	else
	{
		v12= v1+v2;
		v23= v2+v3;
		v31= v3+v1;

		v12.Normalize();v12.SetColor(v1.SGLV_R,v1.SGLV_G,v1.SGLV_B);
		v23.Normalize();v23.SetColor(v2.SGLV_R,v1.SGLV_G,v3.SGLV_B);
		v31.Normalize();v31.SetColor(v3.SGLV_R,v1.SGLV_G,v1.SGLV_B);

		return	subdivide(v1,v12,v31,depth-1)+
				subdivide(v2,v23,v12,depth-1)+
				subdivide(v3,v31,v23,depth-1)+
				subdivide(v12,v23,v31,depth-1);

	}
}

/*!
    \fn SGLIcosahedron::getCenter()
 */
SGLVektor SGLIcosahedron::getCenter()const
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}

