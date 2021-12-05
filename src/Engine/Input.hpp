#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>

namespace oe
{
    struct Mouse
    {
        Sint32 x, y;
    };

    struct InputAction
    {
        SDL_Keycode keyCode;
    };

    struct InputAxis
    {
        SDL_Keycode positiveKey;
        SDL_Keycode negativeKey;
    };

    class Input
    {
    private:
        static bool keys[500];
        static int convertKeycode(SDL_Keycode);
        static bool getKeyPress(SDL_Keycode);

        static Mouse mouse;
        static std::map<std::string, InputAction> actions;
        static std::map<std::string, InputAxis> axes;

    public:
        static void keyDown(SDL_Keycode code);
        static void keyUp(SDL_Keycode code);
        static void mouseMotion(Sint32 x, Sint32 y);
        static void resetMouse();

        static void createAction(const std::string& name, SDL_Keycode keyCode);
        static void createAxis(const std::string& name, SDL_Keycode positiveKey, SDL_Keycode negativeKey);

        static bool getAction(const std::string& action);
        static float getAxis(const std::string& axis);

        static Sint32 mouseX();
        static Sint32 mouseY();
    };
}