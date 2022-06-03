#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Texture.hpp"
#include "Shader.hpp"

using std::vector;

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    vector<Vertex>         verticies;
    vector<unsigned int>   indicies;
    vector<Texture>        textures;

    Mesh(vector<Vertex> verticies, vector<unsigned int> indicies, vector<Texture> textures);
    void Render(Shader &shader);
private:
    // Render data
    unsigned int VAO, VBO, IBO;

    void setupMesh();
};