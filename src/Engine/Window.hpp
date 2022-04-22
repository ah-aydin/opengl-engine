#pragma once

#include <SDL2/SDL.h>

#define WINDOW_TITLE "OpenGl Engine"

#ifndef DEBUG
    // Open the window in full screen mode
    #define WINDOW_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
    #define WINDOW_SIZE DM.w, DM.h
#else
    // Open a window in windowed mode
    #define WINDOW_FLAGS SDL_WINDOW_OPENGL
    #define WINDOW_SIZE 1280, 720
#endif

/**
 * @brief A window class, there should only be one instance of this.
 * 
 */
class Window
{
public:
    Window() {};
    ~Window();

    bool init();

    // Call this on the update functino to switch screen buffers
    void swapWindow();

    // Returns the width of the screen
    int getWidth() { return width; }
    // Returns the height of the screen
    int getHeight() { return height; }

private:
    SDL_Window *window;
    SDL_GLContext context;

    int width;
    int height;
};