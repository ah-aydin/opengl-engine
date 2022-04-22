#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>

#define KEY_ARRAY_SIZE 501

/**
 * @brief Datatype to store mouse movement
 */
struct Mouse
{
    Sint32 x, y;
};

/**
 * @brief 
 */
struct InputAction
{
    SDL_Keycode keyCode;
};

struct InputAxis
{
    SDL_Keycode positiveKey;
    SDL_Keycode negativeKey;
};

/**
 * @brief Class that provides static methods to access the created 
 * input axes, input actions and mouse movements.
 */
class Input
{
private:
    static bool keys[KEY_ARRAY_SIZE];
    static int convertKeycode(SDL_Keycode);
    static bool getKeyPress(SDL_Keycode);

    static Mouse mouse;
    static std::map<std::string, InputAction> actions;
    static std::map<std::string, InputAxis> axes;

public:
    // Event handlers
    
    static void keyDown(SDL_Keycode code);
    static void keyUp(SDL_Keycode code);
    static void mouseMotion(Sint32 x, Sint32 y);
    static void resetMouse();

    // Input creation
    static void createAction(const std::string& name, SDL_Keycode keyCode);
    static void createAxis(const std::string& name, SDL_Keycode positiveKey, SDL_Keycode negativeKey);

    // Input access
    static bool getAction(const std::string& action);
    static float getAxis(const std::string& axis);
    static Sint32 mouseX();
    static Sint32 mouseY();
};