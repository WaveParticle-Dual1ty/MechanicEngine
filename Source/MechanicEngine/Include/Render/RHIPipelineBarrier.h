#pragma once
#include "MechanicEngine/Include/Core/Ptr.h"
#include "RHIBuffer.h"
#include "RHITexture.h"

namespace ME
{
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
