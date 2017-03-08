#include "ObjLoader.h"


ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

Object *ObjLoader::loadFromFile(const char *fileName, Object *obj) {
	return loadFromFile(std::string(fileName));
}

Object *ObjLoader::loadFromFile(std::string fileName, Object *obj) {
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return NULL;
	}
	// Retrieve the directory path of the filepath
	directory = fileName.substr(0, fileName.find_last_of('/'));

	return processScene(scene);
}


Object *ObjLoader::processScene(const aiScene *scene, Object *obj) {
	if (obj == NULL)
		obj = new Object();

	// Process each mesh 
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		// Data to fill
		std::vector<glm::vec3> vertices, normals; 
		std::vector<glm::vec4> colours;
		std::vector<glm::vec2> uvs;
		std::vector<GLuint> indices;
		Texture *texture = NULL;

		// Walk through each of the mesh's vertices
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = mesh->mVertices[j].x;
			vector.y = mesh->mVertices[j].y;
			vector.z = mesh->mVertices[j].z;
			vertices.push_back(vector);

			// Normals
			vector.x = mesh->mNormals[j].x;
			vector.y = mesh->mNormals[j].y;
			vector.z = mesh->mNormals[j].z;
			normals.push_back(vector);

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][j].x;
				vec.y = mesh->mTextureCoords[0][j].y;
				uvs.push_back(vec);
			}
			else{
				uvs.push_back(glm::vec2(0.0f, 0.0f));
			}
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiString str;
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &str)) {
				texture = new Texture(GL_TEXTURE_2D, (directory + '/' + std::string(str.C_Str())).c_str());
			}

			aiColor4D colour;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, colour)) {
				for (unsigned int j = 0; j < vertices.size(); j++) {
					colours.push_back(glm::vec4(colour.r, colour.g, colour.b, colour.a));
				}
			}
			else {
				for (unsigned int j = 0; j < vertices.size(); j++) {
					colours.push_back(glm::vec4(1.0, 1.0, 1.0, 1));
				}
			}
		}
		
		Mesh *newMesh = new Mesh(vertices, uvs, colours, normals, indices, texture, mesh->mNumFaces);
		obj->addMesh(newMesh);
	}

	//setup model's VBO's and VAO's
	obj->setupVBOs();
	obj->setupVAOs();

	return obj;
}
