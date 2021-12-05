#include "Window.hpp"

#include <GL/glew.h>

namespace oe
{
    Window::~Window()
    {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
    }

    bool Window::init()
    {   
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_SIZE, WINDOW_FLAGS);
        if (!window)
        {
            return false;
        }
        context = SDL_GL_CreateContext(window);
        SDL_GetWindowSize(window, &width, &height);
        return true;
    }

    void Window::swapWindow()
    {
        SDL_GL_SwapWindow(window);
    }
}