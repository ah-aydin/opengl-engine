#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "./Mesh.hpp"

class Model
{
public:
    Model(std::string path)
    {
        loadModel(path);
    }
    void Render(Shader &shader);
private:
    std::vector<Mesh> meshes;
    std::string directory;
    // Store the allready loaded textures
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};