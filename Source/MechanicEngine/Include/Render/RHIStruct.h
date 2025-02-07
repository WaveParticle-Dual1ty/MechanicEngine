#pragma once
#include <cstdint>
#include <string>
#include "RHIResources.h"
#include "RHIEnums.h"
#include "RHIBuffer.h"
#include "RHITexture.h"
#include "RHIFramebuffer.h"

namespace ME
{
enum class ERHIImageLayout
{
    RHI_IMAGE_LAYOUT_UNDEFINED,
    RHI_IMAGE_LAYOUT_GENERAL,
    RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    RHI_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    RHI_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    RHI_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    RHI_IMAGE_LAYOUT_PREINITIALIZED,
    RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
    RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
    RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
    RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
    RHI_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
    RHI_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
    RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
    RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
    RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    RHI_IMAGE_LAYOUT_SHARED_PRESENT_KHR,
    RHI_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT,
    RHI_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV,
    RHI_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
    RHI_IMAGE_LAYOUT_MAX_ENUM
};

struct RHIExtend2D
{
    uint32_t Width = 0;
    uint32_t Height = 0;
};

struct RHIRenderArea
{
    int32_t OffsetX = 0;
    int32_t OffsetY = 0;
    uint32_t Width = 0;
    uint32_t Height = 0;
};

struct RHIColor
{
    float R = 0;
    float G = 0;
    float B = 0;
    float A = 0;

    RHIColor() = default;

    RHIColor(float r, float g, float b, float a)
        : R(r)
        , G(g)
        , B(b)
        , A(a)
    {
    }
};

struct RHIColorClearValue
{
    float R = 0;
    float G = 0;
    float B = 0;
    float A = 0;
};

struct RHISwapchainInfo
{
    RHIExtend2D Extend;
    ERHIPixelFormat PixelFormat = ERHIPixelFormat::PF_Unknown;
};

struct RHIRenderPassBeginInfo
{
    RHIRenderArea RenderArea;
    RHIColorClearValue ColorClearValue;
    Ref<RHIRenderPass> RenderPass;
    Ref<RHIFramebuffer> Framebuffer;
};

struct RHIPipelineBarrierInfo
{
    enum class EType
    {
        Memory,
        Buffer,
        Texture,
    };

    EType Type = EType::Memory;

    Ref<RHIBuffer> Buffer;
    Ref<RHITexture> Texture;

    RHIAccessFlags SrcAcessFlag = RHI_ACCESS_NONE;
    RHIAccessFlags DstAcessFlag = RHI_ACCESS_NONE;
    ERHIImageLayout OldLayout = ERHIImageLayout::RHI_IMAGE_LAYOUT_UNDEFINED;
    ERHIImageLayout NewLayout = ERHIImageLayout::RHI_IMAGE_LAYOUT_UNDEFINED;
    RHIPipelineStageFlags SrcPipelineStage = RHI_PIPELINE_STAGE_NONE;
    RHIPipelineStageFlags DstPipelineStage = RHI_PIPELINE_STAGE_NONE;

    uint32_t SrcQueueFamilyIndex = 0;
    uint32_t DstQueueFamilyIndex = 0;

    RHIImageAspectFlags AspectMask = RHI_IMAGE_ASPECT_NONE;
    uint32_t BaseMipLevel = 0;
    uint32_t LevelCount = 0;
    uint32_t BaseArrayLayer = 0;
    uint32_t LayerCount = 0;

    RHIPipelineBarrierInfo(
        Ref<RHITexture> texture,
        RHIAccessFlags srcAcessFlag,
        RHIAccessFlags dstAcessFlag,
        ERHIImageLayout oldLayout,
        ERHIImageLayout newLayout,
        RHIPipelineStageFlags srcPipelineStage,
        RHIPipelineStageFlags dstPipelineStage,
        RHIImageAspectFlags AspectMask)
        : Type(EType::Texture)
        , Texture(texture)
        , SrcAcessFlag(srcAcessFlag)
        , DstAcessFlag(dstAcessFlag)
        , OldLayout(oldLayout)
        , NewLayout(newLayout)
        , SrcPipelineStage(srcPipelineStage)
        , DstPipelineStage(dstPipelineStage)
        , SrcQueueFamilyIndex(0)
        , DstQueueFamilyIndex(0)
        , AspectMask(AspectMask)
        , BaseMipLevel(0)
        , LevelCount(1)
        , BaseArrayLayer(0)
        , LayerCount(1)
    {
    }
};

}  //namespace ME
