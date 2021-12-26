#pragma once

#include <GL/glew.h>

#include <map>
#include <vector>

namespace oe
{
    class Shader
    {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        
        void bind() { glUseProgram(id); };
        void unbind() { glUseProgram(0); };

        void setUniform3f(const char* uniformName, float v0, float v1, float v2);
        void setUniform4f(const char* uniformName, float v0, float v1, float v2, float v3);
    private:
        GLuint id;
        std::map<const char*, int> uniforms;
        std::vector<GLuint> shaderIds;

        bool compileShader(const char* path, GLuint type);
        std::string readFromFile(const char* path);
        int getUniformLocation(const char* uniformName);
    };
}