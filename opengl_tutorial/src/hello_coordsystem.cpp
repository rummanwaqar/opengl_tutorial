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
	GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "Hello CoordSystem", nullptr, nullptr );
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

	// enable 3d depth
	glEnable( GL_DEPTH_TEST );

	/*
	 * shaders and objects
	 */
	Shader myShader( "../opengl_tutorial/shaders/3dcube.vs", "../opengl_tutorial/shaders/textures.fs" );

	// set up vertex data for cube
	GLfloat vertices[] =
	{
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glm::vec3  cubePositions[] =
	{
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// create vertex objects
	GLuint VBO, VAO;
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glBindVertexArray( VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

		// position attributes
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0 );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)( 3 * sizeof(GL_FLOAT) ) );
		glEnableVertexAttribArray( 1 );
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
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// bind texture
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texture1 );
		glUniform1i( glGetUniformLocation( myShader.getProgram(), "ourTexture1" ), 0 );
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, texture2 );
		glUniform1i( glGetUniformLocation( myShader.getProgram(), "ourTexture2" ), 1 );

		// activate shader
		myShader.use();

		// create transforms
		glm::mat4 view, projection;
		view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );
		projection = glm::perspective( glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f );
		glUniformMatrix4fv( glGetUniformLocation( myShader.getProgram(), "view" ), 1, GL_FALSE, glm::value_ptr(view) );
		glUniformMatrix4fv( glGetUniformLocation( myShader.getProgram(), "projection" ), 1, GL_FALSE, glm::value_ptr(projection) );

		// draw container
		glBindVertexArray( VAO );
			for( GLuint i=0; i<10; i++ )
			{
				glm::mat4 model;
				model = glm::translate( model, cubePositions[i] );
				GLfloat angle = 20.0 * i;
				model = glm::rotate( model, glm::radians( angle ), glm::vec3( 1.0f, 0.3f, 0.5f ) );
				glUniformMatrix4fv( glGetUniformLocation( myShader.getProgram(), "model" ), 1, GL_FALSE, glm::value_ptr(model) );
				glDrawArrays( GL_TRIANGLES, 0, 36 );
			}
		glBindVertexArray( 0 );

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
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
