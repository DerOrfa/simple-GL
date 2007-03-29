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

SGLBaseShader::SGLBaseShader(GLenum type,std::string _program):program(_program),compiled(false)
{
	ID.program = glCreateShader(type);
	ID.shader = glCreateShader(type);
	if(ID.shader==0)SGLprintError("Shader konnte nicht angelegt werden");
	if(ID.program==0)SGLprintError("Programmobjekt konnte nicht angelegt werden");
	SGLSpace::printErrors();
}


SGLBaseShader::~SGLBaseShader(){}

unsigned int SGLBaseShader::compile() 
{
	assert(ID);
	const char* src=program.c_ptr();
	
	glShaderSource(ID, 1, &src, 0);
	glCompileShader(ID);
	
	glGetObjectParameteriv(ID, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if(!success) {
		SGLprintError("shader compilation failed\n");
		return 0;
	}

	prog = glCreateProgramObjectARB();
	glAttachObjectARB(prog, sdr);
	glLinkProgramARB(prog);
	glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, &linked);
	if(!linked) {
		SGLprintError("shader linking failed\n");
		return 0;
	}

	glUseProgramObjectARB(prog);
	return prog;
}



bool SGLBaseShader::set_uniformv(std::string name,const GLfloat values[],GLsizei cnt)
{
	int loc = glGetUniformLocationARB(prog, name.c_str());
	if(loc != -1) switch(cnt)
	{
		case 1:glUniform1fv(loc, cnt,values);break;
		case 2:glUniform2fv(loc, cnt,values);break;
		case 3:glUniform3fv(loc, cnt,values);break;
		case 4:glUniform4fv(loc, cnt,values);break;
		default:
			SGLprintError("Ungültige Dimension %d für die Uniforme %s",cnt,name.c_str());
			return false;
	}
	else 
	{
		SGLprintError("Die Uniforme %s ist nicht verfügbar",name.c_str());
		return false;
	}
	return SGLSpace::printErrors();
}

bool SGLBaseShader::set_uniformv(std::string name,const GLint values[],GLsizei cnt)
{
	int loc = glGetUniformLocationARB(prog, name.c_str());
	if(loc != -1) switch(cnt)
	{
		case 1:glUniform1iv(loc, cnt,values);break;
		case 2:glUniform2iv(loc, cnt,values);break;
		case 3:glUniform3iv(loc, cnt,values);break;
		case 4:glUniform4iv(loc, cnt,values);break;
		default:
			SGLprintError("Ungültige Dimension %d für die Uniforme %s",cnt,name.c_str());
			return false;
	}
	else 
	{
		SGLprintError("Die Uniforme %s ist nicht verfügbar",name.c_str());
		return false;
	}
	return SGLSpace::printErrors();
}
