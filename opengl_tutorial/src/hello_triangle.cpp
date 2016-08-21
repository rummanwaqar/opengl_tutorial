/*
 * hello_triangle.cpp
 *
 *  Created on: Aug 20, 2016
 *      Author: rumman
 */

#include <iostream>

#define GL_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fraagmentShaderSource = "#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main( int argc, char** argv )
{
	/*
	 * Initialize
	 */
	// initialize and configure glfw
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	// create a GLFW window object
	GLFWwindow* window = glfwCreateWindow( 800, 600, "Hello Triangle", nullptr, nullptr );
	if( window == nullptr )
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// set callbacks
	glfwSetKeyCallback(window, key_callback);

	// initialize glew before using any opengl functions
	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK )
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// set size of rendering window
	int width, height;
	glfwGetFramebufferSize( window, &width, &height );
	glViewport( 0, 0, width, height );

	/*
	 * Shaders
	 */
	// build and compile the vertex shader
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
	glCompileShader( vertexShader );
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
	}
	// build and compile the fragment shader
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShader, 1, &fraagmentShaderSource, NULL );
	glCompileShader( fragmentShader );
	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
	}
	// shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glLinkProgram( shaderProgram );
	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
	if( !success )
	{
		glGetProgramInfoLog( shaderProgram, 512, NULL, infoLog );
		std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED" << std::endl << infoLog << std::endl;
	}
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );


	/*
	 * Create the object
	 */
	// input for triangle
	GLfloat vertices[] = {
	    -0.5f, -0.5f, 0.0f,
	     0.5f, -0.5f, 0.0f,
	     0.0f,  0.5f, 0.0f
	};
	// create vertex buffer object and vertex array object
	GLuint VBO, VAO;
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glBindVertexArray(VAO);
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*) 0 );
		glEnableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray(0);

	// game loop
	while( !glfwWindowShouldClose(window) )
	{
		glfwPollEvents();

		// Clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// rendering triangle here
		glUseProgram( shaderProgram );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray(0);

		glfwSwapBuffers( window );
	}

	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when user presses esc close the window
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );
}

