/***************************************************************************
                          eglgrid.cpp  -  description
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

#include "sglgrid.h"
#include "../sglmisc.h"

SGLGrid::SGLGrid(short GridType)
{
	Type=GridType;
	Size=5;
}
SGLGrid::~SGLGrid()
{
}

/** No descriptions */
void SGLGrid::generate()
{
	GLint i,j;
	switch(Type)
	{
	case 1:
		{
			glColor3f(0, 0.8, 0);
			glBegin(GL_LINES);

			glVertex2f(0,-Size-0.5);glVertex2f(0,Size+0.5);
			glVertex2f(-0.2,Size+0.3);glVertex2f(0,Size+0.5);glVertex2f(0.2,Size+0.3);glVertex2f(0,Size+0.5);

			glVertex2f(-Size-0.5,0);glVertex2f(Size+0.5,0);
			glVertex2f(Size+0.3,-0.2);glVertex2f(Size+0.5,0);glVertex2f(Size+0.3,0.2);glVertex2f(Size+0.5,0);

			glVertex3f(0,0,-Size-0.5);glVertex3f(0,0,Size+0.5);
			glVertex3f(-0.2,0,Size+0.3);glVertex3f(0,0,Size+0.5);glVertex3f(0.2,0,Size+0.3);glVertex3f(0,0,Size+0.5);

			glEnd();
		}break;
	case 2:
		{
			glColor3f(0.8, 0, 0);
			glBegin(GL_LINES);
 			for (i = -Size; i <= Size; i++)if(i){glVertex2i(i, -Size);glVertex2i(i, Size);}
			for (i = -Size; i <= Size; i++)if(i){glVertex2i(-Size, i);glVertex2i(Size, i);}
			glEnd();

			glBegin(GL_LINES);
			for (i = -Size; i <= Size; i++)if(i){glVertex3i(0, i,Size);glVertex3i(0, i,-Size);}
			for (i = -Size; i <= Size; i++)if(i){glVertex3i(0, Size,i);glVertex3i(0, -Size,i);}
			glEnd();

			glBegin(GL_LINES);
			for (i = -Size; i <= Size; i++)if(i){glVertex3i(i, 0,Size);glVertex3i(i, 0,-Size);}
			for (i = -Size; i <= Size; i++)if(i){glVertex3i(Size, 0,i);glVertex3i(-Size, 0,i);}
			glEnd();
		}break;
	case 3:
		{
				glColor3f(0, 0, 0.8);
				for (i = 1; i <= Size; i++)
					{
						glBegin(GL_LINE_LOOP);
							glVertex3i(-i, -i,-i);
							glVertex3i(i, -i,-i);
							glVertex3i(i, i,-i);
							glVertex3i(-i, i,-i);
						glEnd();
						glBegin(GL_LINE_LOOP);
							glVertex3i(-i, -i,i);
							glVertex3i(i, -i,i);
							glVertex3i(i, i,i);
							glVertex3i(-i, i,i);
						glEnd();
						glBegin(GL_LINES);
							glVertex3i(-i, -i,-i);glVertex3i(-i, -i,i);
							glVertex3i(i, -i,-i);glVertex3i(i, -i,i);
							glVertex3i(i, i,-i);glVertex3i(i, i,i);
							glVertex3i(-i, i,-i);glVertex3i(-i, i,i);
						glEnd();
					}
		}break;
	case 4:
		{
			glColor3f(0.2, 0.2, 0.2);
			glBegin(GL_LINES);
			for(j=-Size;j<=Size;j++)
			{
				for (i = -Size; i <= Size; i++)if(i){glVertex3i(i, -Size,j);glVertex3i(i, Size,j);}
				for (i = -Size; i <= Size; i++)if(i){glVertex3i(-Size, i,j);glVertex3i(Size, i,j);}
				for (i = -Size; i <= Size; i++)if(i){glVertex3i(i, j,-Size);glVertex3i(i, j,Size);}
			}
			glEnd();
		}break;
	}
}

/*!
    \fn SGLGrid::getCenter()
 */
SGLVektor SGLGrid::getCenter()const
{
// @todo "not really implemented yet!!"
	return SGLVektor(0,0,0);
}



/*!
    \fn SGLGrid::setSize(GLuint size)
 */
void SGLGrid::setSize(GLuint size)
{
	Size=size;
	Compile(false,is_free);
}
