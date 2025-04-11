#pragma once

// STD
#include <cassert>
#include <cstring>

// BasedVK
#include "BVKDevice.h"

/// @brief Vulkan buffer wrapper class.
class BVKBuffer
{
    public:

        /// @brief Creates a Vulkan buffer
        /// @param device Vulkan device
        /// @param instanceSize Buffer instance size
        /// @param instanceCount Buffer instance count
        /// @param usageFlags Buffer usage flags
        /// @param memoryPropertyFlags Buffer memory flags
        /// @param minOffsetAlignment Buffer alignment
        BVKBuffer(BVKDevice& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment = 1);
        ~BVKBuffer();

        BVKBuffer(const BVKBuffer&) = delete;
        BVKBuffer& operator=(const BVKBuffer&) = delete;

        /// @brief Maps buffer to memory 
        /// @param size Buffer size
        /// @param offset Buffer offset
        /// @return If the buffer successfully mapped
        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        /// @brief Unmaps the buffer from memory
        void unmap();

        /// @brief Writes data to buffer
        /// @param data Data to write
        /// @param size Size of data
        /// @param offset Data offset
        void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        /// @brief Flush the buffer
        /// @param size
        /// @param offset
        /// @return 
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        /// @brief 
        /// @param size 
        /// @param offset 
        /// @return 
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        /// @brief 
        /// @param size 
        /// @param offset 
        /// @return 
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        /// @brief 
        /// @param data 
        /// @param index 
        void writeToIndex(void* data, int index);
        /// @brief 
        /// @param index 
        /// @return 
        VkResult flushIndex(int index);
        /// @brief 
        /// @param index 
        /// @return 
        VkDescriptorBufferInfo descriptorInfoForIndex(int index);
        /// @brief 
        /// @param index 
        /// @return 
        VkResult invalidateIndex(int index);

        /// @brief 
        /// @return 
        VkBuffer getBuffer() const { return buffer; }
        /// @brief 
        /// @return 
        void* getMappedMemory() const { return mapped; }
        /// @brief 
        /// @return 
        uint32_t getInstanceCount() const { return instanceCount; }
        /// @brief 
        /// @return 
        VkDeviceSize getInstanceSize() const { return instanceSize; }
        /// @brief 
        /// @return 
        VkDeviceSize getAlignmentSize() const { return alignmentSize; }
        /// @brief 
        /// @return 
        VkDeviceSize getBufferSize() const { return bufferSize; }
        /// @brief 
        /// @return 
        VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
        /// @brief 
        /// @return 
        VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }

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