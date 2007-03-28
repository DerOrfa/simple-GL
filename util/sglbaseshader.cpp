//
// C++ Implementation: sglbaseshader
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sglbaseshader.h"

SGLBaseShader::SGLBaseShader(std::string _program):program(_program)
{
}


SGLBaseShader::~SGLBaseShader()
{
}

unsigned int SGLBaseShader::setup_shader() 
{
	sdr = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(sdr, 1, (const char**)&src_buf, 0);
	free(src_buf);

	glCompileShaderARB(sdr);
	glGetObjectParameterivARB(sdr, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if(!success) {
		fprintf(stderr, "shader compilation failed\n");
		return 0;
	}

	prog = glCreateProgramObjectARB();
	glAttachObjectARB(prog, sdr);
	glLinkProgramARB(prog);
	glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, &linked);
	if(!linked) {
		fprintf(stderr, "shader linking failed\n");
		return 0;
	}

	glUseProgramObjectARB(prog);
	return prog;
}

void set_uniform1f(unsigned int prog, const char *name, float val) {
	int loc = glGetUniformLocationARB(prog, name);
	if(loc != -1) {
		glUniform1f(loc, val);
	}
}

void set_uniform2f(unsigned int prog, const char *name, float v1, float v2) {
	int loc = glGetUniformLocationARB(prog, name);
	if(loc != -1) {
		glUniform2f(loc, v1, v2);
	}
}

void set_uniform1i(unsigned int prog, const char *name, int val) {
	int loc = glGetUniformLocationARB(prog, name);
	if(loc != -1) {
		glUniform1i(loc, val);
	}
}
