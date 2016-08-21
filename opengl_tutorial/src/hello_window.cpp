/*
 * hello_window.cpp
 *
 *  Created on: Aug 20, 2016
 *      Author: rumman
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main( int argc, char** argv )
{
	// initialize and configure glfw
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	return 0;
}
