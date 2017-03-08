#include "Mesh.h"
#include <iostream>

Mesh::~Mesh()
{
}


void Mesh::setupVBO() {
	//get size's of buffers
	long cBufferSize = colours.size() * sizeof(glm::vec4),
		vBufferSize = vertices.size() * sizeof(glm::vec3),
		nBufferSize = normals.size() * sizeof(glm::vec3),
		uvBufferSize = uvs.size() * sizeof(glm::vec2);

	//create and bind the VBO
	glGenBuffers(TOTAL_NUMBER_OF_ATTRIBUTES, vbo);
	glGenBuffers(1, &ebo);

	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Allocate buffer and put data into it
	glBufferData(GL_ARRAY_BUFFER, vBufferSize, &vertices[0],
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, cBufferSize, &colours[0],
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nBufferSize, &normals[0],
		GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, uvBufferSize, &uvs[0],
		GL_STATIC_DRAW);

	//load the texture if there exists one
	if (texture != NULL)
		texture->Load();
}

void Mesh::setupVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// assign vbo's to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, GL_FALSE,
		(const GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, 0, GL_FALSE,
		(const GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, 0, GL_FALSE,
		(const GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, 0, GL_FALSE,
		(const GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
