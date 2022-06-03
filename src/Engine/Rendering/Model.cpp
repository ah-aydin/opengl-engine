#include "Model.hpp"

#include <string.h>

#include "../../Logging/Log.hpp"

void Model::Render(Shader &shader)
{
    for (auto it = meshes.begin(); it != meshes.end(); ++it)
    {
        it->Render(shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        gl_log_error("ERROR:ASSIMP::%s", importer.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Process all the meshed in the node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }
    // Recursively call the child nodes
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> verticies;
    std::vector<unsigned int> indicies;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        // Process vertex positions, normals and texture coordinates
        glm::vec3 posVector;
        posVector.x = mesh->mVertices[i].x;
        posVector.y = mesh->mVertices[i].y;
        posVector.z = mesh->mVertices[i].z;

        glm::vec3 normVector;
        normVector.x = mesh->mNormals[i].x;
        normVector.y = mesh->mNormals[i].y;
        normVector.z = mesh->mNormals[i].z;

        glm::vec2 texCoord(0, 0);
        if (mesh->mTextureCoords[0]) // Check if the texture coordinates exist
        {
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
        }
        
        // Add into the list of verticies
        // TODO remove these comments if it works
        // Vertex vertex;
        // vertex.Position = posVector;
        // vertex.Normal = normVector;
        // vertex.TexCoords = texCoord;
        verticies.push_back({posVector, normVector, texCoord});
    }
    
    // Process indicies
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indicies.push_back(face.mIndices[j]);
        }
    }

    // Process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // Load diffuse textures
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Load specular textures
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(verticies, indicies, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str; // Variable to store the texture path
        mat->GetTexture(type, i, &str);
        gl_log("Texture location %s", str.C_Str());

        // If the texture is allready loaded, skip it
        bool skip = false;
        for (auto it = textures_loaded.begin(); it != textures_loaded.end(); ++it)
        {
            if (std::strcmp(it->path, str.C_Str()) == 0)
            {
                textures.push_back(*it);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture(str.C_Str(), typeName);
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}