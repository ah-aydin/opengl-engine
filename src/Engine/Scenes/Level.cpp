#include "Level.hpp"

Level::Level()
{
    
}

Level::~Level()
{
    delete shader;
    delete lightCubeShader;
    delete diffuseMap;
    delete specularMap;
    delete emmisionMap;
}

void Level::init()
{
    // Create a camera for the scene
    camera = new Camera(0, 0, 3, 0, -40, 0, 90, 1600.f / 900.f);
    initBuffers();
    initTextures();
    initShaders();
}

void Level::initBuffers()
{
    // Create cube buffers
    glGenVertexArrays(1, &cubeVAO); // create
    glBindVertexArray(cubeVAO);     // bind

    // VBO and EBO
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);                                          // bind
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // copy data to buffer

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0); // give data pointers to shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));
    glEnableVertexAttribArray(0); // enable array in shader
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // unbind cubeVAO

    // Create light cube buffers
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
    glEnableVertexAttribArray(0);
}

void Level::initTextures()
{
    diffuseMap = new Texture("textures/container2.png", "diffuse");
    specularMap = new Texture("textures/container2_specular.png", "specular");
    emmisionMap = new Texture("textures/matrix.jpg", "emmision");
    
    // Bind the textures
    diffuseMap->bind(0);
    specularMap->bind(1);
    emmisionMap->bind(2);
}

void Level::initShaders()
{
    shader = new Shader("shader.vert", "shader.frag");
    lightCubeShader = new Shader("lightShader.vert", "lightShader.frag");

    shader->setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    // shader->setInt("material.emission", 2);
    shader->setFloat("material.shininess", 32);

    shader->setVector3f("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
    shader->setVector3f("directionalLight.diffuse", 0.5f, 0.5f, 0.5f);
    shader->setVector3f("directionalLight.specular", 1.0f, 1.0f, 1.0f);

    shader->setVector3f("pointLights[0].position", pointLightPositions[4]);
    shader->setVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09f);
    shader->setFloat("pointLights[0].quadratic", 0.032f);

    shader->setVector3f("pointLights[1].position", pointLightPositions[0]);
    shader->setVector3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVector3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVector3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09f);
    shader->setFloat("pointLights[1].quadratic", 0.032f);

    shader->setVector3f("pointLights[2].position", pointLightPositions[1]);
    shader->setVector3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVector3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVector3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09f);
    shader->setFloat("pointLights[2].quadratic", 0.032f);

    shader->setVector3f("pointLights[3].position", pointLightPositions[2]);
    shader->setVector3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVector3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVector3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[3].constant", 1.0f);
    shader->setFloat("pointLights[3].linear", 0.09f);
    shader->setFloat("pointLights[3].quadratic", 0.032f);

    shader->setVector3f("pointLights[4].position", pointLightPositions[3]);
    shader->setVector3f("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVector3f("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVector3f("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[4].constant", 1.0f);
    shader->setFloat("pointLights[4].linear", 0.09f);
    shader->setFloat("pointLights[4].quadratic", 0.032f);
}

void Level::update(float dt)
{
    // Handle camera input
    float forward = Input::getAxis("forward");
    float right = Input::getAxis("right");
    float up = Input::getAction("jump") ? 1 : 0;
    float down = Input::getAction("crouch") ? 1 : 0;
    float mouseX = (float)Input::mouseX();
    float mouseY = (float)Input::mouseY();

    camera->handleInput(forward, right, up, down, mouseX, mouseY);

    // Render cube
    glBindVertexArray(cubeVAO);
    shader->bind();
    shader->setMatrix4f("view", camera->getViewMatrix());
    shader->setMatrix4f("projection", camera->getProjectionMatrix());
    shader->setVector3f("viewPos", camera->getPosition());
    for (unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMatrix4f("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Render point light cubes
    lightCubeShader->bind();
    lightCubeShader->setMatrix4f("view", camera->getViewMatrix());
    lightCubeShader->setMatrix4f("projection", camera->getProjectionMatrix());

    glBindVertexArray(lightVAO);
    glm::mat4 model = glm::mat4(1);

    for (int i = 0; i < 5; ++i)
    {
        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader->setMatrix4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}