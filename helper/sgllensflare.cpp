/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sgllensflare.h"
#include <GL/glu.h>
#include "../sglmisc.h"

void SGLLensFlare::ConvertShort(unsigned short *array, unsigned int length)
{
	unsigned short b1, b2;
	unsigned char *ptr;

	ptr = (unsigned char *) array;
	while (length--)
	{
		b1 = *ptr++;
		b2 = *ptr++;
		*array++ = (b1 << 8) | (b2);
	}
}

void SGLLensFlare::ConvertUint(unsigned *array, unsigned int length)
{
	unsigned int b1, b2, b3, b4;
	unsigned char *ptr;

	ptr = (unsigned char *) array;
	while (length--)
	{
		b1 = *ptr++;
		b2 = *ptr++;
		b3 = *ptr++;
		b4 = *ptr++;
		*array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
	}
}

SGLLensFlare::_ImageRec *SGLLensFlare::ImageOpen(char *fileName)
{
	union
	{
		int testWord;
		char testByte[4];
	} endianTest;
	SGLLensFlare::_ImageRec *image;
	int swapFlag, x;

	endianTest.testWord = 1;
	if (endianTest.testByte[0] == 1)swapFlag = 1;
	else swapFlag = 0;

	image = (_ImageRec *) malloc(sizeof(_ImageRec));
	if (image == NULL)
	{
		SGLprintError("Out of memory!");
		exit(1);
	}
	if ((image->file = fopen(fileName, "rb")) == NULL)
	{
		perror(fileName);
		exit(1);
	}
	fread(image, 1, 12, image->file);

	if (swapFlag)ConvertShort(&image->imagic, 6);

	image->tmp = (unsigned char *) malloc(image->xsize * 256);
	if (image->tmp == NULL)
	{
		SGLprintError("Out of memory!");
		exit(1);
	}
	if ((image->type & 0xFF00) == 0x0100)
	{
		x = image->ysize * image->zsize * (int) sizeof(unsigned);
		image->rowStart = (unsigned *) malloc(x);
		image->rowSize = (int *) malloc(x);
		if (image->rowStart == NULL || image->rowSize == NULL)
		{
			SGLprintError("Out of memory!");
			exit(1);
		}
		image->rleEnd = 512 + (2 * x);
		fseek(image->file, 512, SEEK_SET);
		fread(image->rowStart, 1, x, image->file);
		fread(image->rowSize, 1, x, image->file);
		if (swapFlag)
		{
			ConvertUint(image->rowStart, x / (int) sizeof(unsigned));
			ConvertUint((unsigned *) image->rowSize, x / (int) sizeof(int));
		}
	}
	return image;
}

void SGLLensFlare::ImageClose(_ImageRec * image)
{
	fclose(image->file);
	free(image->tmp);
	free(image);
}

void SGLLensFlare::ImageGetRow(_ImageRec * image, unsigned char *buf, int y, int z)
{
	unsigned char *iPtr, *oPtr, pixel;
	int count;

	if ((image->type & 0xFF00) == 0x0100)
	{
		fseek(image->file, (long) image->rowStart[y + z * image->ysize], SEEK_SET);
		fread(image->tmp, 1, (unsigned int) image->rowSize[y + z * image->ysize],
		image->file);
		iPtr = image->tmp;
		oPtr = buf;
		for (;;)
		{
			pixel = *iPtr++;
			count = (int) (pixel & 0x7F);
			if (!count)return;
			if (pixel & 0x80)
			{
				while (count--)
				*oPtr++ = *iPtr++;
			}
			else
			{
				pixel = *iPtr++;
				while (count--)	*oPtr++ = pixel;
			}
		}
	}
	else
	{
		fseek(image->file, 512 + (y * image->xsize) + (z * image->xsize * image->ysize),SEEK_SET);
		fread(buf, 1, image->xsize, image->file);
	}
}

unsigned char *SGLLensFlare::load_luminance(char *name, int *width, int *height, int *components)
{
	unsigned char *base, *lptr;
	_ImageRec *image;
	int y;

	image = ImageOpen(name);

	if (!image)return NULL;
	if (image->zsize != 1)return NULL;

	*width = image->xsize;
	*height = image->ysize;
	*components = image->zsize;

	base = (unsigned char *) malloc(image->xsize * image->ysize * sizeof(unsigned char));
	if (!base)return NULL;
	lptr = base;
	for (y = 0; y < image->ysize; y++)
	{
		ImageGetRow(image, lptr, y, 0);
		lptr += image->xsize;
	}
	ImageClose(image);
	return base;
}

/////////////////////////////////////////////////////

SGLLensFlare::SGLLensFlare(SGLCamera *Camera,SGLLight  *Light)
{
	red[0]=1;red[1]=red[2]=0;
	green[1] = 1;green[0]=green[2]=0;
	blue[2] =1;blue[0]=blue[1]=0;
	size=1;
	this->Camera=Camera;
	this->Light=Light;
	Light->link(*this);
	Camera->link(*this);
	init_flares();load_textures(); //muss NACH "this->Light=Light" kommen
	IgnoreClip=true;
	IgnoreLight=true;

	int i;
	priority=ontop;
}

SGLLensFlare::~SGLLensFlare()
{
//	Light->unlink(this);
//	Camera->unlink(this);
//@todo unlink ist outdated
}

SGLVektor SGLLensFlare::getCenter()
{
    return SGLVektor(0,0,0); // @todo not really implemented yet
}

void SGLLensFlare::generate()
{
	if(!isVisible(Camera))return;
	bool Texture2D=glIsEnabled(GL_TEXTURE_2D);
	bool DoBlend=glIsEnabled(GL_BLEND);

	GLint blendSrc,blendDst;
	glGetIntegerv(GL_BLEND_SRC,&blendSrc);
	glGetIntegerv(GL_BLEND_DST,&blendDst);


	if(!Texture2D)glEnable(GL_TEXTURE_2D);
	if(!DoBlend)glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);

	GLdouble clipface= Camera->ClipFace >= 1 ?  Camera->ClipFace+.01 :1;
	DoFlares(Camera,clipface);

	glEnable(GL_DEPTH_TEST);
	if(!Texture2D)glDisable(GL_TEXTURE_2D);
	if(!DoBlend)glDisable(GL_BLEND);
	glBlendFunc(blendSrc,blendDst);
}

Flare SGLLensFlare::set_flare(int type, float location, float scale, GLfloat color[3], float colorScale)
{
  Flare ret;

  ret.type = type;
  ret.loc = location;
  ret.scale = scale;
  ret.color[0] = color[0] * colorScale;
  ret.color[1] = color[1] * colorScale;
  ret.color[2] = color[2] * colorScale;

  return ret;
}

void SGLLensFlare::init_flares()
{
  /* Shines */
  flare[0] = set_flare(-1, 1.0f, 0.3f, blue, 1);
  flare[1] = set_flare(-1, 1.0f, 0.2f, green, 1);
  flare[2] = set_flare(-1, 1.0f, 0.25f, red, 1);

  /* Flares */
  flare[3] = set_flare(2, 1.3f, 0.04f, red, 0.6);
  flare[4] = set_flare(3, 1.0f, 0.1f, red, 0.4);
  flare[5] = set_flare(1, 0.5f, 0.2f, red, 0.3);
  flare[6] = set_flare(3, 0.2f, 0.05f, red, 0.3);
  flare[7] = set_flare(0, 0.0f, 0.04f, red, 0.3);
  flare[8] = set_flare(5, -0.25f, 0.07f, red, 0.5);
  flare[9] = set_flare(5, -0.4f, 0.02f, red, 0.6);
  flare[10] = set_flare(5, -0.6f, 0.04f, red, 0.4);
  flare[11] = set_flare(5, -1.0f, 0.03f, red, 0.2);
  num_flares = 12;
}

void SGLLensFlare::DoFlares(SGLCamera *cam, GLfloat near_clip)
{
	GLfloat dot;
	int i;
	int shine_tic = 0;

	SGLVektor LightPos=Light->getMyPos();
	SGLVektor from=cam->Pos;
	SGLVektor at=cam->LookAt;

	/* view_dir = normalize(at-from) */
	EVektor<GLdouble> view_dir= at - from;
	view_dir.Normalize();

	/* center = from + near_clip * view_dir */
	EVektor<GLdouble> center = from + (view_dir* near_clip);

	/* light_dir = normalize(light-from) */
	EVektor<GLdouble> light_dir=LightPos-from;
	light_dir.Normalize();

	/* light = from + dot(light,view_dir)*near_clip*light_dir */
	dot =
		light_dir.SGLV_X* view_dir.SGLV_X+
		light_dir.SGLV_Y* view_dir.SGLV_Y+
		light_dir.SGLV_Z* view_dir.SGLV_Z;

	LightPos= from+(light_dir*(near_clip/dot));

	EVektor<GLdouble> dx,dy;
	EVektor<GLdouble> axis;


	/* axis = light - center */
	axis= LightPos- center;

	if((EVektor<GLdouble>)LightPos !=  center)dx=axis;/* dx = normalize(axis) */
	else dx = from.kreuzprod(cam->UpVect); //dx ist senkrecht auf Kameravektor

	dx.Normalize();

	/* dy = cross(dx,view_dir) */
	dy = dx.kreuzprod(view_dir);

	for (i = 0; i < num_flares; i++)
	{
		EVektor<GLdouble> sx= dx* (flare[i].scale * size);
		EVektor<GLdouble> sy= dy* (flare[i].scale * size);

		if (flare[i].type < 0)
		{
			glBindTexture(GL_TEXTURE_2D, shineTex[shine_tic]);
			shine_tic = (shine_tic + 1) % 3;
		}
		else glBindTexture(GL_TEXTURE_2D, flareTex[flare[i].type]);

		/* position = center + flare[i].loc * axis */
		SGLVektor position= center+ (axis* flare[i].loc);

		SGLVektor A=position+ sx + sy;
		SGLVektor B=position- sx + sy;
		SGLVektor C=position- sx - sy;
		SGLVektor D=position+ sx - sy;

		LichtFarbe tColor=Light->getFarbeAt(from);
		glColor3f
		(
			flare[i].color[0]*tColor.Glanz[0]*2,
			flare[i].color[1]*tColor.Glanz[1]*2,
			flare[i].color[2]*tColor.Glanz[2]*2
		);
		glBegin(GL_QUADS);

			glTexCoord2f(0.0, 0.0);
			A.DrawPureVertex();

			glTexCoord2f(1.0, 0.0);
			B.DrawPureVertex();

			glTexCoord2f(1.0, 1.0);
			C.DrawPureVertex();

			glTexCoord2f(0.0, 1.0);
			D.DrawPureVertex();

		glEnd();

	}
}

void SGLLensFlare::setup_texture(char *filename, GLuint texobj, GLenum minFilter, GLenum maxFilter)
{
	unsigned char *buf;
	int width, height, components;

	glBindTexture(GL_TEXTURE_2D, texobj);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
	buf = load_luminance(filename, &width, &height, &components);

	SGLprintInfo("%s loaded: %dx%dx%d",filename, width, height, components);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf);
	free(buf);
}

void SGLLensFlare::load_textures(void)
{
#ifdef DATA_DIR
	#define PICS_DIR DATA_DIR "/pics"
#else
	#warning DATA_DIR ist nicht def.
	#define PICS_DIR "/opt/prog/simpleGL/libsgl/data/pics"
#endif
	
	char filename[256];
	GLenum minFilter, maxFilter;
	short id = 1, i;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	minFilter = GL_LINEAR_MIPMAP_LINEAR;
	maxFilter = GL_LINEAR;

	for (i = 0; i < 3; i++)
	{
		shineTex[i] = id;
		sprintf(filename, PICS_DIR "/Shine%d.bw", i);
		setup_texture(filename, shineTex[i], minFilter, maxFilter);
		id++;
	}
	for (i = 0; i < 6; i++)
	{
		flareTex[i] = id;
		sprintf(filename, PICS_DIR "/Flare%d.bw", i + 1);
		setup_texture(filename, flareTex[i], minFilter, maxFilter);
		id++;
	}
}


/*!
    \fn SGLLensFlare::isVisible(SGLCamera *cam)
 */
bool SGLLensFlare::isVisible(SGLCamera *cam)
{
	if(!Light->IsOn())return false;//Wenn's Licht aus is gibts kein Flare

	LichtFarbe tCol=Light->getFarbeAt(Camera->Pos);
	if(tCol.Glanz[0]+tCol.Glanz[1]+tCol.Glanz[2] < .05)return false;//Wenn's Licht zu dunkel is, gibts kein Flare

	SGLVektor blickricht=cam->Pos-cam->LookAt;
	SGLVektor Lichtricht=cam->Pos-Light->getMyPos();
	if(blickricht.skalarprod(Lichtricht) <0)return false;//Wenn das licht hinter der Kamera is => kein Flare
	return true;
}
