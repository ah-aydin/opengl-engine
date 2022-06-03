#include "Mesh.hpp"

#include <string>

using std::string;

Mesh::Mesh(vector<Vertex> verticies, vector<unsigned int> indicies, vector<Texture> textures)
{
    this->verticies = verticies;
    this->indicies = indicies;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    // Create vertex array object and the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    // Supply buffer data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

    // Data pointers
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // Vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Render(Shader &shader)
{
    // Bind the textures
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int slot = 0;
    for (auto texture = textures.begin(); texture != textures.end(); ++texture)
    {
        // Bind the texture to the slot
        texture->bind(slot);

        // Build the texture uniform name in the shader program
        string number;
        string name = texture->getType();
        if (name == "texture_diffuse")
        {
            number = std::to_string(++diffuseNr);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(++specularNr);
        }

        // Set texture uniform
        shader.setInt(("material." + name + number).c_str(), slot);
        slot++;
    }

    // Render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}