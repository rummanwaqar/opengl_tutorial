/*
 * shader.hpp
 *
 *  Created on: Aug 20, 2016
 *      Author: rumman
 */

#ifndef INCLUDE_SHADER_HPP_
#define INCLUDE_SHADER_HPP_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include <GL/glew.h>

class Shader
{
public:
	// reads vs and fs paths and build a shader program
	Shader( const GLchar* vertexPath, const GLchar* fragmentPath );
	// use the program
	void use();
	// return pointer to program
	GLuint getProgram();

private:
	GLuint program;

	// read file
	void readFile( const GLchar* path, std::string& code );
	// compile shaders
	GLuint compileShader( const GLchar* code, GLuint shaderType );
};


#endif /* INCLUDE_SHADER_HPP_ */
