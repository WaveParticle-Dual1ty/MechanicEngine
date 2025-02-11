#pragma once
#include <vector>
#include <initializer_list>
#include <cstdint>
#include "RHIEnums.h"

namespace ME
{
struct RHIRenderPassCreateDesc
{
    ERHIPixelFormat PixelFormat = ERHIPixelFormat::PF_Unknown;
};

struct RHIAttachment
{
    ERHIPixelFormat Format = ERHIPixelFormat::PF_Unknown;
    ERHITextureUsage TextureUsage = ERHITextureUsage::None;

    RHIAttachment() = default;

    RHIAttachment(ERHIPixelFormat format, ERHITextureUsage textureUsage)
        : Format(format)
        , TextureUsage(textureUsage)
    {
    }
};

// Only support one subpass
struct RHIRenderPassCreateDesc2
{
    std::vector<RHIAttachment> Attachments;

    RHIRenderPassCreateDesc2() = default;

    RHIRenderPassCreateDesc2(const std::initializer_list<RHIAttachment>& attachments)
        : Attachments(attachments)
    {
    }
};

struct RHIRenderPass
{
    virtual ~RHIRenderPass() = default;
};

}  //namespace ME