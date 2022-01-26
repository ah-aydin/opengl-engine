#pragma once

#include <GL/glew.h>

// TODO add texture specific settings
namespace oe
{
    class Texture
    {
    public:
        Texture(const char* path);
        void bind(int buffer);
        void unBind(int buffer);
    private:
        int width, height, nrChannels;
        GLuint id;
    };
}