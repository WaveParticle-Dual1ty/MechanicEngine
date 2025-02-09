#pragma once
#include "RHIStruct.h"
#include "RHIEnums.h"

namespace ME
{
struct RHISwapchainInfo
{
    RHIExtend2D Extend;
    ERHIPixelFormat PixelFormat = ERHIPixelFormat::PF_Unknown;
};

}  //namespace ME
