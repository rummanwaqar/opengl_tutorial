/*
 * shader.cpp
 *
 *  Created on: Aug 20, 2016
 *      Author: rumman
 */

#include <shader.hpp>

Shader::Shader( const GLchar* vertexPath, const GLchar* fragmentPath )
{
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	try
	{
		vertex = compileShader( vertexPath, GL_VERTEX_SHADER );
		fragment = compileShader( fragmentPath, GL_FRAGMENT_SHADER );
	}
	catch( std::exception& e )
	{
		exit(1);
	}

	// link program
	this->program = glCreateProgram();
	glAttachShader(this->program, vertex);
	glAttachShader(this->program, fragment);
	glLinkProgram(this->program);
	glGetProgramiv( this->program, GL_LINK_STATUS, &success );
	if( !success )
	{
		glGetProgramInfoLog( this->program, 512, NULL, infoLog );
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
		exit(1);
	}
	glDeleteShader( vertex );
	glDeleteShader( fragment );
}

void Shader::use()
{
	glUseProgram( this->program );
}

GLuint Shader::getProgram()
{
	return this->program;
}

void Shader::readFile( const GLchar* path, std::string& code )
{
	std::ifstream shaderFile;
	shaderFile.exceptions( std::ifstream::badbit );

	try
	{
		// open file
		shaderFile.open( path );
		std::stringstream shaderStream;
		// read contents into buffer
		shaderStream << shaderFile.rdbuf();
		// close the file
		shaderFile.close();
		if( shaderStream.str().length() < 10 )
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: improper length" << std::endl;
			throw std::exception();
		}
		code = shaderStream.str();
	}
	catch( std::ifstream::failure& e )
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
		throw e;
	}
}

GLuint Shader::compileShader( const GLchar* path, GLuint shaderType )
{
	std::string code;
	try
	{
		readFile( path, code );
	}
	catch ( std::exception& e )
	{
		throw e;
	}
	const GLchar* shaderCode = code.c_str();

	GLuint shader;
	GLint success;
	GLchar infoLog[512];
	shader = glCreateShader( shaderType );
	glShaderSource( shader, 1, &shaderCode, NULL );
	glCompileShader( shader );
	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( shader, 512, NULL, infoLog );
		std::cout << "ERROR::SHADER::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
		throw std::exception();
	}
	return shader;
}
