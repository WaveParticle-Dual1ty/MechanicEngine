#pragma once
#include "vulkan/vulkan.h"
#include "shaderc/shaderc.hpp"
#include "MechanicEngine/Include/Render/RHIEnums.h"
#include "MechanicEngine/Include/Render/RHIPixelFormat.h"
#include "MechanicEngine/Include/Render/RHIStruct.h"
#include "MechanicEngine/Include/Render/RHIResources.h"
#include "MechanicEngine/Include/Render/RHIBuffer.h"
#include "MechanicEngine/Include/Render/RHITexture.h"
#include "MechanicEngine/Include/Render/RHIPipeline.h"

namespace ME
{
namespace Util
{
VkFormat ConvertERHIPixelFormatToVkFormat(ERHIPixelFormat format);
ERHIPixelFormat ConvertVkFormatToERHIPixelFormat(VkFormat format);

VkAccessFlags ConvertERHIAccessFlagToVkAccessFlags(ERHIAccessFlag flag);
VkImageAspectFlags ConvertERHIImageAspectFlagToVkImageAspectFlags(ERHIImageAspectFlag flag);
VkImageLayout ConvertERHIImageLayoutToVkImageLayout(ERHIImageLayout flag);
shaderc_shader_kind ConvertRHIShaderTypeToShaderKind(ERHIShaderType type);
VkShaderStageFlagBits ConvertRHIShaderTypeToVkShaderStageFlagBits(ERHIShaderType type);
VkFormat ConvertERHIShaderDataTypeToVkFormat(ERHIShaderDataType type);
VkPrimitiveTopology ConvertRHIPrimitiveTopologyToVkPrimitiveTopology(RHIPrimitiveTopology type);
VkShaderStageFlagBits ConvertERHIShaderStageToVkShaderStageFlagBits(ERHIShaderStage stage);
VkDescriptorType ConvertERHIDescriptorTypeToVkDescriptorType(ERHIDescriptorType type);

VkPipelineStageFlags ConvertRHIPipelineStageFlagsToVkPipelineStageFlags(RHIPipelineStageFlags flag);
VkImageUsageFlags ConvertERHITextureUsageFlagsToVkImageUsageFlags(RHITextureUsageFlags flags);
VkMemoryPropertyFlags ConvertRHIMemoryPropertyFlagsToVkMemoryPropertyFlags(RHIMemoryPropertyFlags flags);
VkShaderStageFlags ConvertRHIShaderStageFlagsToVkShaderStageFlags(RHIShaderStageFlags flags);
VkBufferUsageFlags ConvertRHIBufferUsageFlagsToVkBufferUsageFlags(RHIBufferUsageFlags flags);

}  //namespace Util

}  //namespace ME
