#include "BVKBuffer.h"

BVKBuffer::BVKBuffer(BVKDevice &device, VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment) : bufferDevice(device), instanceSize(instanceSize), instanceCount(instanceCount), usageFlags(usageFlags), memoryPropertyFlags(memoryPropertyFlags)
{
    alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
    bufferSize = alignmentSize * instanceCount;
    bufferDevice.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, bufferMemory);
}

BVKBuffer::~BVKBuffer()
{
}

VkResult BVKBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    return VkResult();
}

void BVKBuffer::unmap()
{
}

void BVKBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset)
{
}

VkResult BVKBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    return VkResult();
}

VkDescriptorBufferInfo BVKBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
{
    return VkDescriptorBufferInfo();
}

VkResult BVKBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    return VkResult();
}

void BVKBuffer::writeToIndex(void *data, int index)
{
}

VkResult BVKBuffer::flushIndex(int index)
{
    return VkResult();
}

VkDescriptorBufferInfo BVKBuffer::descriptorInfoForIndex(int index)
{
    return VkDescriptorBufferInfo();
}

VkResult BVKBuffer::invalidateIndex(int index)
{
    return VkResult();
}

VkDeviceSize BVKBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
{
    return VkDeviceSize();
}
