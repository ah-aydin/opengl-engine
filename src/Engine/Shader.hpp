#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <vector>

/**
 * @brief Wrapper class around OpenGL shaders
 * 
 */
class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    
    void bind() { glUseProgram(id); };
    void unbind() { glUseProgram(0); };

    // Uniform setters
    void setInt(const char* uniformName, int v0);
    void setFloat(const char* uniformName, GLfloat v0);
    void setVector3f(const char* uniformName, glm::vec3 vec);
    void setVector3f(const char* uniformName, float v0, float v1, float v2);
    void setVector4f(const char* uniformName, glm::vec4 vec);
    void setVector4f(const char* uniformName, float v0, float v1, float v2, float v3);
    void setMatrix4f(const char* uniformName, glm::mat4 mat);
private:
    char* path;

    GLuint id;
    std::map<const char*, int> uniforms; // Store prev accessed uniform names
    std::vector<GLuint> shaderIds;

    bool compileShader(const char* path, GLuint type);
    std::string readFromFile(const char* path);
    int getUniformLocation(const char* uniformName);

    void cleanUp();
};