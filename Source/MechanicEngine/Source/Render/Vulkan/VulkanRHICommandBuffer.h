#pragma once
#include "vulkan/vulkan.h"
#include "MechanicEngine/Include/Render/RHICommandBuffer.h"

namespace ME
{
struct VulkanRHICommandBuffer : RHICommandBuffer
{
    VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;
};

}  //namespace ME