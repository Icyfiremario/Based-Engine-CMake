#ifndef BVKWINDOW_H
#define BVKWINDOW_H

// Vulkan
#include <vulkan/vulkan.hpp>

// BasedCore
#include "../BasedCore/Common/BEWindow.h"


class BVKWindow final : public BasedEngine::Common::BEWindow
{
public:

    BVKWindow(int w, int h, std::string title);

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:

    void frameBufferResizedFunc(int width, int height) override;
};


#endif // BVKWINDOW_H