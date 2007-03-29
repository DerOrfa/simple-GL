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
#include <GL/gl.h>

/**
@author 
*/
class SGLBaseShader{
	struct {unsigned int shader,program;}ID;
	bool compiled;
public:
	SGLBaseShader(std::string _program);
	
	virtual ~SGLBaseShader();
	virtual bool unloadShader() = 0;
	virtual bool loadShader() = 0;
	unsigned int compile();	
	template<class T> void set_uniform(std::string name, const T val1,const T val2,const T val3,const T val4)
	{
		const T vals[]={val1,val2,val3,val4};
		set_uniformv(name,vals,4);
	}
	bool set_uniformv(std::string name,const GLint values[],GLsizei cnt);
	bool set_uniformv(std::string name,const GLfloat values[],GLsizei cnt);
public:
	std::string program;
};



#endif
