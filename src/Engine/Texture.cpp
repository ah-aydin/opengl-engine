#define STB_IMAGE_IMPLEMENTATION

#include "Texture.hpp"
#include "../External/stb_image.h"

#include "../Logging/Log.hpp"

namespace oe
{
    Texture::Texture(const char* path)
    {
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

        int transparent = 0;
        if (nrChannels == 4) transparent = 1; 

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + transparent, width, height, 0, GL_RGB + transparent, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

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
}