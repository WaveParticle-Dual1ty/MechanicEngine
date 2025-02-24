﻿#include "PicRenderer.h"
#include "MechanicEngine/Include/Core/Assert.h"
#include "MechanicEngine/Include/Application/Application.h"
#include "MechanicEngine/Include/Render/RHIStruct.h"
#include "MechanicPicLog.h"

namespace ME
{
PicRenderer::PicRenderer()
{
    m_RHI = Application::Get().GetRHI();
}

bool PicRenderer::Init(uint32_t w, uint32_t h)
{
    bool ret = ValidTargetColorTexture(w, h);
    if (!ret)
    {
        MEPIC_LOG_ERROR("ValidTargetColorTexture fail, w = {}, h = {}", w, h);
        return false;
    }

    ret = CreatePicRenderResourece();
    if (!ret)
    {
        MEPIC_LOG_ERROR("CreatePicRenderResourece fail");
        return false;
    }

    ret = CreatePicRenderGraphicPass();
    if (!ret)
    {
        MEPIC_LOG_ERROR("CreatePicRenderGraphicPass fail");
        return false;
    }

    return true;
}

bool PicRenderer::Resize(uint32_t w, uint32_t h)
{
    bool ret = ValidTargetColorTexture(w, h);
    if (!ret)
    {
        MEPIC_LOG_ERROR("ValidTargetColorTexture fail");
        return false;
    }

    return true;
}

void PicRenderer::Draw(Ref<RHICommandBuffer> cmdBuffer)
{
    Ref<RHITexture2D> target = m_TargetColorTexture;

    RHIColor clearColor = RHIColor(0.1f, 0.1f, 0.1f, 1.f);

    if (!m_ImageBuffer)
    {
        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT, RHI_PIPELINE_STAGE_TRANSFER_BIT, target,
                           ERHITextureUsage::None, ERHITextureUsage::TransferDst));

        m_RHI->CmdClearColor(cmdBuffer, target, clearColor);

        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_TRANSFER_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, target,
                           ERHITextureUsage::TransferDst, ERHITextureUsage::Sampled));
    }
    else
    {
        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT, RHI_PIPELINE_STAGE_TRANSFER_BIT, target,
                           ERHITextureUsage::None, ERHITextureUsage::TransferDst));

        if (!m_UploadTexture)
        {
            if (m_ImageTexture)
            {
                m_RHI->DestroyRHITexture2D(m_ImageTexture);
                m_ImageTexture.reset();
            }

            RHITexture2DCreateDesc imageTexCreateDesc;
            if (m_ImageInfo.Format == EMPixelFormat::BGRA32)
                imageTexCreateDesc.PixelFormat = ERHIPixelFormat::PF_B8G8R8A8_UNORM;
            else if (m_ImageInfo.Format == EMPixelFormat::BGR24)
                imageTexCreateDesc.PixelFormat = ERHIPixelFormat::PF_B8G8R8A8_UNORM;
            imageTexCreateDesc.Width = m_ImageInfo.Width;
            imageTexCreateDesc.Height = m_ImageInfo.Height;
            imageTexCreateDesc.NumMips = 1;
            imageTexCreateDesc.NumSamples = 1;
            imageTexCreateDesc.Usage = RHI_TEXTURE_USAGE_TRANSFER_DST_BIT | RHI_TEXTURE_USAGE_SAMPLED_BIT;
            //imageTexCreateDesc.Usage = RHI_TEXTURE_USAGE_TRANSFER_DST_BIT;
            imageTexCreateDesc.MemoryProperty = 0;
            m_ImageTexture = m_RHI->CreateRHITexture2D(imageTexCreateDesc);
            if (!m_ImageTexture)
            {
                ME_ASSERT(false, "RHI::CreateRHITexture2D fail");
                return;
            }

            std::vector<RHIWriteDescriptorSet> writeDescSets = {
                RHIWriteDescriptorSet(ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLER, 0, 0, m_PicRenderSampler),
                RHIWriteDescriptorSet(ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0, m_ImageTexture)};

            m_RHI->UpdateDescriptorSets(m_PicRenderDescriptorSet, writeDescSets);

            m_RHI->CmdCopyBufferToImage(cmdBuffer, m_ImageBuffer, m_ImageTexture);

            m_RHI->CmdTransition(
                cmdBuffer, RHITransition(
                               RHI_PIPELINE_STAGE_TRANSFER_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, m_ImageTexture,
                               ERHITextureUsage::TransferDst, ERHITextureUsage::Sampled));

            m_UploadTexture = true;
        }

        m_PicRenderGraphicPass->BeginPass(cmdBuffer, m_TargetColorTexture, clearColor);

        ConstantData constantData;
        constantData.ProjectMat = GetProjectMat(m_ImageTexture, m_TargetColorTexture);
        m_RHI->CmdPushConstants(
            cmdBuffer, m_PicRenderGraphicPass->GetPipeline(), ERHIShaderStage::RHI_SHADER_STAGE_VERTEX_BIT, 0,
            sizeof(constantData), &constantData);

        m_RHI->CmdBindVertexBuffer(cmdBuffer, m_PicRenderVertexBuffer);
        m_RHI->CmdBindIndexBuffer(cmdBuffer, m_PicRenderIndexBuffer);
        m_RHI->CmdBindDescriptorSets(cmdBuffer, m_PicRenderGraphicPass->GetPipeline(), m_PicRenderDescriptorSets);
        m_RHI->CmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);

        m_PicRenderGraphicPass->EndPass(cmdBuffer);

        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                           target, ERHITextureUsage::ColorAttachment, ERHITextureUsage::Sampled));
    }
}

void* PicRenderer::GetTargetImTextureID()
{
    return m_TargetImTextureID;
}

void PicRenderer::UpdateImageFrame(const ImageInfo& imageInfo, const ImageFrame& frame)
{
    m_UploadTexture = false;
    m_ImageInfo = imageInfo;

    if (m_ImageBuffer)
    {
        m_RHI->DestroyRHIBuffer(m_ImageBuffer);
        m_ImageBuffer.reset();
    }

    RHIBufferCreateDesc bufferDesc;
    bufferDesc.Usage = RHI_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferDesc.MemoryProperty = RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    bufferDesc.BufferSize = frame.Datasize[0];
    bufferDesc.Data = frame.Data[0];

    std::vector<uint8_t> tmpFrameData;
    if (imageInfo.Format == EMPixelFormat::BGR24)
    {
        uint32_t size = imageInfo.Width * imageInfo.Height * 4;
        tmpFrameData.resize(size);
        uint8_t* srcData = (uint8_t*)frame.Data[0];

        for (uint32_t row = 0; row < imageInfo.Height; ++row)
        {
            for (uint32_t col = 0; col < imageInfo.Width; ++col)
            {
                uint32_t srcIndex = (col + row * imageInfo.Width) * 3;
                uint32_t dstIndex = (col + row * imageInfo.Width) * 4;
                tmpFrameData[dstIndex] = srcData[srcIndex];
                tmpFrameData[dstIndex + 1] = srcData[srcIndex + 1];
                tmpFrameData[dstIndex + 2] = srcData[srcIndex + 2];
                tmpFrameData[dstIndex + 3] = 255;
            }
        }

        bufferDesc.BufferSize = size;
        bufferDesc.Data = tmpFrameData.data();
    }

    m_ImageBuffer = m_RHI->CreateRHIBuffer(bufferDesc);
    if (!m_ImageBuffer)
    {
        ME_ASSERT(false, "RHI::CreateRHIBuffer fail");
        return;
    }
}

bool PicRenderer::ValidTargetColorTexture(uint32_t w, uint32_t h)
{
    if (m_TargetColorTexture)
    {
        m_RHI->DestroyRHITexture2D(m_TargetColorTexture);
        m_TargetColorTexture.reset();
    }

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
        MEPIC_LOG_ERROR("RHI::CreateRHITexture2D fail");
        return false;
    }

    if (m_TargetImTextureID != nullptr)
    {
        m_RHI->DestroyImTextureID(m_TargetImTextureID);
        m_TargetImTextureID = nullptr;
    }

    m_TargetImTextureID = m_RHI->CreateImTextureID(m_TargetColorTexture);
    return true;
}

bool PicRenderer::CreatePicRenderResourece()
{
    // shaders
    const std::string resPath = Application::Get().GetResourcePath();
    RHIShaderCreateInfo shaderCreateInfo;
    shaderCreateInfo.Type = ERHIShaderType::Vertex;
    shaderCreateInfo.ShaderFile = resPath + "/Shaders/PicRender.vert";
    shaderCreateInfo.EntryName = "main";
    m_PicRenderVS = m_RHI->CreateRHIShader(shaderCreateInfo);

    shaderCreateInfo.Type = ERHIShaderType::Pixel;
    shaderCreateInfo.ShaderFile = resPath + "/Shaders/PicRender.frag";
    shaderCreateInfo.EntryName = "main";
    m_PicRenderPS = m_RHI->CreateRHIShader(shaderCreateInfo);

    // Vertex/Index Buffer
    RHIVertexBufferP2T2 vertexDatas[4] = {
        { {-1, 1}, {0, 1}},
        {  {1, 1}, {1, 1}},
        { {1, -1}, {1, 0}},
        {{-1, -1}, {0, 0}},
    };

    RHIBufferCreateDesc bufferDesc;
    bufferDesc.Usage = RHI_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferDesc.MemoryProperty = RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    bufferDesc.BufferSize = sizeof(vertexDatas);
    bufferDesc.Data = vertexDatas;
    m_PicRenderVertexBuffer = m_RHI->CreateRHIBuffer(bufferDesc);
    if (!m_PicRenderVertexBuffer)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHIBuffer fail");
        return false;
    }

    RHIIndexBuffer indexData[2] = {
        {0, 1, 2},
        {0, 2, 3},
    };

    bufferDesc.Usage = RHI_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferDesc.MemoryProperty = RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    bufferDesc.BufferSize = sizeof(indexData);
    bufferDesc.Data = indexData;
    m_PicRenderIndexBuffer = m_RHI->CreateRHIBuffer(bufferDesc);
    if (!m_PicRenderIndexBuffer)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHIBuffer fail");
        return false;
    }

    // Descriptor Set
    RHIDescriptorSetCreateInfo descSetCreateInfo = {
        {0,       ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLER, 1, RHI_SHADER_STAGE_FRAGMENT_BIT},
        {1, ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, RHI_SHADER_STAGE_FRAGMENT_BIT}
    };

    m_PicRenderDescriptorSet = m_RHI->CreateRHIDescriptorSet(descSetCreateInfo);
    if (!m_PicRenderDescriptorSet)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHIDescriptorSet fail");
        return false;
    }

    m_PicRenderDescriptorSets = {m_PicRenderDescriptorSet};

    // Sampler
    RHISamplerCreateInfo samplerInfo;
    m_PicRenderSampler = m_RHI->CreateRHISampler(samplerInfo);
    if (!m_PicRenderSampler)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHISampler fail");
        return false;
    }

    return true;
}

bool PicRenderer::CreatePicRenderGraphicPass()
{
    // render pass desc
    RHIRenderPassCreateDesc renderPassDesc = {
        {ERHIPixelFormat::PF_R8G8B8A8_UNORM, ERHITextureUsage::ColorAttachment}
    };

    // Constant Range
    std::vector<RHIConstantRange> constantRanges;
    RHIConstantRange range;
    range.ShaderStage = ERHIShaderStage::RHI_SHADER_STAGE_VERTEX_BIT;
    range.Size = sizeof(ConstantData);
    constantRanges.push_back(range);

    // Pipeline Stats
    RHIGraphicPipelineStats pipelineStats;
    pipelineStats.ShaderVS = m_PicRenderVS;
    pipelineStats.ShaderPS = m_PicRenderPS;
    pipelineStats.VertexInputLayout = {
        {"InPosition", ERHIShaderDataType::Float2, 0},
        {"InTexcoord", ERHIShaderDataType::Float2, 1}
    };
    pipelineStats.InputAssemblyInfo.PrimitiveTopology = RHIPrimitiveTopology::RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineStats.ColorBlendDesc = {
        {true, RHIBlendFactor::SrcAlpha, RHIBlendFactor::OneMinusSrcAlpha, RHIBlendOp::Add, RHIBlendFactor::SrcAlpha,
         RHIBlendFactor::DstAlpha, RHIBlendOp::Add}
    };
    pipelineStats.ConstantRanges = constantRanges;
    pipelineStats.DescriptorSets = m_PicRenderDescriptorSets;

    GraphicsPassBuildInfo buildInfo;
    buildInfo.Name = "PicRenderPass";
    buildInfo.RenderPassDesc = renderPassDesc;
    buildInfo.PipelineStats = pipelineStats;

    m_PicRenderGraphicPass = CreateRef<GraphicsPass>(m_RHI);
    bool ret = m_PicRenderGraphicPass->BuildGraphicsPass(buildInfo);
    if (!ret)
    {
        MEPIC_LOG_ERROR("GraphicsPass::BuildGraphicsPass fail");
        return false;
    }

    return true;
}

glm::mat4 PicRenderer::GetProjectMat(Ref<RHITexture2D> srcTex, Ref<RHITexture2D> viewportTex)
{
    glm::mat4 res = glm::mat4(1.f);

    uint32_t srcW = srcTex->GetWidth();
    uint32_t srcH = srcTex->GetHeight();
    uint32_t viewportW = viewportTex->GetWidth();
    uint32_t viewportH = viewportTex->GetHeight();

    glm::mat4 vertexPosToSrcPosOnViewport = glm::scale(glm::mat4(1.f), glm::vec3(srcW / 2, srcH / 2, 1.f));
    glm::mat4 viewportToStandard = glm::scale(glm::mat4(1.f), glm::vec3(2.f / viewportW, 2.f / viewportH, 1.f));

    float scaleW = (float)viewportW / srcW;
    float scaleH = (float)viewportH / srcH;
    float scale = (scaleW > scaleH) ? scaleH : scaleW;
    glm::mat4 fitSrcToViewport = glm::scale(glm::mat4(1.f), glm::vec3(scale, scale, 1.f));

    res = viewportToStandard * fitSrcToViewport * vertexPosToSrcPosOnViewport;
    return res;
}

}  //namespace ME
