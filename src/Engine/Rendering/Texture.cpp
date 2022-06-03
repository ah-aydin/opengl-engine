#define STB_IMAGE_IMPLEMENTATION

#include <STB/stb_image.h>

#include "Texture.hpp"
#include "../../Logging/Log.hpp"

/**
 * @brief Construct a new Texture:: Texture object
 * 
 * @param path path to the texture file
 * @param type type of the texture
 */
Texture::Texture(const char* path, string type) : path(path), type(type)
{
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        int transparent = 0;
        if (nrChannels == 4) transparent = 1; 

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + transparent, width, height, 0, GL_RGB + transparent, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        gl_log_error("ERROR::TEXTURE Failed to load texture from path: %s", path);
    }

    stbi_image_free(data);
}

/**
 * @brief Bind the buffer to the given index
 * 
 * @param buffer index of the buffer
 */
void Texture::bind(int buffer)
{
    glActiveTexture(GL_TEXTURE0 + buffer);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unBind(int buffer)
{
    glActiveTexture(GL_TEXTURE0 + buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
}