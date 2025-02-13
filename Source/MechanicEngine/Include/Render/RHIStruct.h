#pragma once
#include <cstdint>

namespace ME
{
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

struct RHIViewport
{
    float X = 0;
    float Y = 0;
    float Width = 0;
    float Height = 0;
    float MinDepth = 0;
    float MaxDepth = 1;

    RHIViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        : X(static_cast<float>(x))
        , Y(static_cast<float>(y))
        , Width(static_cast<float>(width))
        , Height(static_cast<float>(height))
    {
    }

    RHIViewport(float x, float y, float width, float height)
        : X(x)
        , Y(y)
        , Width(width)
        , Height(height)
    {
    }

    RHIViewport(float x, float y, float width, float height, float minDepth, float maxDepth)
        : X(x)
        , Y(y)
        , Width(width)
        , Height(height)
        , MinDepth(minDepth)
        , MaxDepth(maxDepth)
    {
    }
};

struct RHIScissor
{
    int32_t OffsetX;
    int32_t OffsetY;
    uint32_t Width = 0;
    uint32_t Height = 0;
};

}  //namespace ME
