#pragma once

#include <SDL2/SDL.h>

#define WINDOW_TITLE "OpenGl Engine"

#ifndef DEBUG
    #define WINDOW_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
    #define WINDOW_SIZE DM.w, DM.h
#else
    #define WINDOW_FLAGS SDL_WINDOW_OPENGL
    #define WINDOW_SIZE 800, 600
#endif

namespace oe
{
    class Window
    {
    public:
        Window() {};
        ~Window();

        bool init();

        void swapWindow();

        int getWidth() { return width; }
        int getHeight() { return height; }

    private:
        SDL_Window *window;
        SDL_GLContext context;

        int width;
        int height;
    };
}