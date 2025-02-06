#pragma once
#include "MechanicEngine/Include/Render/RHIResources.h"
#include "vulkan/vulkan.h"

namespace ME
{
struct VulkanRHISampler : public RHISampler
{
    VkSampler Sampler = VK_NULL_HANDLE;
};
}  //namespace ME