#include "BVKDescriptors.h"

BVKDescriptorSetLayout::Builder& BVKDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
    assert(bindings.count(binding) == 0 && "Binding already in use.");

    VkDescriptorSetLayoutBinding layoutBinding {};

    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;

    return *this;
}

std::unique_ptr<BVKDescriptorSetLayout> BVKDescriptorSetLayout::Builder::build() const
{
    return std::make_unique<BVKDescriptorSetLayout>(builderDevice, bindings);
}

BVKDescriptorPool::Builder& BVKDescriptorPool::Builder::addPoolSize(const VkDescriptorType descriptorType, const uint32_t count)
{
    poolSizes.push_back({.type = descriptorType, .descriptorCount = count});
    return *this;
}

BVKDescriptorPool::Builder& BVKDescriptorPool::Builder::setPoolFlags(const VkDescriptorPoolCreateFlags flags)
{
    poolFlags = flags;
    return *this;
}

BVKDescriptorPool::Builder& BVKDescriptorPool::Builder::setMaxSets(const uint32_t count)
{
    maxSets = count;
    return *this;
}

std::unique_ptr<BVKDescriptorPool> BVKDescriptorPool::Builder::build() const
{
    return std::make_unique<BVKDescriptorPool>(builderDevice, maxSets, poolFlags, poolSizes);
}

BVKDescriptorPool::BVKDescriptorPool(BVKDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes) : desPoolDevice(device)
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo {};

    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    if (vkCreateDescriptorPool(desPoolDevice.getDevice(), &descriptorPoolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        PLOGF << "Failed to create descriptor pool.";
        throw std::runtime_error("Failed to create descriptor pool!");
    }
}

BVKDescriptorPool::~BVKDescriptorPool()
{
    vkDestroyDescriptorPool(desPoolDevice.getDevice(), descriptorPool, nullptr);
}

bool BVKDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
{
    VkDescriptorSetAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    if (vkAllocateDescriptorSets(desPoolDevice.getDevice(), &allocInfo, &descriptor) != VK_SUCCESS)
    {
        return false;
    }

    return true;
}

void BVKDescriptorPool::freeDescriptors(const std::vector<VkDescriptorSet>& descriptors) const
{
    vkFreeDescriptorSets(desPoolDevice.getDevice(), descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
}

void BVKDescriptorPool::resetPool() const
{
    vkResetDescriptorPool(desPoolDevice.getDevice(), descriptorPool, 0);
}

BVKDescriptorWriter::BVKDescriptorWriter(BVKDescriptorSetLayout& setLayout, BVKDescriptorPool& pool) : setLayout{setLayout}, pool{pool}
{

}

BVKDescriptorWriter& BVKDescriptorWriter::writeBuffer(const uint32_t binding, const VkDescriptorBufferInfo* bufferInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain the specified binding!");

    const auto& bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info when binding expects multiple!");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);

    return *this;
}

BVKDescriptorWriter& BVKDescriptorWriter::writeImage(const uint32_t binding, const VkDescriptorImageInfo* imageInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding!");

    const auto& bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info when binding expects multiple!");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

bool BVKDescriptorWriter::build(VkDescriptorSet& set)
{
    if (const bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set); !success)
    {
        return false;
    }

    overwrite(set);
    return true;
}

void BVKDescriptorWriter::overwrite(VkDescriptorSet& set)
{
    for (auto& write : writes)
    {
        write.dstSet = set;
    }

    vkUpdateDescriptorSets(pool.desPoolDevice.getDevice(), writes.size(), writes.data(), 0, nullptr);
}
