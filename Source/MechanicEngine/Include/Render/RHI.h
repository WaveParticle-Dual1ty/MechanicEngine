﻿#pragma once
#include <memory>
#include "MechanicEngine/Include/Core/Ptr.h"
#include "MechanicEngine/Include/Window/Window.h"
#include "RHIStruct.h"
#include "RHIResources.h"
#include "RHITexture.h"
#include "RHIFramebuffer.h"
#include "RHIBuffer.h"
#include "RHIDescriptorSet.h"
#include "RHIPipeline.h"
#include "RHIShaders.h"
#include "RHIRenderPass.h"
#include "RHICommandBuffer.h"

namespace ME
{
class RHI
{
public:
    enum class API
    {
        Vulkan,
    };

public:
    virtual ~RHI() = default;
    static std::shared_ptr<RHI> CreateRHI(API api);

public:
    virtual bool Initialize(std::shared_ptr<Window> wnd) = 0;
    virtual bool Resize(uint32_t width, uint32_t height) = 0;
    virtual bool PrepareForNextFrame() = 0;
    virtual bool SubmmitRenderCommands() = 0;
    virtual bool Present() = 0;

    virtual Ref<RHICommandBuffer> GetCurrentCommandBuffer() const = 0;
    virtual Ref<RHITexture2D> GetCurrentBackTexture() = 0;
    virtual RHISwapchainInfo GetSwapchainInfo() = 0;

    // ImGui
    virtual void* CreateImTextureID(Ref<RHITexture2D> texture) = 0;
    virtual void DestroyImTextureID(void* imTextureID) = 0;

    // Resources
    virtual Ref<RHIBuffer> CreateRHIBuffer(RHIBufferCreateDesc createDesc) = 0;
    virtual Ref<RHITexture2D> CreateRHITexture2D(RHITexture2DCreateDesc desc) = 0;
    virtual Ref<RHIRenderPass> CreateRHIRenderPass(RHIRenderPassCreateDesc desc) = 0;
    virtual Ref<RHIFramebuffer> CreateRHIFramebuffer(
        uint32_t width,
        uint32_t height,
        Ref<RHIRenderPass> renderPass,
        std::vector<Ref<RHITexture2D>>& textures) = 0;
    virtual Ref<RHIShader> CreateRHIShader(RHIShaderCreateInfo createInfo) = 0;
    virtual Ref<RHIDescriptorSet> CreateRHIDescriptorSet(RHIDescriptorSetCreateInfo createInfo) = 0;
    virtual Ref<RHISampler> CreateRHISampler(RHISamplerCreateInfo createInfo) = 0;
    virtual Ref<RHIGraphicPipeline> CreateGraphicPipeline(RHIGraphicPipelineCreateInfo createInfo) = 0;

    virtual void UpdateDescriptorSets(
        Ref<RHIDescriptorSet> RelateDescriptorSet,
        const std::vector<RHIWriteDescriptorSet>& writesDescSets) = 0;

    virtual void DestroyRHITexture2D(Ref<RHITexture2D> texture) = 0;
    virtual void DestroyRHIRenderPass(Ref<RHIRenderPass> renderPass) = 0;
    virtual void DestroyRHIFramebuffer(Ref<RHIFramebuffer> framebuffer) = 0;

    // Command
    virtual bool BeginCommandBuffer(Ref<RHICommandBuffer> cmdBuffer) = 0;
    virtual bool EndCommandBuffer(Ref<RHICommandBuffer> cmdBuffer) = 0;
    virtual void CmdPushEvent(Ref<RHICommandBuffer> cmdBuffer, const char* name, RHIColor color) = 0;
    virtual void CmdPopEvent(Ref<RHICommandBuffer> cmdBuffer) = 0;
    virtual void CmdBeginRenderPass(Ref<RHICommandBuffer> cmdBuffer, RHIRenderPassBeginInfo beginIhfo) = 0;
    virtual void CmdEndRenderPass(Ref<RHICommandBuffer> cmdBuffer) = 0;
    virtual void CmdClearColor(Ref<RHICommandBuffer> cmdBuffer, Ref<RHITexture2D> texture, RHIColor color) = 0;
    virtual void CmdPipelineBarrier(Ref<RHICommandBuffer> cmdBuffer, RHIPipelineBarrierInfo barrierInfo) = 0;
    virtual void CmdCopyTexture(Ref<RHICommandBuffer> cmdBuffer, Ref<RHITexture2D> src, Ref<RHITexture2D> dst) = 0;
    virtual void CmdBindGraphicPipeline(Ref<RHICommandBuffer> cmdBuffer, Ref<RHIGraphicPipeline> pipeline) = 0;
    virtual void CmdSetViewport(Ref<RHICommandBuffer> cmdBuffer, RHIViewport viewport) = 0;
    virtual void CmdSetScissor(Ref<RHICommandBuffer> cmdBuffer, RHIScissor scissor) = 0;
    virtual void CmdBindVertexBuffer(Ref<RHICommandBuffer> cmdBuffer, Ref<RHIBuffer> buffer) = 0;
    virtual void CmdBindIndexBuffer(Ref<RHICommandBuffer> cmdBuffer, Ref<RHIBuffer> buffer) = 0;
    virtual void CmdBindDescriptorSets(Ref<RHICommandBuffer> cmdBuffer, Ref<RHIGraphicPipeline> pipeline) = 0;
    virtual void CmdBindDescriptorSets(
        Ref<RHICommandBuffer> cmdBuffer,
        Ref<RHIGraphicPipeline> pipeline,
        const std::vector<Ref<RHIDescriptorSet>> descriptorSets) = 0;
    virtual void
    CmdCopyBufferToImage(Ref<RHICommandBuffer> cmdBuffer, Ref<RHIBuffer> buffer, Ref<RHITexture2D> texture) = 0;
    virtual void CmdPushConstants(
        Ref<RHICommandBuffer> cmdBuffer,
        Ref<RHIGraphicPipeline> pipeline,
        ERHIShaderStage shaderStage,
        uint32_t offset,
        uint32_t size,
        const void* data) = 0;
    virtual void CmdDrawIndexed(
        Ref<RHICommandBuffer> cmdBuffer,
        uint32_t indexCount,
        uint32_t instanceCount,
        uint32_t firstIndex,
        int32_t vertexOffset,
        uint32_t firstInstance) = 0;
};
}  //namespace ME
