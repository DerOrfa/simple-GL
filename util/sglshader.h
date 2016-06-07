#ifndef SGLSHADER_H
#define SGLSHADER_H
#include <string>
#include <map>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

/**
 * Class SGLShader
 * 
 */
class SGLShaderProgram {
	class shader{
	public:
		shader(const std::string _src);
		shader();
		bool attached,compiled;
		std::string src,compileLog;
	};
/**
 * Public stuff
 */
public:
	/**
	 * Fields
	 */
	 std::map<GLuint ,shader >  shaders;
	 bool ready,valid;
	/**
	 * 
	 */
	/**
	 * Constructors
	 */
	SGLShaderProgram();
	~SGLShaderProgram();
	/**
	* Operations
	*/
	/**
	* 
	* @param src 
	*/
	GLuint  addFragment (const std::string src);
	/**
	* 
	* @param src 
	*/
	GLuint  addVertex (const std::string src);
	/**
	* 
	*/
	bool  isOK ();
	/**
	* 
	*/
	bool  compile ();
	/**
	* 
	*/
	bool set_uniformv(std::string name,const GLint values[],GLsizei cnt);
	bool set_uniformv(std::string name,const GLfloat values[],GLsizei cnt);
    bool loadShader();
    bool unloadShader();
/**
 * Protected stuff
 */
protected:
/**
 * Private stuff
 */
private:
	/**
	* Fields
	*/
	GLenum ID;
	/**
	* 
	*/
	/**
	* Operations
	*/
	/**
	* 
	* @param src 
	* @param type 
	*/
	GLuint  addShader (const std::string src, GLenum type);
	/**
	* 
	*/
	bool  linkShaders();
    void getStatus(GLenum pname,GLint * 	params);
    bool compileShader(GLuint id);
	bool compileShader(GLuint id,shader sh);
};
#endif //SGLSHADER_H

