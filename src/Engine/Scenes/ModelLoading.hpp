#pragma once

#include "../Rendering/Shader.hpp"
#include "../Scene.hpp"
#include "../Rendering/Model.hpp"

class ModelLoading : public Scene
{
public:
    ModelLoading();
    void init() override;
    void update(float dt) override;
    ~ModelLoading();

protected:
    void handleInput() override;

private:
    Shader* shader;
    Model* model;
};