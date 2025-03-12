#include "BEapp.h"

BEapp::BEapp(int width, int height, int maxFrameTime, const std::string name, int api) : width(width), height(height), maxFrameTime(maxFrameTime), name(name), renderAPI(api)
{
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
            std::make_unique<BVKDevice>(window);
            std::make_unique<BVKRenderer>(window, *VKDevice.get());
            break;

        case BasedCore::OPENGL:
            break;
    
        default:
            throw std::runtime_error("Invalid render API");
    }
}

BEapp::~BEapp()
{
}

void BEapp::run()
{
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        {
            while (!window.shouldClose())
            {
                if(glfwGetKey(window.getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
                {
                    std::cout << "Switching to OpenGL." << std::endl;
                    window.switchRenderAPI(BasedCore::OPENGL);
                    renderAPI = BasedCore::OPENGL;
                    run();
                }

                glfwPollEvents();
            }
            break;
        }

        case BasedCore::OPENGL:
        {
            BGLShader shader("gl_shaders/gl_shader.vs", "gl_shaders/gl_shader.fs");

            float vertices[] = {
                // positions         // colors
                 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
                 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
            };

            unsigned int VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            while (!window.shouldClose())
            {
                if(glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
                }

                if(glfwGetKey(window.getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
                {
                    std::cout << "Switching to Vulkan." << std::endl;
                    window.switchRenderAPI(BasedCore::VULKAN);
                    renderAPI = BasedCore::VULKAN;
                    run();
                }

                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                shader.use();
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                glfwPollEvents();
                glfwSwapBuffers(window.getWindow());
            }

            break;
        }
    
        default:
            throw std::runtime_error("Invalid render API");
    }
    
}
