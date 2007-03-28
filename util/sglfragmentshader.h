//
// C++ Interface: sglfragmentshader
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLFRAGMENTSHADER_H
#define SGLFRAGMENTSHADER_H

#include "sglbaseshader.h"

/**
@author 
*/
class SGLFragmentShader : public SGLBaseShader
{
public:
    SGLFragmentShader(std::string _program);

    ~SGLFragmentShader();

    virtual bool loadShader();
    virtual bool unloadShader();

};

#endif
