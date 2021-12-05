#include "Input.hpp"

#include <iostream>

#include "../Logging/Log.hpp"

namespace oe
{
    Mouse Input::mouse = {0, 0};
    bool Input::keys[500] = {0};
    std::map<std::string, InputAction> Input::actions;
    std::map<std::string, InputAxis> Input::axes;

    int padding = SDLK_CAPSLOCK - 128; // since keys above SDLK_CAPSLOCK have high id's

    int Input::convertKeycode(SDL_Keycode code)
    {
        if (code >= SDLK_CAPSLOCK)
            return code - padding;
        else
            return code;
    }

    bool Input::getKeyPress(SDL_Keycode code)
    {
        return Input::keys[Input::convertKeycode(code)];
    }

    void Input::keyDown(SDL_Keycode code)
    {
        keys[Input::convertKeycode(code)] = true;
    }

    void Input::keyUp(SDL_Keycode code)
    {
        keys[Input::convertKeycode(code)] = false;
    }

    void Input::mouseMotion(Sint32 x, Sint32 y)
    {
        Input::mouse.x = x;
        Input::mouse.y = y;
    }

    void Input::resetMouse()
    {
        Input::mouse.x = 0;
        Input::mouse.y = 0;
    }

    void Input::createAction(const std::string &name, SDL_Keycode keyCode)
    {
        Input::actions.insert(
            std::pair<std::string, InputAction>(name, {keyCode}));
    }

    void Input::createAxis(const std::string &name, SDL_Keycode positiveKey, SDL_Keycode negativeKey)
    {
        Input::axes.insert(
            std::pair<std::string, InputAxis>(name, {positiveKey, negativeKey}));
    }

    bool Input::getAction(const std::string &action)
    {
        try
        {
            return Input::getKeyPress(Input::actions.at(action).keyCode);
        }
        catch(const std::exception& e)
        {
            gl_log_error("Input action %s does not exist", action);
            return false;
        }
        return false;
    }

    float Input::getAxis(const std::string &axis)
    {
        try
        {
            auto ax = Input::axes.at(axis);
            float pInput = (float)Input::getKeyPress(ax.positiveKey);
            float bInput = (float)Input::getKeyPress(ax.negativeKey);
            return pInput - bInput;
        }
        catch(const std::exception& e)
        {
            gl_log_error("Input axis %s does not exist", axis);
            return 0.f;
        }
        return 0.f;
    }

    Sint32 Input::mouseX() { return Input::mouse.x; }
    Sint32 Input::mouseY() { return Input::mouse.y; }
}