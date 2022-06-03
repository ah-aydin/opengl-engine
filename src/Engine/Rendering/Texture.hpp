#pragma once

#include <GL/glew.h>
#include <string>

using std::string;

/**
 * @brief Wrapper claass arround OpenGL textures
 * 
 */
class Texture
{
public:
    const char* path;

    Texture(const char* path, string type);
    // Bind the texture to one of the buffers
    void bind(int buffer);
    // Unbine the texture from the given buffer
    void unBind(int buffer);

    // Returns the type of the texture
    string getType() { return this->type; }
private:
    int width, height, nrChannels;
    GLuint id;
    string type;
};