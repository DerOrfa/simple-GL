/***************************************************************************
 *   Copyright (C) 2003 by Enrico Reimer                                   *
 *   enni@Willow.Reimer                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sgltextbackend.h"
#include "../sglmisc.h"

SGLTextBackend::~SGLTextBackend()
{
}

void SGLTextBackend::replace(string &myText,string findText,string replString)
{
	string::size_type pos;
	while((pos = myText.find(findText)) && (pos < myText.length()))
		myText.replace(pos,findText.length(),replString);
}

