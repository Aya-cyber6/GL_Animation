#pragma once

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <string>
#include <vector>
#include "StaticMesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class StaticModel
{
public:
    // model data
    std::vector<Texture0> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    // constructor
    StaticModel(aiNode* node, const aiScene* scene, std::string dir ) {
        this->directory = dir;
        //printf("%d", directory);
        processNode(node, scene);
    }

    // draw the StaticModel
    void Draw(Shader& shader);

private:
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture0> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


