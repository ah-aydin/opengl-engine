#define STB_IMAGE_IMPLEMENTATION

#include "Application.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Time.hpp"
#include "../Logging/Log.hpp"
#include "../External/stb_image.h"

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

            SDL_ShowCursor(SDL_DISABLE); // Hide cursor
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
            // Settigns
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
            // Cube
            GLfloat verticies[] = {
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };

        GLuint indicies[] = {
            0, 1, 3,
            1, 2, 3
        };

        // VAO to store the VBOs
        GLuint vao;
        glGenVertexArrays(1, &vao); // create
        glBindVertexArray(vao);     // bind

        // VBO and EBO
        GLuint vbo, ebo; // create
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);                                          // bind
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // copy data to buffer

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)0); // give data pointers to shader
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));
        glEnableVertexAttribArray(0); // enable array in shader
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // unbind vao

        Shader shader("shader.vert", "shader.frag");
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);

        // Load texture using stb_image
        int width, height, nrChannels;
        unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0); // TODO error checking

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);                                                    // bind
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // generate
        glGenerateMipmap(GL_TEXTURE_2D);                                                          // generate mipmaps

        stbi_image_free(data); // free memory

        // Generate texture 2
        data = stbi_load("textures/halo.jpg", &width, &height, &nrChannels, 0);
        GLuint texture2;
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        glm::vec3 cubePositions[] = 
        {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };

        // Camera
        Camera camera(0, 0, 3, 0, -40 ,0, 90, 1600.f / 900.f);
        
        while (running)
        {
            PollEvents();
            if (!running) break;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (Input::getAction("quit")) break;

            // Handle camera input
            float forward = Input::getAxis("forward");
            float right = Input::getAxis("right");
            float up = Input::getAction("jump") ? 1 : 0;
            float down = Input::getAction("crouch") ? 1 : 0;
            float mouseX = (float)Input::mouseX();
            float mouseY = (float)Input::mouseY();
            
            camera.handleInput(forward, right, up, down, mouseX, mouseY);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            shader.bind();
            shader.setMatrix4f("view", camera.getViewMatrix());
            shader.setMatrix4f("projection", camera.getProjectionMatrix());

            glBindVertexArray(vao);
            for(unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i; 
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.setMatrix4f("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            window.swapWindow();
            Time::tick();
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
            if (event.type == SDL_KEYDOWN)
                Input::keyDown(event.key.keysym.sym);
            if (event.type == SDL_KEYUP)
                Input::keyUp(event.key.keysym.sym);
            if (event.type == SDL_MOUSEMOTION)
                Input::mouseMotion(event.motion.xrel, event.motion.yrel);
        }
    }
}