#ifndef BVKDESCRIPTORS_H
#define BVKDESCRIPTORS_H

// STD
#include <memory>
#include <unordered_map>
#include <vector>
#include <cassert>

// BasedVK
#include "BVKDevice.h"

class BVKDescriptorSetLayout
{
public:

    class Builder
    {
    public:
        explicit Builder(BVKDevice& device) : builderDevice(device) {};

        Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);

        [[nodiscard]] std::unique_ptr<BVKDescriptorSetLayout> build() const;

    private:

        BVKDevice& builderDevice;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings {};
    };

    BVKDescriptorSetLayout(BVKDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~BVKDescriptorSetLayout();

    BVKDescriptorSetLayout(const BVKDescriptorSetLayout&) = delete;
    BVKDescriptorSetLayout& operator=(const BVKDescriptorSetLayout&) = delete;

    [[nodiscard]] VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; };

private:

    BVKDevice& desSetDevice;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings {};

    friend class BVKDescriptorWriter;
};

class BVKDescriptorPool
{
public:

    class Builder
    {
    public:
        explicit Builder(BVKDevice& device) : builderDevice(device) {}

        Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder& setMaxSets(uint32_t count);

        [[nodiscard]] std::unique_ptr<BVKDescriptorPool> build() const;

    private:

        BVKDevice& builderDevice;
        std::vector<VkDescriptorPoolSize> poolSizes {};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    BVKDescriptorPool(BVKDevice& device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
    ~BVKDescriptorPool();

    BVKDescriptorPool(const BVKDescriptorPool&) = delete;
    BVKDescriptorPool& operator=(const BVKDescriptorPool&) = delete;

    bool allocateDescriptor(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

    void freeDescriptors(const std::vector<VkDescriptorSet>& descriptors) const;

    void resetPool() const;

private:

    BVKDevice& desPoolDevice;
    VkDescriptorPool descriptorPool;

    friend class BVKDescriptorWriter;
};

class BVKDescriptorWriter
{
public:

    BVKDescriptorWriter(BVKDescriptorSetLayout& setLayout, BVKDescriptorPool& pool);

    BVKDescriptorWriter& writeBuffer(uint32_t binding, const VkDescriptorBufferInfo* bufferInfo);
    BVKDescriptorWriter& writeImage(uint32_t binding, const VkDescriptorImageInfo* imageInfo);

    bool build(VkDescriptorSet& set);
    void overwrite(VkDescriptorSet& set);

private:

    BVKDescriptorSetLayout& setLayout;
    BVKDescriptorPool& pool;
    std::vector<VkWriteDescriptorSet> writes;
};

#endif // BVKDESCRIPTORS_H
