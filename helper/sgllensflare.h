/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


/* Delivered from glflare-sample of Mark J. Kilgard [mjk@nvidia.com]
   His original comment:
   This source code is based on Direct3D-based code written by Stephen
   Coy of Microsoft.  All credit for the original implementation of the
   idea should go to Stephen.  While I referenced Stephen's code during
   the writing of my code, my code was completely written by me from
   scratch.  The algorithms are basically the same to ease critical
   comparison of the OpenGL and Direct3D versions.  */

/* The image files used by this program are partly derived from
   images developed by Microsoft. */


#ifndef SGLLENSFLARE_H
#define SGLLENSFLARE_H

#include "../sglobj.h"

#include "sglcamera.h"
#include "sgllight.h"

typedef struct t_flare {
  int type;             /* flare texture index, 0..5 */
  float scale;
  float loc;            /* postion on axis */
  GLfloat color[3];
} Flare;

/**
  *@author Enrico Reimer
  */
class SGLLensFlare : public SGLHelper
{
public:
	struct _ImageRec
	{
		unsigned short imagic;
		unsigned short type;
		unsigned short dim;
		unsigned short xsize, ysize, zsize;
		unsigned int min, max;
		unsigned int wasteBytes;
		char name[80];
		unsigned long colorMap;
		FILE *file;
		unsigned char *tmp;
		unsigned long rleEnd;
		unsigned int *rowStart;
		int *rowSize;
	};

private:
	GLfloat red[3];
	GLfloat green[3];
	GLfloat blue[3];
	GLuint flareTex[6], shineTex[10];
	SGLCamera *Camera;
	SGLLight  *Light;


	Flare flare[20];
	int num_flares;
	float position[3];
	GLfloat	size;

	void ConvertShort(unsigned short *array, unsigned int length);
	void ConvertUint(unsigned *array, unsigned int length);
	_ImageRec *ImageOpen(char *fileName);
	void ImageClose(_ImageRec * image);
	void ImageGetRow(_ImageRec * image, unsigned char *buf, int y, int z);
	unsigned char *load_luminance(char *name, int *width, int *height, int *components);

public:
	SGLLensFlare(SGLCamera *Camera,SGLLight  *Light);
	~SGLLensFlare();

	virtual SGLVektor getCenter();
	virtual void generate();
	Flare set_flare(int type, float location, float scale, GLfloat color[3], float colorScale);
	void init_flares();
	void DoFlares(SGLCamera *cam, GLfloat near_clip);
	void setup_texture(char *filename, GLuint texobj, GLenum minFilter, GLenum maxFilter);
	void load_textures();
    bool isVisible(SGLCamera *cam);
};

#endif
