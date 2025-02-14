﻿#include "MechanicEngine/Include/Media/MediaEnumToStr.h"
#include "MechanicEngine/Include/Core/Assert.h"

namespace ME
{
namespace Utils
{
std::string EMPixelFormatToStr(EMPixelFormat format)
{
    switch (format)
    {
        case EMPixelFormat::None:
            return "None";
        case EMPixelFormat::BGR24:
            return "BGR24";
        case EMPixelFormat::BGRA32:
            return "BGRA32";
        case EMPixelFormat::YUVJ420P:
            return "YUVJ420P";
        default:
            ME_ASSERT(false, "EMPixelFormatToStr: not support EMPixelFormat now");
            return "None";
    }
}
}  //namespace Utils

}  //namespace ME