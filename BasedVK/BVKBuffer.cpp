#include "BVKBuffer.h"

BVKBuffer::BVKBuffer(BVKDevice& device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment) : bufferDevice(device), instanceCount(instanceCount), instanceSize(instanceSize), usageFlags(usageFlags), memoryPropertyFlags(memoryPropertyFlags)
{
    alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
    bufferSize = instanceSize * instanceCount;
    device.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}

BVKBuffer::~BVKBuffer()
{
    unmap();
    vkDestroyBuffer(bufferDevice.getDevice(), buffer, nullptr);
    vkFreeMemory(bufferDevice.getDevice(), memory, nullptr);
}

VkResult BVKBuffer::map(const VkDeviceSize size, const VkDeviceSize offset)
{
    assert(buffer && memory && "Called map on buffer before it was created!");
    return vkMapMemory(bufferDevice.getDevice(), memory, offset, size, 0, &mapped);
}

void BVKBuffer::unmap()
{
    if (mapped)
    {
        vkUnmapMemory(bufferDevice.getDevice(), memory);
        mapped = nullptr;
    }
}

void BVKBuffer::writeToBuffer(const void* data, const VkDeviceSize size, const VkDeviceSize offset) const
{
    assert(mapped && "Cannot copy into an unmapped buffer!");

    if (size == VK_WHOLE_SIZE)
    {
        memcpy(mapped, data, bufferSize);
    }
    else
    {
        auto memOffset = static_cast<char*>(mapped);
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

VkResult BVKBuffer::flush(const VkDeviceSize size, const VkDeviceSize offset) const
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;

    return vkFlushMappedMemoryRanges(bufferDevice.getDevice(), 1, &mappedRange);
}

VkDescriptorBufferInfo BVKBuffer::descriptorInfo(const VkDeviceSize size, const VkDeviceSize offset) const
{
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = offset;
    bufferInfo.range = size;
    return bufferInfo;
}

VkResult BVKBuffer::invalidate(const VkDeviceSize size, const VkDeviceSize offset) const
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;

    return vkInvalidateMappedMemoryRanges(bufferDevice.getDevice(), 1, &mappedRange);
}

void BVKBuffer::writeToIndex(const void* data, const int index) const
{
    writeToBuffer(data, instanceSize, index * alignmentSize);
}

VkResult BVKBuffer::flushIndex(const int index) const
{
    return flush(alignmentSize, index * alignmentSize);
}

VkDescriptorBufferInfo BVKBuffer::descriptorInfoForIndex(const int index) const
{
    return descriptorInfo(alignmentSize, index * alignmentSize);
}

VkResult BVKBuffer::invalidateIndex(const int index) const
{
    return invalidate(alignmentSize, index * alignmentSize);
}

VkDeviceSize BVKBuffer::getAlignment(const VkDeviceSize instanceSize, const VkDeviceSize minOffsetAlignment)
{
    if (minOffsetAlignment > 0)
    {
        return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    }

    return instanceSize;
}
