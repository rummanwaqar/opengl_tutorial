/*
 * hello_transformations.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: rumman
 */

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

// function prototypes
void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode );
GLuint loadTexture( std::string path );

// window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int main( int argc, char** argv )
{
	/*
	 * initialize
	 */
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	// create a window object
	GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "Hello Textures", nullptr, nullptr );
	if( window == nullptr )
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );
	glfwSetKeyCallback( window, key_callback );

	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK )
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// define view port dims
	int width, height;
	glfwGetFramebufferSize( window, &width, &height );
	glViewport( 0, 0, width, height );

	/*
	 * shaders and objects
	 */
	Shader myShader( "../opengl_tutorial/shaders/transform.vs", "../opengl_tutorial/shaders/textures.fs" );

	// set up vertex data for rectangle
	GLfloat vertices[] =
	{
		// Positions			// Colors			// Texture Coords
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f		// top left
	};
	GLuint indices[] = {
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};
	// create vertex objects
	GLuint VBO, VAO, EBO;
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &EBO );
	glBindVertexArray( VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

		// position attributes
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0 );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)( 3 * sizeof(GL_FLOAT) ) );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)( 6 * sizeof(GL_FLOAT) ) );
		glEnableVertexAttribArray( 2 );
	glBindVertexArray( 0 ); // unbind VAO

	// generate a texture
	GLuint texture1, texture2;
	texture1 = loadTexture( "../opengl_tutorial/textures/container.jpg" );
	texture2 = loadTexture( "../opengl_tutorial/textures/awesomeface.png" );

	/*
	 * loop
	 */
	while( !glfwWindowShouldClose(window) )
	{
		glfwPollEvents();

		// render
		// clear the color buffer
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		// activate shader
		myShader.use();
		// transform the box
		glm::mat4 trans;
		trans = glm::translate( trans, glm::vec3(0.5f, -0.5f, 0.0f) );
		trans = glm::rotate( trans, glm::radians( (GLfloat)glfwGetTime() * 50.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
		glUniformMatrix4fv( glGetUniformLocation( myShader.getProgram(), "transform" ), 1, GL_FALSE, glm::value_ptr(trans) );
		// bind texture
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texture1 );
		glUniform1i( glGetUniformLocation( myShader.getProgram(), "ourTexture1" ), 0 );
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, texture2 );
		glUniform1i( glGetUniformLocation( myShader.getProgram(), "ourTexture2" ), 1 );
		// draw container
		glBindVertexArray( VAO );
			glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &EBO );
	glfwTerminate();
	return 0;
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mode )
{
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );
}

GLuint loadTexture( std::string path )
{
	GLuint texture;
	int width, height;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
		// set texture wrapping / filtering options
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		// load texture image
		unsigned char* image = SOIL_load_image( path.c_str(), &width, &height, 0, SOIL_LOAD_RGB );
		if( image == NULL )
		{
			std::cerr << "TEXTURE::NOT_LOADED::" << path << std::endl;
			return -1;
		}
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
		glGenerateMipmap( GL_TEXTURE_2D );
		SOIL_free_image_data( image );
	glBindTexture( GL_TEXTURE_2D, 0 );
	return texture;
}
