#include "BVKBuffer.h"

BVKBuffer::BVKBuffer(BVKDevice &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment) : bufferDevice(device), instanceCount(instanceCount), instanceSize(instanceSize), usageFlags(usageFlags), memoryPropertyFlags(memoryPropertyFlags)
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

VkResult BVKBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    assert(buffer && memory && "Called mao on buffer before it was created!");
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

void BVKBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
{
    assert(mapped && "Cannot copy into an unmapped buffer!");

    if (size == VK_WHOLE_SIZE)
    {
        memcpy(mapped, data, bufferSize);
    }
    else
    {
        char* memOffset = (char*)mapped;
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

VkResult BVKBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;

    return vkFlushMappedMemoryRanges(bufferDevice.getDevice(), 1, &mappedRange);
}

VkDescriptorBufferInfo BVKBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
{
    return VkDescriptorBufferInfo{buffer, offset, size};
}

VkResult BVKBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange mappedRange = {};
    mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mappedRange.memory = memory;
    mappedRange.offset = offset;
    mappedRange.size = size;

    return vkInvalidateMappedMemoryRanges(bufferDevice.getDevice(), 1, &mappedRange);
}

void BVKBuffer::writeToIndex(void *data, int index)
{
    writeToBuffer(data, instanceSize, index * alignmentSize);
}

VkResult BVKBuffer::flushIndex(int index)
{
    return flush(alignmentSize, index * alignmentSize);
}

VkDescriptorBufferInfo BVKBuffer::descriptorInfoForIndex(int index)
{
    return descriptorInfo(alignmentSize, index * alignmentSize);
}

VkResult BVKBuffer::invalidateIndex(int index)
{
    return invalidate(alignmentSize, index * alignmentSize);
}

VkDeviceSize BVKBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
{
    if (minOffsetAlignment > 0)
    {
        return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    }
    else
    {
        return instanceSize;
    }
}
