/*
 * mesh.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: rumman
 */

#include <mesh.hpp>

Mesh::Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures )
	: VAO(0), VBO(0), EBO(0)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

void Mesh::draw( Shader shader )
{
	// bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for( GLuint i =0; i < this->textures.size(); i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i );
		std::stringstream ss;
		std::string number;
		std::string name = this->textures[i].type;
		if( name == "texture_diffuse" )
			ss << diffuseNr++;
		else if ( name == "texture_specular" )
			ss << specularNr++;
		number = ss.str();
		// set the sample to the correct texture unit
		glUniform1i( glGetUniformLocation( shader.getProgram(), ("material."+name+number).c_str() ), i );
		// bind the texture
		glBindTexture( GL_TEXTURE_2D, this->textures[i].id );
	}
	glUniform1f( glGetUniformLocation( shader.getProgram(), "material.shininess" ), 16.0f );

	// draw mesh
	glBindVertexArray( this->VAO );
		glDrawElements( GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0 );
	glBindVertexArray(0);

	// unbind textures
	for( GLuint i=0; i < this->textures.size(); i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
}

void Mesh::setupMesh()
{
	glGenVertexArrays( 1, &this->VAO );
	glGenBuffers( 1, &this->VBO );
	glGenBuffers( 1, &this->EBO );

	glBindVertexArray( this->VAO );
		// populate buffers
		glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
		glBufferData( GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
				&this->vertices[0], GL_STATIC_DRAW );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
				&this->indices[0], GL_STATIC_DRAW );
		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid*)0 );
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid*)offsetof(Vertex, Normal) );
		glEnableVertexAttribArray(2);
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid*)offsetof(Vertex, TexCoords) );
	glBindVertexArray(0);
}
