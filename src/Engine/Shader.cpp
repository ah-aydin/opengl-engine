#include "Shader.hpp"

#include "../Logging/Log.hpp"

#include <string>
#include <fstream>
#include <iostream>

namespace oe
{
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        if (!compileShader(vertexPath, GL_VERTEX_SHADER))       return;
        if (!compileShader(fragmentPath, GL_FRAGMENT_SHADER))   return;

        // Shader program
        id = glCreateProgram();
        // Attach shaders
        for (std::vector<GLuint>::iterator it = shaderIds.begin(); it != shaderIds.end(); ++it)
        {
            glAttachShader(id, *it);
        }
        glLinkProgram(id); // link

        // check for errors
        GLint success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            gl_log_error(infoLog);
        }

        // Cleanup
        for (std::vector<GLuint>::iterator it = shaderIds.begin(); it != shaderIds.end(); ++it)
        {
            glDeleteShader(*it);
        }
    }

    bool Shader::compileShader(const char* path, GLuint type)
    {
        // Create shader
        GLuint shaderId = glCreateShader(type);

        // Get code from file
        std::string source = readFromFile(path);
        const char* cSource = source.c_str();
        glShaderSource(shaderId, 1, &cSource, NULL); // give code
        glCompileShader(shaderId); // compile

        // check for errors
        GLint success;
        char infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            std::string typeString = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            gl_log_error("Error from %s shader", typeString.c_str());
            gl_log_error(infoLog);
            return false;
        }
        
        shaderIds.push_back(shaderId);
        gl_log(cSource);
        return true;
    }

    std::string Shader::readFromFile(const char* path)
    {
        std::ifstream file(path);
        std::string src;
        std::string line;

        while (std::getline(file, line))
        {
            src += line + "\n";
        }
        return src;
    }

    int Shader::getUniformLocation(const char* uniformName)
    {
        std::map<const char*, int>::iterator it = uniforms.find(uniformName);
        if (it != uniforms.end())
        {
            return it->second;
        }
        else
        {
            GLint location = glGetUniformLocation(id, uniformName);
            if (location == -1)
            {
                gl_log_error("Uniform error: '%s' is not a valid uniform name", uniformName);
                return -1;
            }
            // If found, put it in the map
            uniforms.insert(std::pair<const char*, int>(uniformName, location));
            return location;
        }
    }

    void Shader::setUniform3f(const char* uniformName, float v0, float v1, float v2)
    {
        GLint location = getUniformLocation(uniformName);
        
        if (location == -1) return;
        bind();
        glUniform3f(location, v0, v1, v2);
        unbind();
    }

    void Shader::setUniform4f(const char* uniformName, float v0, float v1, float v2, float v3)
    {
        GLint location = getUniformLocation(uniformName);
        
        if (location == -1) return;
        bind();
        glUniform4f(location, v0, v1, v2, v3);
        unbind();
    }
}