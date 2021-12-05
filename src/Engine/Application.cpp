#include "Application.hpp"

#include <GL/glew.h>

#include "Input.hpp"
#include "../Logging/Log.hpp"

namespace oe
{
    Application::~Application()
    {
        SDL_Quit();
    }

    int Application::run()
    {
        // Initialize
        if (init() == false)
            return EXIT_FAILURE;
        // Run main loop
        mainLoop();

        return EXIT_SUCCESS;
    }

    bool Application::init()
    {
        gl_log_reset(); // reset log file
        if (!sdlInit() || !window.init() || !glInit()) // intialize all components
            return false;
        inputInit(); // create sample input axes/actions
        return true;
    }

    bool Application::sdlInit()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            gl_log_error("Failed to initialize SDL");
            return false;
        }

        // Set OpenGl Attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


        SDL_GL_SetSwapInterval(1);
        return true;
    }

    bool Application::glInit()
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            gl_log_error("Failed to initialize glew");
            return false;
        }

        glViewport(0, 0, window.getWidth(), window.getHeight());

        glClearColor ( 1.0, 0.0, 0.0, 1.0 );

        return true;
    }

    void Application::inputInit()
    {
        // Some inputs for testing
        Input::createAxis("forward", SDLK_w, SDLK_s);
        Input::createAxis("right", SDLK_d, SDLK_a);
        Input::createAction("jump", SDLK_SPACE);
        Input::createAction("crouch", SDLK_LCTRL);
    }

    void Application::mainLoop()
    {
        while (running)
        {
            PollEvents();
            if (!running) break;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // do stuff here

            window.swapWindow();
        }
    }

    void Application::PollEvents()
    {
        Input::resetMouse();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                running = false;
            }
            if (event.type == SDL_KEYDOWN)          Input::keyDown(event.key.keysym.sym);
            if (event.type == SDL_KEYUP)            Input::keyUp(event.key.keysym.sym);
            if (event.type == SDL_MOUSEMOTION)      Input::mouseMotion(event.motion.xrel, event.motion.yrel);
        }
    }
}