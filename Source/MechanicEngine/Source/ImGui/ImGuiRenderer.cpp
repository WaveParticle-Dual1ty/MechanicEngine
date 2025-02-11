#include "ImGuiRenderer.h"
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/backends/imgui_impl_vulkan.h"
#include "MechanicEngine/Source/Render/Vulkan/VulkanRHICommandBuffer.h"
#include "ImGuiLog.h"

namespace ME
{
ImGuiRenderer::ImGuiRenderer(Ref<RHI> rhi)
    : m_RHI(rhi)
{
}

bool ImGuiRenderer::Init(uint32_t w, uint32_t h, ERHIPixelFormat swapchainFormat)
{
    m_Format = swapchainFormat;

    bool ret = CreateRenderPass();
    if (!ret)
    {
        IMGUI_LOG_ERROR("CreateRenderPass fail");
        return false;
    }

    ret = ValidTargetColorTextureAndFramebuffer(w, h);
    if (!ret)
    {
        IMGUI_LOG_ERROR("ValidTargetColorTexture fail, w = {}, h = {}", w, h);
        return false;
    }

    return true;
}

bool ImGuiRenderer::Resize(uint32_t w, uint32_t h)
{
    bool ret = ValidTargetColorTextureAndFramebuffer(w, h);
    if (!ret)
    {
        IMGUI_LOG_ERROR("ValidTargetColorTextureAndFramebuffer fail");
        return false;
    }

    return true;
}

void ImGuiRenderer::Draw(Ref<RHICommandBuffer> cmdBuffer)
{
    uint32_t w = m_FrameBuffer->GetWidth();
    uint32_t h = m_FrameBuffer->GetHeight();

    RHIRenderPassBeginInfo renderPassBeginIhfo;
    renderPassBeginIhfo.RenderArea = {0, 0, w, h};
    renderPassBeginIhfo.ColorClearValue = {0.1f, 0.2f, 0.3f, 1.f};
    renderPassBeginIhfo.RenderPass = m_RenderPass;
    renderPassBeginIhfo.Framebuffer = m_FrameBuffer;

    Ref<VulkanRHICommandBuffer> vulkanBuffer = std::dynamic_pointer_cast<VulkanRHICommandBuffer>(cmdBuffer);

    m_RHI->CmdBeginRenderPass(cmdBuffer, renderPassBeginIhfo);

    // to do
    ImDrawData* main_draw_data = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(main_draw_data, vulkanBuffer->CommandBuffer);

    m_RHI->CmdEndRenderPass(cmdBuffer);
}

Ref<RHIRenderPass> ImGuiRenderer::GetRenderPass()
{
    return m_RenderPass;
}

Ref<RHITexture2D> ImGuiRenderer::GetTargetColorTexture()
{
    return m_TargetColorTexture;
}

bool ImGuiRenderer::ValidTargetColorTextureAndFramebuffer(uint32_t w, uint32_t h)
{
    if (m_TargetColorTexture)
    {
        m_RHI->DestroyRHITexture2D(m_TargetColorTexture);
        m_TargetColorTexture.reset();
    }

    if (m_FrameBuffer)
    {
        m_RHI->DestroyRHIFramebuffer(m_FrameBuffer);
        m_FrameBuffer.reset();
    }

    m_TargetTextures.resize(0);

    RHITexture2DCreateDesc texCreateDesc;
    texCreateDesc.PixelFormat = ERHIPixelFormat::PF_R8G8B8A8_UNORM;
    texCreateDesc.Width = w;
    texCreateDesc.Height = h;
    texCreateDesc.NumMips = 1;
    texCreateDesc.NumSamples = 1;
    texCreateDesc.Usage = RHI_TEXTURE_USAGE_COLOR_ATTACHMENT_BIT | RHI_TEXTURE_USAGE_TRANSFER_SRC_BIT |
                          RHI_TEXTURE_USAGE_TRANSFER_DST_BIT | RHI_TEXTURE_USAGE_SAMPLED_BIT;

    m_TargetColorTexture = m_RHI->CreateRHITexture2D(texCreateDesc);
    if (!m_TargetColorTexture)
    {
        IMGUI_LOG_ERROR("RHI::CreateRHITexture2D fail");
        return false;
    }

    m_TargetTextures.push_back(m_TargetColorTexture);
    m_FrameBuffer = m_RHI->CreateRHIFramebuffer(w, h, m_RenderPass, m_TargetTextures);
    if (!m_FrameBuffer)
    {
        IMGUI_LOG_ERROR("RHI::CreateRHIFramebuffer fail");
        return false;
    }

    return true;
}

bool ImGuiRenderer::CreateRenderPass()
{
    RHIRenderPassCreateDesc renderPassDesc = {
        {m_Format, ERHITextureUsage::ColorAttachment}
    };

    m_RenderPass = m_RHI->CreateRHIRenderPass(renderPassDesc);
    if (!m_RenderPass)
    {
        IMGUI_LOG_ERROR("RHI::CreateRHIRenderPass fail");
        return false;
    }

    return true;
}

}  //namespace ME
