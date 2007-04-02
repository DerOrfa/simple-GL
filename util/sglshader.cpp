#include <string>
#include "sglshader.h"
#include "../sglspace.h"

/**
 * Constructors/Destructors
 */
SGLShaderProgram::SGLShaderProgram()
{
	ID = glCreateProgram();
	if(ID==0)SGLprintError("Shaderprogramm konnte nicht angelegt werden");
	SGLSpace::printErrors();
}
SGLShaderProgram::~SGLShaderProgram()
{
	for(std::map<GLuint ,shader >::iterator i=shaders.begin();i!=shaders.end();i++)
	{
		const GLuint id=i->first;
		if(id)glDeleteShader(id);
		else SGLprintWarning("Versuch ungültigen Shader zu löschen");
	}
	shaders.clear();
	if(ID)
	{
		glDeleteProgram(ID);
		ID=0;
	}
	else SGLprintWarning("Versuch ungültiges Shaderprogram zu löschen");
	SGLSpace::printErrors();
}

/**
 * Methods
 */
/**
 * 
 */
GLuint SGLShaderProgram::addShader (const std::string src, GLenum type)
{
	GLuint id=glCreateShader(type);
	if(id!=0)shaders[ID]=shader(src);
	SGLSpace::printErrors();
	return id;
}
GLuint SGLShaderProgram::addFragment (const std::string src)
{
	const GLuint ret=addShader(src,GL_FRAGMENT_SHADER);
	if(!ret)SGLprintError("Fragmentshader konnte nicht angelegt werden");
	return ret;
}
GLuint SGLShaderProgram::addVertex (const std::string src) 
{
	const GLuint ret=addShader(src,GL_VERTEX_SHADER);
	if(!ret)SGLprintError("Vertexshader konnte nicht angelegt werden");
	return ret;
}
/**
 * 
 */
bool SGLShaderProgram::isOK () 
{
	GLint stat;
	getStatus( GL_LINK_STATUS,&stat);
	return stat==GL_TRUE;
}
/**
 * 
 */
bool SGLShaderProgram::compile () 
{
	GLint stat;
	bool ret=true;
	for(std::map<GLuint ,shader >::iterator it=shaders.begin();it!=shaders.end();it++)
	{
		shader &sh=it->second;
		const GLuint id=it->first;
		bool success=compileShader(id,sh);
		if(!sh.attached && success)glAttachShader(ID,id);
		ret&=success;
	}
	if(ret)SGLSpace::printErrors();
	else return ret;
	glLinkProgram(ID);
	glGetProgramiv(ID,GL_LINK_STATUS,&stat);
	SGLSpace::printErrors();
	return ready=(stat==GL_TRUE);
}
bool SGLShaderProgram::set_uniformv(std::string name,const GLfloat values[],GLsizei cnt)
{
	int loc = glGetUniformLocation(ID, name.c_str());
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

bool SGLShaderProgram::set_uniformv(std::string name,const GLint values[],GLsizei cnt)
{
	int loc = glGetUniformLocation(ID, name.c_str());
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


/*!
    \fn SGLShader::getStatus(GLenum pname,GLint * 	params)
 */
void SGLShaderProgram::getStatus(GLenum pname,GLint * params)
{
	assert(ID);
	glGetProgramiv(ID,pname,params);
}

SGLShaderProgram::shader::shader(std::string _src):src(_src){}

/*!
    \fn SGLShader::compileShader(GLuint id)
 */
bool SGLShaderProgram::compileShader(GLuint id)
{
	if(!glIsShader(id))
	{
		SGLprintError("Es gibt keinen shader mit der ID %d");
		return false;
	}
	std::map<GLuint ,shader >::iterator i=shaders.find(id);
	if(i==shaders.end())
	{
		SGLprintError("Der shader ID %d ist diesem Shaderprogramm nicht bekannt");
		return false;
	}	
	return compileShader(id,i->second);
}
bool SGLShaderProgram::compileShader(GLuint id,shader sh)
{
	GLint stat,len;
	const char* csrc=sh.src.c_str();
	glShaderSource(id, 1, &csrc, 0);
	glCompileShader(id);
	
	glGetShaderiv(id,GL_COMPILE_STATUS,&stat);
	glGetShaderiv(id,GL_INFO_LOG_LENGTH,&len);
	GLchar *log=(GLchar *)malloc(len+1);
	glGetShaderInfoLog(id,stat+1,NULL,log);
	sh.compileLog=std::string(log);
	
	if(stat!=GL_TRUE)
	{
		SGLprintError("Der Shader %d konnte nicht kompiliert werden");
		SGLprintWarning("compile-log:\n%s",log);
		return false;
	}
	return true;
}



/*!
    \fn SGLShaderProgram::loadShader()
 */
bool SGLShaderProgram::loadShader()
{
	if(!valid)
	{
		GLint stat;
		glValidateProgram(ID);
		getStatus(GL_VALIDATE_STATUS,&stat);
		if(stat==GL_FALSE)
		{
			SGLprintError("Shader %d ist nicht verfügbar");
			return false;
		}
	}
	glUseProgram(ID);
}

bool SGLShaderProgram::unloadShader()
{
	glUseProgram(0);
	return true;
}
