#ifndef BVKBUFFER_H
#define BVKBUFFER_H

// STD
#include <cassert>
#include <cstring>

// BasedVK
#include "BVKDevice.h"

class BVKBuffer
{
public:

    BVKBuffer(BVKDevice &device, VkDeviceSize size, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
    ~BVKBuffer();

    BVKBuffer(const BVKBuffer &) = delete;
    BVKBuffer &operator=(const BVKBuffer &) = delete;

    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();
};


#endif // BVKBUFFER_H
