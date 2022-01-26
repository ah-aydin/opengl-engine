#include "Application.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
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
        // Cube
        GLfloat verticies[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

        GLuint indicies[] = {
            0, 1, 3,
            1, 2, 3};

        // VAO to store the VBOs
        GLuint cubeVAO, vbo, ebo;
        {
            glGenVertexArrays(1, &cubeVAO); // create
            glBindVertexArray(cubeVAO);     // bind

            // VBO and EBO
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);                                          // bind
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // copy data to buffer

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0); // give data pointers to shader
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));
            glEnableVertexAttribArray(0); // enable array in shader
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glBindVertexArray(0); // unbind cubeVAO
        }

        GLuint lightVAO;
        {
            glGenVertexArrays(1, &lightVAO);
            glBindVertexArray(lightVAO);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
            glEnableVertexAttribArray(0);
        }

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
        };

        glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
        };

        Shader shader("shader.vert", "shader.frag");
        Shader lightCubeShader("lightShader.vert", "lightShader.frag");

        Texture diffuseMap("textures/container2.png");
        Texture specularMap("textures/container2_specular.png");
        Texture emmisionMap("textures/matrix.jpg");
        diffuseMap.bind(0);
        specularMap.bind(1);
        emmisionMap.bind(2);

        // Camera
        Camera camera(0, 0, 3, 0, -40, 0, 90, 1600.f / 900.f);

        // Lighting
        glm::vec3 lightPos(1.2f, 0, 2.0f);

        shader.setVector3f("material.specular", 0.5f, 0.5f, 0.5f);
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        // shader.setInt("material.emission", 2);
        shader.setFloat("material.shininess", 32);

        shader.setVector3f("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
        shader.setVector3f("directionalLight.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVector3f("directionalLight.specular", 1.0f, 1.0f, 1.0f);

        shader.setVector3f("pointLights[0].position", 0, 0, 0);
        shader.setVector3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVector3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVector3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[0].constant", 1.0f);
        shader.setFloat("pointLights[0].linear", 0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.032f);

        shader.setVector3f("pointLights[1].position", pointLightPositions[0]);
        shader.setVector3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVector3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVector3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[1].constant", 1.0f);
        shader.setFloat("pointLights[1].linear", 0.09f);
        shader.setFloat("pointLights[1].quadratic", 0.032f);

        shader.setVector3f("pointLights[2].position", pointLightPositions[1]);
        shader.setVector3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVector3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVector3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[2].constant", 1.0f);
        shader.setFloat("pointLights[2].linear", 0.09f);
        shader.setFloat("pointLights[2].quadratic", 0.032f);

        shader.setVector3f("pointLights[3].position", pointLightPositions[2]);
        shader.setVector3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVector3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVector3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[3].constant", 1.0f);
        shader.setFloat("pointLights[3].linear", 0.09f);
        shader.setFloat("pointLights[3].quadratic", 0.032f);

        shader.setVector3f("pointLights[4].position", pointLightPositions[3]);
        shader.setVector3f("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
        shader.setVector3f("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVector3f("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
        shader.setFloat("pointLights[4].constant", 1.0f);
        shader.setFloat("pointLights[4].linear", 0.09f);
        shader.setFloat("pointLights[4].quadratic", 0.032f);

        double time = 0;
        while (running)
        {
            PollEvents();
            if (!running)
                break;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (Input::getAction("quit"))
                break;

            // Handle camera input
            float forward = Input::getAxis("forward");
            float right = Input::getAxis("right");
            float up = Input::getAction("jump") ? 1 : 0;
            float down = Input::getAction("crouch") ? 1 : 0;
            float mouseX = (float)Input::mouseX();
            float mouseY = (float)Input::mouseY();

            camera.handleInput(forward, right, up, down, mouseX, mouseY);

            // Move light
            lightPos.x = sin(time) * 2;
            // lightPos.y = sin(time) * 2;
            lightPos.z = cos(time) * 2;

            // Render cube

            glBindVertexArray(cubeVAO);
            shader.bind();
            shader.setMatrix4f("view", camera.getViewMatrix());
            shader.setMatrix4f("projection", camera.getProjectionMatrix());
            shader.setVector3f("viewPos", camera.position);
            shader.setVector3f("pointLights[0].position", lightPos);
            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.setMatrix4f("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Render point light cube that moves
            lightCubeShader.bind();
            lightCubeShader.setMatrix4f("view", camera.getViewMatrix());
            lightCubeShader.setMatrix4f("projection", camera.getProjectionMatrix());

            glBindVertexArray(lightVAO);
            glm::mat4 model = glm::mat4(1);
            model = translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));
            lightCubeShader.setMatrix4f("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Render the other point lights
            for (int i = 0; i < 4; ++i)
            {
                model = glm::mat4(1);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                lightCubeShader.setMatrix4f("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            window.swapWindow();
            Time::tick();
            time += Time::getDeltaTime();
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