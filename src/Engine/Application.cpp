#include "Application.hpp"

#include <GL/glew.h>

#include "Input.hpp"
#include "Time.hpp"
#include "../Logging/Log.hpp"
#include "../External/stb_image.h"

#include "Scenes/Level.hpp"

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

        quit();

        return EXIT_SUCCESS;
    }

    bool Application::init()
    {
        gl_log_reset();                                // reset log file
        if (!sdlInit() || !window.init() || !glInit()) // intialize all components
            return false;

        inputInit(); // create sample input axes/actions

        // stb_image settings
        stbi_set_flip_vertically_on_load(true);
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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_ShowCursor(SDL_DISABLE);        // Hide cursor
        SDL_SetRelativeMouseMode(SDL_TRUE); // FPS style mouse movement

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

        glEnable(GL_DEPTH_TEST);

        glViewport(0, 0, window.getWidth(), window.getHeight());

        /*
         * Texture
         */
        // Settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // If chose clamp to border as an option, need to specify a color
        GLfloat borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
        glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // wireframe mode
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        return true;
    }

    void Application::quit()
    {
        SDL_Quit();
    }

    void Application::inputInit()
    {
        // Some inputs for testing
        Input::createAxis("forward", SDLK_w, SDLK_s);
        Input::createAxis("right", SDLK_d, SDLK_a);
        Input::createAction("jump", SDLK_SPACE);
        Input::createAction("crouch", SDLK_LCTRL);
        Input::createAction("quit", SDLK_q);
    }

    void Application::mainLoop()
    {
        Level* level = new Level();
        level->init();

        while (running)
        {
            PollEvents();
            if (!running)
                break;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (Input::getAction("quit"))
                break;
            
            level->update(Time::getDeltaTime());

            window.swapWindow();
            Time::tick();
        }

        delete level;
    }

    void Application::PollEvents()
    {
        Input::resetMouse();
        while (SDL_PollEvent(&event))
        {
            // Quit events
            if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                running = false;
            }
            // Input events
            if (event.type == SDL_KEYDOWN)
                Input::keyDown(event.key.keysym.sym);
            if (event.type == SDL_KEYUP)
                Input::keyUp(event.key.keysym.sym);
            if (event.type == SDL_MOUSEMOTION)
                Input::mouseMotion(event.motion.xrel, event.motion.yrel);
        }
    }
}