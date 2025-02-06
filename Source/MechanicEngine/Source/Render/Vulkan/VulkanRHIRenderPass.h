#pragma once
#include "vulkan/vulkan.h"
#include "MechanicEngine/Include/Render/RHIRenderPass.h"

namespace ME
{
struct VulkanRHIRenderPass : RHIRenderPass
{
    VkRenderPass RenderPass = VK_NULL_HANDLE;
};

}  //namespace ME