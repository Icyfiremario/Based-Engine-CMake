#include "BVKDescriptors.h"

BVKDescriptorSetLayout::Builder &BVKDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count)
{
    assert(bindings.count(binding) == 0 && "Binding already in use.");

    VkDescriptorSetLayoutBinding layoutBinding{};

    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;

    return *this;
}

BVKDescriptorPool::Builder &BVKDescriptorPool::Builder::addPoolSize(VkDescriptorType descriptorType, uint32_t count)
{
    poolSizes.push_back({descriptorType, count});
    return *this;
}

BVKDescriptorPool::Builder &BVKDescriptorPool::Builder::setPoolFlags(VkDescriptorPoolCreateFlags flags)
{
    poolFlags = flags;
    return *this;
}

BVKDescriptorPool::Builder &BVKDescriptorPool::Builder::setMaxSets(uint32_t count)
{
    maxSets = count;
    return *this;
}

std::unique_ptr<BVKDescriptorSetLayout> BVKDescriptorSetLayout::Builder::build() const
{
    return std::make_unique<BVKDescriptorSetLayout>(builderDevice, bindings);
}

std::unique_ptr<BVKDescriptorPool> BVKDescriptorPool::Builder::build() const
{
    return std::make_unique<BVKDescriptorPool>(builderDevice, maxSets, poolFlags, poolSizes);
}

BVKDescriptorSetLayout::BVKDescriptorSetLayout(BVKDevice &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) : desSetDevice{device}, bindings{bindings}
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};

    for (auto kv : bindings)
    {
        setLayoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(desSetDevice.getDevice(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout.");
    }
}

BVKDescriptorSetLayout::~BVKDescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(desSetDevice.getDevice(), descriptorSetLayout, nullptr);
}

BVKDescriptorPool::BVKDescriptorPool(BVKDevice &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes) : desPoolDevice{device}
{
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};

    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    if (vkCreateDescriptorPool(desPoolDevice.getDevice(), &descriptorPoolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool.");
    }
}

BVKDescriptorPool::~BVKDescriptorPool()
{
    vkDestroyDescriptorPool(desPoolDevice.getDevice(), descriptorPool, nullptr);
}

bool BVKDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const
{
    VkDescriptorSetAllocateInfo allocInfo{};
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

void BVKDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const
{
    vkFreeDescriptorSets(desPoolDevice.getDevice(), descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
}

void BVKDescriptorPool::resetPool() const
{
    vkResetDescriptorPool(desPoolDevice.getDevice(), descriptorPool, 0);
}

BVKDescriptorWriter::BVKDescriptorWriter(BVKDescriptorSetLayout &setLayout, BVKDescriptorPool &pool) : setLayout{setLayout}, pool{pool}
{

}

BVKDescriptorWriter &BVKDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding!");

    auto& bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info when binding expect multiple!");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

BVKDescriptorWriter &BVKDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo)
{
    assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding!");

    auto& bindingDescription = setLayout.bindings[binding];

    assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info when binding expect multiple!");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

bool BVKDescriptorWriter::build(VkDescriptorSet &set)
{
    bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
    if (!success)
    {
        return false;
    }
    overwrite(set);
    return true;
}

void BVKDescriptorWriter::overwrite(VkDescriptorSet &set)
{
    for (auto& write : writes)
    {
        write.dstSet = set;
    }

    vkUpdateDescriptorSets(pool.desPoolDevice.getDevice(), writes.size(), writes.data(), 0, nullptr);
}
