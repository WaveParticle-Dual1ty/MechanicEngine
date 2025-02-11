#pragma once
#include <cstdint>
#include "MechanicEngine/Include/Core/Ptr.h"
#include "RHIEnums.h"
#include "RHITexture.h"

namespace ME
{
struct RHIMemoryTransition
{
};

struct RHIBufferTransition
{
};

struct RHITextureTransition
{
    Ref<RHITexture> Texture;
    ERHITextureUsage OldUsage = ERHITextureUsage::None;
    ERHITextureUsage NewUsage = ERHITextureUsage::None;
};

struct RHITransition
{
    enum class EType
    {
        Memory,
        Buffer,
        Texture,
    };

    EType Type = EType::Memory;
    RHIPipelineStageFlags SrcStage = 0;
    RHIPipelineStageFlags DstStage = 0;

    RHIMemoryTransition MemoryTransion;
    RHIBufferTransition BufferTransiton;
    RHITextureTransition TextureTransiton;

    RHITransition() = default;

    RHITransition(
        RHIPipelineStageFlags srcStage,
        RHIPipelineStageFlags dstStage,
        Ref<RHITexture> texture,
        ERHITextureUsage oldUsage,
        ERHITextureUsage newUsage)
        : Type(RHITransition::EType::Texture)
        , SrcStage(srcStage)
        , DstStage(dstStage)
        , TextureTransiton({texture, oldUsage, newUsage})
    {
    }
};

}  //namespace ME
