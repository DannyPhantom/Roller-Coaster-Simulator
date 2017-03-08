#include "Object.h"

Object::Object()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}


Object::~Object()
{
}


void Object::setupVAOs() {
	//no need to create VAO's if they've already been created
	if (vaoSetup)
		return;

	//setup VAO's for each mesh separately
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes.at(i)->setupVAO();
	}

	vaoSetup = true;

}

void Object::setupVBOs() {
	//no need to create VBO's if they've already been created
	if (vboSetup)
		return;

	//setup VBO's for each mesh separately
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes.at(i)->setupVBO();
	}

	vboSetup = true;

}

void Object::draw(GLuint program, glm::mat4 viewMatrix)
{
	//setup general uniforms
	glUniform1i(glGetUniformLocation(program, "TextureUniform"), 0);
	glm::mat4 mvMatrix = viewMatrix * modelMatrix;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mvMatrix)));
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, false, &mvMatrix[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, false, &normalMatrix[0][0]);

	//process and draw one mesh at a time
	for (unsigned int i = 0; i < meshes.size(); i++) {
		Mesh *mesh = meshes.at(i);
		//bind the texture for the object if there is one
		if (mesh->hasTexture()) {
			mesh->getTexture()->Bind(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(program, "TextureValid"), true);
		}
		else {
			glUniform1i(glGetUniformLocation(program, "TextureValid"), false);
		}
		// Draw mesh
		glBindVertexArray(mesh->getVAO());
		glDrawElements(GL_TRIANGLES, mesh->getIndices()->size(), GL_UNSIGNED_INT, 0);

		//disable everything
		glBindVertexArray(0);
		if (mesh->hasTexture()) {
			mesh->getTexture()->unBind(GL_TEXTURE0);
		}
	}
}

void Object::recalculateModelMatrix() {
	//translation only
	glm::mat4x4 translate = glm::translate(glm::mat4x4(), position);

	//translation * rotation x
	glm::mat4x4 trx = glm::rotate(translate, rotation.x, glm::vec3(1, 0, 0));

	//translation * rotation x * rotation y
	glm::mat4x4 trxry = glm::rotate(trx, rotation.y, glm::vec3(0, 1, 0));

	//translation * rotation x * rotation y * rotation z
	glm::mat4x4 trxryrz = glm::rotate(trxry, rotation.z, glm::vec3(0, 0, 1));

	//translation * rotation x * rotation y * rotation z * scale
	glm::mat4x4 trxryrzs = glm::scale(trxryrz, this->scale);

	//store the result in the model matrix
	modelMatrix = trxryrzs;
}

void Object::setModelMatrix(glm::mat4 mm) {
	modelMatrix = mm;
}
