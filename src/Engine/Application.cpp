#include "Application.hpp"

#include <GL/glew.h>

#include "Input.hpp"
#include "Shader.hpp"
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

        quit();

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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
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

        glClearColor ( 0.0, 0.0, 0.0, 1.0 );

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
    }

    void Application::mainLoop()
    {
        // Triangle
        GLfloat verticies[] = {
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f// top left 
        };

        GLuint indicies[] = {
            0, 1, 3,
            1, 2, 3
        };

        // VAO to store the VBOs
        GLuint vao;
        glGenVertexArrays(1, &vao); // create
        glBindVertexArray(vao); // bind

        // VBO and EBO
        GLuint vbo, ebo; // create
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // copy data to buffer

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW); 
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0); // give data pointers to shader
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
        glEnableVertexAttribArray(0); // enable array in shader
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0); // unbind vao

        // wireframe mode
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        Shader shader("shader.vert", "shader.frag");

        while (running)
        {
            PollEvents();
            if (!running) break;
            glClear(GL_COLOR_BUFFER_BIT);

            shader.bind();

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
            // do stuff here

            window.swapWindow();
        }
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
            if (event.type == SDL_KEYDOWN)          Input::keyDown(event.key.keysym.sym);
            if (event.type == SDL_KEYUP)            Input::keyUp(event.key.keysym.sym);
            if (event.type == SDL_MOUSEMOTION)      Input::mouseMotion(event.motion.xrel, event.motion.yrel);
        }
    }
}