//
// C++ Interface: sglbaseshader
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLBASESHADER_H
#define SGLBASESHADER_H

#include <string>

/**
@author 
*/
class SGLBaseShader{
public:
	SGLBaseShader(std::string _program);
	
	virtual ~SGLBaseShader();
	virtual bool unloadShader() = 0;
	virtual bool loadShader() = 0;
	unsigned int setup_shader(const char *fname);	
	void set_uniform1f(unsigned int prog, const char *name, float val);
	void set_uniform2f(unsigned int prog, const char *name, float v1, float v2);
	void set_uniform1i(unsigned int prog, const char *name, int val);

public:
	std::string program;
};



#endif
