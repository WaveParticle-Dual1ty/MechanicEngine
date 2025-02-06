#pragma once
#include "vulkan/vulkan.h"
#include "MechanicEngine/Include/Render/RHISampler.h"

namespace ME
{
struct VulkanRHISampler : public RHISampler
{
    VkSampler Sampler = VK_NULL_HANDLE;
};

}  //namespace ME