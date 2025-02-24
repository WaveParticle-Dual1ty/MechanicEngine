﻿#pragma once
#include "vulkan/vulkan.h"
#include "shaderc/shaderc.hpp"
#include "MechanicEngine/Include/Render/RHIEnums.h"
#include "MechanicEngine/Include/Render/RHIStruct.h"
#include "MechanicEngine/Include/Render/RHIBuffer.h"
#include "MechanicEngine/Include/Render/RHITexture.h"
#include "MechanicEngine/Include/Render/RHIPipeline.h"
#include "MechanicEngine/Include/Render/RHITransition.h"

namespace ME
{
namespace Util
{
VkFormat ConvertERHIPixelFormatToVkFormat(ERHIPixelFormat format);
ERHIPixelFormat ConvertVkFormatToERHIPixelFormat(VkFormat format);

VkImageLayout ConvertERHIImageLayoutToVkImageLayout(ERHIImageLayout flag);
shaderc_shader_kind ConvertRHIShaderTypeToShaderKind(ERHIShaderType type);
VkShaderStageFlagBits ConvertRHIShaderTypeToVkShaderStageFlagBits(ERHIShaderType type);
VkFormat ConvertERHIShaderDataTypeToVkFormat(ERHIShaderDataType type);
VkPrimitiveTopology ConvertRHIPrimitiveTopologyToVkPrimitiveTopology(RHIPrimitiveTopology type);
VkShaderStageFlagBits ConvertERHIShaderStageToVkShaderStageFlagBits(ERHIShaderStage stage);
VkDescriptorType ConvertERHIDescriptorTypeToVkDescriptorType(ERHIDescriptorType type);

VkAccessFlags ConvertRHIAccessFlagsToVkAccessFlags(RHIAccessFlags flag);
VkPipelineStageFlags ConvertRHIPipelineStageFlagsToVkPipelineStageFlags(RHIPipelineStageFlags flag);
VkImageAspectFlags ConvertRHIImageAspectFlagsToVkImageAspectFlags(RHIImageAspectFlags flag);
VkImageUsageFlags ConvertRHITextureUsageFlagsToVkImageUsageFlags(RHITextureUsageFlags flags);
VkMemoryPropertyFlags ConvertRHIMemoryPropertyFlagsToVkMemoryPropertyFlags(RHIMemoryPropertyFlags flags);
VkShaderStageFlags ConvertRHIShaderStageFlagsToVkShaderStageFlags(RHIShaderStageFlags flags);
VkBufferUsageFlags ConvertRHIBufferUsageFlagsToVkBufferUsageFlags(RHIBufferUsageFlags flags);

RHIAccessFlags ConvertERHITextureUsageToRHIAccessFlags(ERHITextureUsage usage);
ERHIImageLayout ConvertERHITextureUsageToERHIImageLayout(ERHITextureUsage usage);

VkBlendFactor ConvertRHIBlendFactorToVkBlendFactor(RHIBlendFactor type);
VkBlendOp ConvertRHIBlendOpToVkBlendOp(RHIBlendOp type);
VkDynamicState ConverRHIDynamicStateToVkDynamicState(RHIDynamicState state);

}  //namespace Util

}  //namespace ME
