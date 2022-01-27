#pragma once

#include <vector>

#include "Camera.hpp"
#include "Input.hpp"

namespace oe
{
    class Scene
    {
    protected:
        Camera* camera;
    public:
        Scene() { };
        virtual void init() { };
        virtual void update(float dt) { };
        virtual ~Scene() { };
    };
}