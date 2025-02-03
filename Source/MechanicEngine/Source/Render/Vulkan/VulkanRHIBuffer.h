#pragma once
#include "vulkan/vulkan.h"
#include "MechanicEngine/Include/Render/RHIBuffer.h"

namespace ME
{
struct VulkanRHIBuffer : RHIBuffer
{
    VkBuffer Buffer = VK_NULL_HANDLE;
    VkDeviceMemory BufferMem = VK_NULL_HANDLE;
};

}  //namespace ME