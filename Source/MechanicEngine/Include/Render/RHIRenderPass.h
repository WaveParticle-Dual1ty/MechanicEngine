#pragma once
#include "RHIPixelFormat.h"

namespace ME
{
struct RHIRenderPassCreateDesc
{
    ERHIPixelFormat PixelFormat = ERHIPixelFormat::PF_Unknown;
};

struct RHIRenderPass
{
    virtual ~RHIRenderPass() = default;
};

}  //namespace ME