#pragma once
#include "MechanicEngine/Include/Render/RHIResources.h"
#include "vulkan/vulkan.h"

namespace ME
{
struct VulkanRHICommandBuffer : RHICommandBuffer
{
    VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;
};

struct VulkanRHISampler : public RHISampler
{
    VkSampler Sampler = VK_NULL_HANDLE;
};
}  //namespace ME