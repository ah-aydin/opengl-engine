#pragma once

#include <vector>

#include "Camera.hpp"
#include "Input.hpp"

/**
 * @brief Class from which all other levels/scenes should be extended from
 * 
 */
class Scene
{
protected:
    // The main camera of the scene
    Camera* camera;
public:
    Scene() { };
    // Call on creation
    virtual void init() { };
    /**
     * @brief Call on every frame for update
     * 
     * @param dt time in miliseconds since last frame
     */
    virtual void update(float dt) { };
    virtual ~Scene() { };
};