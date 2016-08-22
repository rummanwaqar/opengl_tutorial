/*
 * mesh.hpp
 *
 *  Created on: Aug 21, 2016
 *      Author: rumman
 */

#ifndef INCLUDE_MESH_HPP_
#define INCLUDE_MESH_HPP_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/types.h>

#include <shader.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	/* mesh data */
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	/*	Functions */
	Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures );
	void draw( Shader shader );

private:
	/* render data */
	GLuint VAO, VBO, EBO;

	/* Functions */
	void setupMesh();
};



#endif /* INCLUDE_MESH_HPP_ */
