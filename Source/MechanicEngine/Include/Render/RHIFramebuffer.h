﻿#pragma once
#include <cstdint>
#include <vector>
#include "MechanicEngine/Include/Core/Ptr.h"
#include "RHITexture.h"
#include "RHIRenderPass.h"

namespace ME
{
class RHIFramebuffer
{
public:
    virtual ~RHIFramebuffer() = default;

public:
    Ref<RHITexture2D> GetTargetTexture(uint32_t index);
    const std::vector<Ref<RHITexture2D>>& GetTargetTextures() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

protected:
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    Ref<RHIRenderPass> m_RenderPass;
    std::vector<Ref<RHITexture2D>> m_Textures;
};

}  //namespace ME