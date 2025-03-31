#include "BEapp.h"

/// @brief Temp light direction def
struct GlobalUbo
{
    glm::mat4 projectionView{ 1.f };
    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
};

BEapp::BEapp(int width, int height, int maxFrameTime, const std::string name, int api) : appWidth(width), appHeight(height), maxFrameTime(maxFrameTime), renderAPI(api), name(name)
{
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        case BasedCore::OPENGL:
            break;
    
        default:
            throw std::runtime_error("Invalid render API");
    }

    appWindow = std::make_unique<BEwindow>(appWidth, appHeight, name, renderAPI);
}

BEapp::BEapp(BasedCore::Config config, const std::string name, int maxFrameTime) : maxFrameTime(maxFrameTime), name(name)
{
    appWidth = config.width;
    appHeight = config.height;
    renderAPI = config.renderAPI;

    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        case BasedCore::OPENGL:
            break;
    
        default:
            throw std::runtime_error("Invalid render API");
    }

    appWindow = std::make_unique<BEwindow>(appWidth, appHeight, name, renderAPI);
}

BEapp::~BEapp()
{
    
    appRenderer.reset();
    appDevice.reset();
    appWindow.reset();
}

void BEapp::run()
{
    
    appRenderer.reset();
    appDevice.reset();

    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };
    
    switch (renderAPI)
    {
        case BasedCore::VULKAN:
        {
            appDevice = std::make_unique<BVKDevice>(*appWindow.get());
            appRenderer = std::make_unique<BVKRenderer>(*appWindow.get(), *appDevice.get());

            //BVKRenderSystem renderSystem{ *appDevice.get(), appRenderer->getRenderPass(), nullptr };

            auto currentTime = std::chrono::high_resolution_clock::now();
            
            while (!appWindow.get()->shouldClose())
            {
                if(glfwGetKey(appWindow.get()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(appWindow.get()->getWindow(), GLFW_TRUE);
                }

                if(glfwGetKey(appWindow.get()->getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
                {
#ifdef DEBUG
                    std::cout << "Switching to OpenGL." << std::endl;
#endif
                    vkDeviceWaitIdle(appDevice.get()->getDevice());
                    appWindow.get()->switchRenderAPI(BasedCore::OPENGL);
                    renderAPI = BasedCore::OPENGL;
                    run();
                }

                glfwPollEvents();

                auto newTime = std::chrono::high_resolution_clock::now();
                float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
                currentTime = newTime;

                frameTime = fmin(frameTime, maxFrameTime);

                if (auto commandBuffer = appRenderer.get()->beginFrame())
                {
                    //int frameIndex = appRenderer.get()->getFrameIndex();

                    appRenderer.get()->beginSwapchainRenderPass(commandBuffer);
                    appRenderer.get()->endSwapchainRenderPass(commandBuffer);
                    appRenderer.get()->endFrame();
                }
            }

            vkDeviceWaitIdle(appDevice.get()->getDevice());
            break;
        }

        case BasedCore::OPENGL:
        {
            BGLShader shader("gl_shaders/gl_shader.vs", "gl_shaders/gl_shader.fs");

            unsigned int VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            while (!appWindow.get()->shouldClose())
            {
                if(glfwGetKey(appWindow.get()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(appWindow.get()->getWindow(), GLFW_TRUE);
                }

                if(glfwGetKey(appWindow.get()->getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
                {
#ifdef DEBUG
                    std::cout << "Switching to Vulkan." << std::endl;
#endif
                    appWindow.get()->switchRenderAPI(BasedCore::VULKAN);
                    renderAPI = BasedCore::VULKAN;
                    run();
                }

                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                shader.use();
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                glfwPollEvents();
                glfwSwapBuffers(appWindow.get()->getWindow());

                float offset = sin(glfwGetTime()) / 2.0f;
                vertices[12] = offset; // Sets x position of top vertex

                shader.setFloat("u_time", glfwGetTime());

                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            }

            break;
        }
    
        default:
            throw std::runtime_error("Invalid render API");
    }
    
}
