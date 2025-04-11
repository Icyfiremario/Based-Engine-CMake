#include "BEapp.h"

/// @brief Temp light direction def
struct GlobalUbo
{
    /// @brief projection view
    glm::mat4 projectionView{ 1.f };
    /// @brief Light direction
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
    globalPool.reset();
    appRenderer.reset();
    appDevice.reset();
    appWindow.reset();
}

void BEapp::run()
{
    globalPool.reset();
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
            initVulkanObjects();

            std::vector<std::unique_ptr<BVKBuffer>> uboBuffers(BVKSwapchain::MAX_FRAMES_IN_FLIGHT);

            for (size_t i = 0; i < uboBuffers.size(); i++)
            {
                uboBuffers[i] = std::make_unique<BVKBuffer>(*appDevice.get(), sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
                uboBuffers[i]->map();
            }

            auto globalSetLayout = BVKDescriptorSetLayout::Builder(*appDevice.get()).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL).build();

            std::vector<VkDescriptorSet> globalDescriptorSets(BVKSwapchain::MAX_FRAMES_IN_FLIGHT);
            for (size_t i = 0; i < globalDescriptorSets.size(); i++)
            {
                auto bufferInfo = uboBuffers[i]->descriptorInfo();
                BVKDescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
            }

            BVKRenderSystem renderSystem{ *appDevice.get(), appRenderer->getRenderPass(), globalSetLayout->getDescriptorSetLayout() };

            BECamera camera{};
            camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

            auto viewerObject = BVKObject::createGameObject();

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

                    // Delete all data off the GPU. 
                    uboBuffers.clear();
                    globalSetLayout.reset();
                    viewerObject.~BVKObject();
                    renderSystem.~BVKRenderSystem();

                    destroyVulkanObjects();
                    appWindow.get()->switchRenderAPI(BasedCore::OPENGL);
                    renderAPI = BasedCore::OPENGL;
                    run();
                }

                glfwPollEvents();

                auto newTime = std::chrono::high_resolution_clock::now();
                float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
                currentTime = newTime;

                frameTime = fmin(frameTime, maxFrameTime);

                camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

                float aspect = appRenderer.get()->getAspectRatio();
                camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

                if (auto commandBuffer = appRenderer.get()->beginFrame())
                {
                    int frameIndex = appRenderer.get()->getFrameIndex();
                    FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

                    GlobalUbo ubo{};
                    ubo.projectionView = camera.getProjection() * camera.getView();
                    uboBuffers[frameIndex]->writeToBuffer(&ubo);
                    uboBuffers[frameIndex]->flush();

                    appRenderer.get()->beginSwapchainRenderPass(commandBuffer);
                    renderSystem.renderGameObjects(frameInfo, appVulkanObjects);
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

void BEapp::initVulkanObjects()
{
    appDevice = std::make_unique<BVKDevice>(*appWindow.get());
    appRenderer = std::make_unique<BVKRenderer>(*appWindow.get(), *appDevice.get());
    globalPool = BVKDescriptorPool::Builder(*appDevice.get()).setMaxSets(BVKSwapchain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BVKSwapchain::MAX_FRAMES_IN_FLIGHT).build();

    loadVulkanAppObjects();
}

void BEapp::destroyVulkanObjects()
{
    vkDeviceWaitIdle(appDevice.get()->getDevice()); // Wait for the device to finish all operations before destroying objects
    appVulkanObjects.clear();
    globalPool.reset();
    appRenderer.reset();
    appDevice.reset();
}

void BEapp::loadVulkanAppObjects()
{
    std::shared_ptr<BVKModel> cubeModel = BVKModel::createModelFromFile(*appDevice.get(), "3D_Models/cube.wobj");

    auto cube = BVKObject::createGameObject();
    cube.model = cubeModel;
    cube.transform.translation = { 0.f, 0.f, 2.5f };
    cube.transform.scale = { .5f, .5f, .5f };

    appVulkanObjects.push_back(std::move(cube));
}
