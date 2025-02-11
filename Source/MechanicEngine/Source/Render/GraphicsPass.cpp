#include "MechanicEngine/Include/Render/GraphicsPass.h"
#include "MechanicEngine/Include/Core/Assert.h"
#include "MechanicEngine/Include/Application/Application.h"
#include "RenderLog.h"

namespace ME
{
GraphicsPass::GraphicsPass(Ref<RHI> rhi)
    : m_RHI(rhi)
{
}

bool GraphicsPass::BuildGraphicsPass(GraphicsPassBuildInfo createInfo)
{
    m_Name = createInfo.Name;

    // Create render pass
    m_RenderPass = m_RHI->CreateRHIRenderPass2(createInfo.RenderPassDesc);
    if (!m_RenderPass)
    {
        RENDER_LOG_ERROR("RHI::CreateRHIRenderPass2 fail");
        return false;
    }

    // Create pipeline
    GraphicsPassPipelineStats pipelineStats = createInfo.PipelineStats;

    std::vector<Ref<RHIShader>> shaders;
    if (pipelineStats.ShaderVS)
        shaders.push_back(pipelineStats.ShaderVS);
    if (pipelineStats.ShaderPS)
        shaders.push_back(pipelineStats.ShaderPS);
    ME_ASSERT(shaders.size() != 0, "Empty shaders");

    RHIGraphicPipelineCreateInfo graphicPipelineCreateInfo;
    graphicPipelineCreateInfo.Shaders = shaders;
    graphicPipelineCreateInfo.VertexInputLayout = pipelineStats.VertexInputLayout;
    graphicPipelineCreateInfo.InputAssemblyInfo = pipelineStats.InputAssemblyInfo;
    graphicPipelineCreateInfo.RenderPass = m_RenderPass;
    graphicPipelineCreateInfo.ConstantRanges = pipelineStats.ConstantRanges;
    graphicPipelineCreateInfo.DescriptorSets = pipelineStats.DescriptorSets;

    m_Pipeline = m_RHI->CreateRHIGraphicPipeline(graphicPipelineCreateInfo);
    if (!m_Pipeline)
    {
        RENDER_LOG_ERROR("RHI::CreateRHIGraphicPipeline fail");
        return false;
    }

    return true;
}

void GraphicsPass::BeginPass(Ref<RHICommandBuffer> cmdBuffer, Ref<RHITexture2D> targetColor)
{
    std::vector<Ref<RHITexture2D>> attachments = {targetColor};
    bool ret = CheckFramebuffer(attachments);
    if (!ret)
    {
        if (m_Framebuffer)
        {
            m_RHI->DestroyRHIFramebuffer(m_Framebuffer);
            m_Framebuffer.reset();
        }

        Ref<RHITexture2D> texTmp = attachments[0];
        uint32_t w = texTmp->GetWidth();
        uint32_t h = texTmp->GetHeight();

        m_Framebuffer = m_RHI->CreateRHIFramebuffer(w, h, m_RenderPass, attachments);
        if (!m_Framebuffer)
        {
            ME_ASSERT(false, "RHI::CreateRHIFramebuffer fail");
            return;
        }
    }

    m_RHI->CmdPushEvent(cmdBuffer, m_Name.c_str(), RHIColor(1, 0, 0, 1));

    uint32_t w = m_Framebuffer->GetWidth();
    uint32_t h = m_Framebuffer->GetHeight();

    RHIRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.RenderArea = {0, 0, w, h};
    renderPassBeginInfo.ColorClearValue = {0.8f, 0.2f, 0.3f, 1.f};
    renderPassBeginInfo.RenderPass = m_RenderPass;
    renderPassBeginInfo.Framebuffer = m_Framebuffer;
    m_RHI->CmdBeginRenderPass(cmdBuffer, renderPassBeginInfo);

    m_RHI->CmdBindGraphicPipeline(cmdBuffer, m_Pipeline);

    RHIViewport viewport = RHIViewport(0, 0, w, h);
    m_RHI->CmdSetViewport(cmdBuffer, viewport);
    RHIScissor scissor = {0, 0, w, h};
    m_RHI->CmdSetScissor(cmdBuffer, scissor);
}

void GraphicsPass::EndPass(Ref<RHICommandBuffer> cmdBuffer)
{
    m_RHI->CmdEndRenderPass(cmdBuffer);
    m_RHI->CmdPopEvent(cmdBuffer);
}

Ref<RHIGraphicPipeline> GraphicsPass::GetPipeline()
{
    return m_Pipeline;
}

bool GraphicsPass::CheckFramebuffer(const std::vector<Ref<RHITexture2D>>& attachments)
{
    if (!m_Framebuffer)
        return false;

    const std::vector<Ref<RHITexture2D>>& textures = m_Framebuffer->GetTargetTextures();
    if (textures.size() != attachments.size())
        return false;

    for (size_t i = 0; i < attachments.size(); ++i)
    {
        if (attachments[i] != textures[i])
            return false;
    }

    return true;
}

}  //namespace ME
