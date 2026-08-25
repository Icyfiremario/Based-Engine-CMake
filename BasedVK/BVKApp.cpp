#include "BVKApp.h"

BVKApp::BVKApp()
{
    appWindow = std::make_unique<BVKWindow>(800, 600, "Based Vulkan");
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);

    resetDeviceRenderer();

    globalPool = BVKDescriptorPool::Builder(*deviceManager->getDevicePtr()).setMaxSets(BVKSwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BVKSwapChain::MAX_FRAMES_IN_FLIGHT).build();
}

BVKApp::BVKApp(int width, int height, const char* title)
{
    appWindow = std::make_unique<BVKWindow>(width, height, title);
    deviceManager = std::make_unique<BVKDeviceManager>(*appWindow);

    resetDeviceRenderer();

    globalPool = BVKDescriptorPool::Builder(*deviceManager->getDevicePtr()).setMaxSets(BVKSwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BVKSwapChain::MAX_FRAMES_IN_FLIGHT).build();
}

BVKApp::~BVKApp()
{
    globalPool.reset();

    appRenderer.reset();
    deviceManager.reset();
    appWindow.reset();
}

void BVKApp::run()
{
    static int deviceIndex = 0;
    const int maxDeviceIndex = static_cast<int>(deviceManager->getDeviceList()->size());

	static bool keyFPressed = false;

    std::shared_ptr<BVKModel> cubeModel = BVKModel::createModelFromFile(*deviceManager->getDevicePtr(), "3D_Models/cube.wobj");
    auto cube = BVKObject::createGameObject();
    cube.model = cubeModel;
    cube.transform.translation = { 0.f, -0.1f, 0.f };
    cube.transform.scale = { .5f, .5f, .5f };
    appObjects.emplace(cube.getId(), std::move(cube));

    std::vector<std::unique_ptr<BVKBuffer>> uboBuffers(BVKSwapChain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < uboBuffers.size(); i++)
    {
        uboBuffers[i] = std::make_unique<BVKBuffer>(*deviceManager->getDevicePtr(), sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    auto globalSetLayout = BVKDescriptorSetLayout::Builder(*deviceManager->getDevicePtr()).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build();

    std::vector<VkDescriptorSet> globalDescriptorSets(BVKSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < globalDescriptorSets.size(); i++)
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        BVKDescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    const BVKRenderSystem renderSystem{ *deviceManager->getDevicePtr(), appRenderer->getRenderPass(), globalSetLayout->getDescriptorSetLayout() };

    BECamera camera{};
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(1.f, 1.f, 2.5f));

    auto viewerObject = BVKObject::createGameObject();
    viewerObject.transform.translation.z = -3.f;
    viewerObject.transform.translation.y = -1.5f;
    viewerObject.transform.rotation.x = -.5f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!appWindow->shouldClose())
    {
        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            PLOGI << "Escape key pressed. Closing app...";
            glfwSetWindowShouldClose(appWindow->getWindow(), true);
        }

        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_F) == GLFW_PRESS && !keyFPressed)
        {
            PLOGI << "Attempting to switch GPUs.";

            vkDeviceWaitIdle(deviceManager->getDevicePtr()->getDevice());

            for (auto& buffer : uboBuffers)
            {
                buffer->flush();
            }

            uboBuffers.clear();
            globalSetLayout.reset();
            viewerObject.~BVKObject();
            renderSystem.~BVKRenderSystem();

            appObjects.clear();
            globalPool.reset();

            deviceIndex++;

            if (deviceIndex >= maxDeviceIndex)
            {
                deviceIndex = 0;
            }

            if (!deviceManager->switchDevice(deviceIndex))
            {
                PLOGF << "Failed to switch app device.";
                throw std::runtime_error("Failed to switch app device!");
            }
            
            resetDeviceRenderer();

            PLOGI << "Switched device to device: " << deviceIndex;
			keyFPressed = true;
        }

        if (glfwGetKey(appWindow->getWindow(), GLFW_KEY_F) == GLFW_RELEASE && keyFPressed)
        {
            keyFPressed = false;
        }

        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        frameTime = fmin(frameTime, 10);

        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = appRenderer->getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        for (auto& kv : appObjects)
        {
            auto& object = kv.second;

            object.transform.rotation.y += 0.01f;
        }

        if (const auto commandBuffer = appRenderer->beginFrame())
        {
            const int frameIndex = appRenderer->getFrameIndex();
            FrameInfo frameInfo{ .frameIndex = frameIndex, .frameTime = frameTime, .commandBuffer = commandBuffer, .globalDescriptorSet = globalDescriptorSets[frameIndex], .appObjects = appObjects};

            GlobalUBO ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();

            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            appRenderer->beginSwapChainRenderPass(commandBuffer);

            renderSystem.renderGameObjects(frameInfo);

            appRenderer->endSwapChainRenderPass(commandBuffer);
            appRenderer->endFrame();
        }

        vkDeviceWaitIdle(deviceManager->getDevicePtr()->getDevice());
    }
}

void BVKApp::resetDeviceRenderer()
{
    appRenderer.reset();
    appRenderer = nullptr;
    appRenderer = std::make_unique<BVKRenderer>(*appWindow, *deviceManager->getDevicePtr());
}
