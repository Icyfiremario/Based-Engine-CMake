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

    BVKBuffer(BVKDevice &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
    ~BVKBuffer();

    BVKBuffer(const BVKBuffer &) = delete;
    BVKBuffer &operator=(const BVKBuffer &) = delete;

    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();

    void writeToBuffer(const void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
    [[nodiscard]] VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
    [[nodiscard]] VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
    [[nodiscard]] VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

    void writeToIndex(const void* data, int index) const;
    [[nodiscard]] VkResult flushIndex(int index) const;
    [[nodiscard]] VkDescriptorBufferInfo descriptorInfoForIndex(int index) const;
    [[nodiscard]] VkResult invalidateIndex(int index) const;

    [[nodiscard]] VkBuffer getBuffer() const { return buffer; }
    [[nodiscard]] void* getMappedMemory() const { return mapped; }
    [[nodiscard]] uint32_t getInstanceCount() const { return instanceCount; }
    [[nodiscard]] VkDeviceSize getInstanceSize() const { return instanceSize; }
    [[nodiscard]] VkDeviceSize getAlignmentSize() const { return alignmentSize; }
    [[nodiscard]] VkDeviceSize getBufferSize() const { return bufferSize; }
    [[nodiscard]] VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
    [[nodiscard]] VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }

private:

    BVKDevice& bufferDevice;

    void* mapped = nullptr;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkDeviceSize bufferSize;
    uint32_t instanceCount;
    VkDeviceSize instanceSize;
    VkDeviceSize alignmentSize;
    VkBufferUsageFlags usageFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;

    static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);
};


#endif // BVKBUFFER_H
