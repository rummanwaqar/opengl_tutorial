/*
 * model.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: rumman
 */

#include "model.hpp"

GLint TextureFromFile(const char* path, std::string directory);

Model::Model( GLchar* path )
{
	this->loadModel( path );
}

void Model::draw( Shader shader )
{
	for( GLuint i=0; i<this->meshes.size(); i++ )
		this->meshes[i].draw( shader );
}

void Model::loadModel( std::string path )
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );

	if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		exit(1);
	}
	this->directory = path.substr( 0, path.find_last_of('/') );
	this->processNode( scene->mRootNode, scene );
}

void Model::processNode( aiNode* node, const aiScene* scene )
{
	// process all the nodes meshes
	for( GLuint i=0; i < node->mNumMeshes; i++ )
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back( this->processMesh(mesh, scene) );
	}

	// process each child node
	for( GLuint i=0; i < node->mNumChildren; i++ )
		this->processNode( node->mChildren[i], scene );
}

Mesh Model::processMesh( aiMesh* mesh, const aiScene* scene )
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for( GLuint i=0; i<mesh->mNumVertices; i++ )
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		if( mesh->mTextureCoords[0] )
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for( GLuint i=0; i<mesh->mNumFaces; i++ )
	{
		aiFace face = mesh->mFaces[i];
		for( GLuint j=0; j<face.mNumIndices; j++ )
			indices.push_back(face.mIndices[j]);
	}

	if( mesh->mMaterialIndex >= 0 )
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
		textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );
		std::vector<Texture> specularMaps = this->loadMaterialTextures( material, aiTextureType_SPECULAR, "texture_specular" );
		textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );
	}

	return Mesh( vertices, indices, textures );
}

std::vector<Texture> Model::loadMaterialTextures( aiMaterial* mat, aiTextureType type, std::string typeName )
{
	std::vector<Texture> textures;
	for( GLuint i=0; i<mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture( type, i, &str );
		// check if texture is already loaded
		GLboolean skip = false;
		for( GLuint j=0; j<textures_loaded.size(); j++ )
		{
			if( textures_loaded[j].path == str )
			{
				textures.push_back( textures_loaded[j] );
				skip = true;
				break;
			}
		}
		if( !skip )
		{
			// if texture isn't already loaded, load texture
			Texture texture;
			texture.id = TextureFromFile( str.C_Str(), this->directory );
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back( texture );
		}
	}
	return textures;
}

GLint TextureFromFile(const char* path, std::string directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures( 1, &textureID );
	int width, height;
	unsigned char* image = SOIL_load_image( filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB );
	if( image == NULL )
	{
		std::cerr << "TEXTURE::NOT_LOADED::" << filename << std::endl;
		return -1;
	} else {
		std::cout << "Loaded: " << filename << std::endl;
	}
	// assign texture to ID
	glBindTexture( GL_TEXTURE_2D, textureID );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
		glGenerateMipmap( GL_TEXTURE_2D );

		// parameters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture( GL_TEXTURE_2D, 0 );
	SOIL_free_image_data(image);
	return textureID;
}
