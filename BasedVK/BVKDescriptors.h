#pragma once

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

                Builder(BVKDevice& device) : builderDevice{device} {}

                Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);

                std::unique_ptr<BVKDescriptorSetLayout> build() const;

            private:

                BVKDevice& builderDevice;
                std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        BVKDescriptorSetLayout(BVKDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~BVKDescriptorSetLayout();

        BVKDescriptorSetLayout(const BVKDescriptorSetLayout&) = delete;
        BVKDescriptorSetLayout& operator=(const BVKDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        
        BVKDevice& desSetDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

        friend class BVKDescriptorWriter;
};

class BVKDescriptorPool
{};

class BVKDescriptorWriter
{};