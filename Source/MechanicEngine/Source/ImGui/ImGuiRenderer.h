#pragma once
#include <vector>
#include "MechanicEngine/Include/Core/Ptr.h"
#include "MechanicEngine/Include/Render/RHI.h"

namespace ME
{
class ImGuiRenderer
{
public:
    ImGuiRenderer(Ref<RHI> rhi);

public:
    bool Init(uint32_t w, uint32_t h, ERHIPixelFormat swapchainFormat);
    bool Resize(uint32_t w, uint32_t h);
    void Draw(Ref<RHICommandBuffer> cmdBuffer);
    Ref<RHIRenderPass> GetRenderPass();
    Ref<RHITexture2D> GetTargetColorTexture();

private:
    bool ValidTargetColorTextureAndFramebuffer(uint32_t w, uint32_t h);
    bool CreateRenderPass();

private:
    ERHIPixelFormat m_Format = ERHIPixelFormat::PF_Unknown;
    Ref<RHI> m_RHI;
    Ref<RHITexture2D> m_TargetColorTexture;
    std::vector<Ref<RHITexture2D>> m_TargetTextures;
    Ref<RHIFramebuffer> m_FrameBuffer;
    Ref<RHIRenderPass> m_RenderPass;
};

}  //namespace ME
