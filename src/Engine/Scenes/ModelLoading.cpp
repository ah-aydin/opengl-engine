#include "ModelLoading.hpp"

ModelLoading::ModelLoading()
{

}

ModelLoading::~ModelLoading()
{
    delete shader;
    delete model;
}

void ModelLoading::init()
{
    camera = new Camera(0, 0, 0, 0, -40, 0, 90, 1600.f / 900.f);
    shader = new Shader("model_loading.vert", "model_loading.frag");
    model = new Model("backpack.obj");
}

void ModelLoading::update(float dt)
{
    Scene::update(dt);

    shader->setMatrix4f("projection", camera->getProjectionMatrix());
    shader->setMatrix4f("view", camera->getViewMatrix());

    glm::mat4 modelMat = glm::mat4(1.f);
    modelMat = glm::translate(modelMat, glm::vec3(0, 0, 0));
    modelMat = glm::scale(modelMat, glm::vec3(1, 1, 1));
    shader->setMatrix4f("model", modelMat);
    model->Render(*shader);
}

void ModelLoading::handleInput()
{
    // Handle camera input
    float forward = Input::getAxis("forward");
    float right = Input::getAxis("right");
    float up = Input::getAction("jump") ? 1 : 0;
    float down = Input::getAction("crouch") ? 1 : 0;
    float mouseX = (float)Input::mouseX();
    float mouseY = (float)Input::mouseY();

    camera->handleInput(forward, right, up, down, mouseX, mouseY);
}