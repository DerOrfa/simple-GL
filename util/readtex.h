/* readtex.h */

#ifndef READTEX_H
#define READTEX_H


#ifdef WIN32
	#include <windows.h>								// Header File For Windows
	#include <gl\gl.h>								// Header File For The OpenGL32 Library
	#include <gl\glu.h>								// Header File For The GLu32 Library
	#include <gl\glaux.h>								// Header File For The GLaux Library
#else
	#include <GL/gl.h>
#endif

extern GLboolean
LoadRGBMipmaps( const char *imageFile, GLint intFormat );


extern GLboolean
LoadRGBMipmaps2( const char *imageFile, GLenum target,
                 GLint intFormat, GLint *width, GLint *height );


extern GLubyte *
LoadRGBImage( const char *imageFile,
              GLint *width, GLint *height, GLenum *format );


#endif
