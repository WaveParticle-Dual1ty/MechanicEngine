﻿#pragma once
#include "vulkan/vulkan.h"
#include "MechanicEngine/Include/Render/RHITexture.h"

namespace ME
{
class VulkanRHITexture2D : public RHITexture2D
{
public:
private:
    VkFormat m_VKFormat = VK_FORMAT_UNDEFINED;
    VkImage m_Image = VK_NULL_HANDLE;
    VkMemoryRequirements m_MemRequirements = {};
    VkDeviceMemory m_DeviceMem = VK_NULL_HANDLE;
    VkImageView m_ImageView = VK_NULL_HANDLE;
    VkImageAspectFlags m_AspectFlags = 0;

private:
    friend class VulkanRHI;
};

}  //namespace ME