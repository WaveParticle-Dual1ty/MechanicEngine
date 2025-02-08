#pragma once
#include "RHIPixelFormat.h"
#include "RHIStruct.h"

namespace ME
{
struct RHISwapchainInfo
{
    RHIExtend2D Extend;
    ERHIPixelFormat PixelFormat = ERHIPixelFormat::PF_Unknown;
};

}  //namespace ME
