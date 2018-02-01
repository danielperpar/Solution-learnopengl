#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class Model
{
public:

	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gamma_correction;

	Model(const std::string &path, bool gamma = false);
	void Draw(Shader shader);

private:
	
	void LoadModel(const std::string &path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string type_name);
	unsigned int TextureFromFile(const char *path, const std::string directory, bool gamma);
};

#endif
