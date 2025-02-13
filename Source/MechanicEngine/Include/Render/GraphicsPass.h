#pragma once
#include <cstdint>
#include <vector>
#include <mutex>
#include "RHIStruct.h"
#include "RHI.h"
#include "RHITexture.h"
#include "RHIRenderPass.h"
#include "RHIPipeline.h"

namespace ME
{
struct GraphicsPassBuildInfo
{
    std::string Name = "GraphicsPass";
    RHIRenderPassCreateDesc RenderPassDesc;
    RHIGraphicPipelineStats PipelineStats;
};

class GraphicsPass
{
public:
    GraphicsPass(Ref<RHI> rhi);

public:
    bool BuildGraphicsPass(GraphicsPassBuildInfo createInfo);
    void BeginPass(Ref<RHICommandBuffer> cmdBuffer, Ref<RHITexture2D> targetColor, RHIColor clearColor);
    void EndPass(Ref<RHICommandBuffer> cmdBuffer);

    Ref<RHIGraphicPipeline> GetPipeline();

private:
    bool CheckFramebuffer(const std::vector<Ref<RHITexture2D>>& attachments);

private:
    std::string m_Name;
    Ref<RHI> m_RHI;
    Ref<RHIRenderPass> m_RenderPass;
    Ref<RHIGraphicPipeline> m_Pipeline;
    Ref<RHIFramebuffer> m_Framebuffer;
};

}  //namespace ME
