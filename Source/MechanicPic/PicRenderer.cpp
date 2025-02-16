#include "PicRenderer.h"
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

    ret = CreateRenderResourece();
    if (!ret)
    {
        MEPIC_LOG_ERROR("CreateRenderResourece fail");
        return false;
    }

    ret = CreateGraphicPass();
    if (!ret)
    {
        MEPIC_LOG_ERROR("CreateGraphicPass fail");
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

    if (!m_ImageTexture)
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
            m_RHI->CmdCopyBufferToImage(cmdBuffer, m_ImageBuffer, m_ImageTexture);

            m_RHI->CmdTransition(
                cmdBuffer, RHITransition(
                               RHI_PIPELINE_STAGE_TRANSFER_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, m_ImageTexture,
                               ERHITextureUsage::TransferDst, ERHITextureUsage::Sampled));

            m_UploadTexture = true;
        }

        m_GraphicPass->BeginPass(cmdBuffer, m_TargetColorTexture, clearColor);

        m_RHI->CmdBindVertexBuffer(cmdBuffer, m_VertexBuffer);
        m_RHI->CmdBindIndexBuffer(cmdBuffer, m_IndexBuffer);
        m_RHI->CmdBindDescriptorSets(cmdBuffer, m_GraphicPass->GetPipeline(), m_DescriptorSets);
        m_RHI->CmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);

        m_GraphicPass->EndPass(cmdBuffer);

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
    // todo, now only support rgba

    m_UploadTexture = false;

    if (m_ImageBuffer)
    {
        m_RHI->DestroyRHIBuffer(m_ImageBuffer);
        m_ImageBuffer.reset();
    }

    if (m_ImageTexture)
    {
        m_RHI->DestroyRHITexture2D(m_ImageTexture);
        m_ImageTexture.reset();
    }

    RHIBufferCreateDesc bufferDesc;
    bufferDesc.Usage = RHI_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferDesc.MemoryProperty = RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    bufferDesc.BufferSize = frame.Datasize[0];
    bufferDesc.Data = frame.Data[0];
    m_ImageBuffer = m_RHI->CreateRHIBuffer(bufferDesc);
    if (!m_ImageBuffer)
    {
        ME_ASSERT(false, "RHI::CreateRHIBuffer fail");
        return;
    }

    RHITexture2DCreateDesc imageTexCreateDesc;
    if (imageInfo.Format == EMPixelFormat::BGRA32)
        imageTexCreateDesc.PixelFormat = ERHIPixelFormat::PF_B8G8R8A8_UNORM;
    else if (imageInfo.Format == EMPixelFormat::BGR24)
        imageTexCreateDesc.PixelFormat = ERHIPixelFormat::PF_B8G8R8_UNORM;
    imageTexCreateDesc.Width = imageInfo.Width;
    imageTexCreateDesc.Height = imageInfo.Height;
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
        RHIWriteDescriptorSet(ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLER, 0, 0, m_Sampler),
        RHIWriteDescriptorSet(ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0, m_ImageTexture)};

    m_RHI->UpdateDescriptorSets(m_DescriptorSet, writeDescSets);
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

bool PicRenderer::CreateRenderResourece()
{
    // shaders
    const std::string resPath = Application::Get().GetResourcePath();
    RHIShaderCreateInfo shaderCreateInfo;
    shaderCreateInfo.Type = ERHIShaderType::Vertex;
    shaderCreateInfo.ShaderFile = resPath + "/Shaders/PicRender.vert";
    shaderCreateInfo.EntryName = "main";
    m_VertexShader = m_RHI->CreateRHIShader(shaderCreateInfo);

    shaderCreateInfo.Type = ERHIShaderType::Pixel;
    shaderCreateInfo.ShaderFile = resPath + "/Shaders/PicRender.frag";
    shaderCreateInfo.EntryName = "main";
    m_PixelShader = m_RHI->CreateRHIShader(shaderCreateInfo);

    // Vertex/Index Buffer
    RHIVertexBufferP2T2 vertexDatas[4] = {
        { {-0.5, 0.5}, {0, 1}},
        {  {0.5, 0.5}, {1, 1}},
        { {0.5, -0.5}, {1, 0}},
        {{-0.5, -0.5}, {0, 0}},
    };

    RHIBufferCreateDesc bufferDesc;
    bufferDesc.Usage = RHI_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferDesc.MemoryProperty = RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    bufferDesc.BufferSize = sizeof(vertexDatas);
    bufferDesc.Data = vertexDatas;
    m_VertexBuffer = m_RHI->CreateRHIBuffer(bufferDesc);
    if (!m_VertexBuffer)
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
    m_IndexBuffer = m_RHI->CreateRHIBuffer(bufferDesc);
    if (!m_IndexBuffer)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHIBuffer fail");
        return false;
    }

    // Descriptor Set
    RHIDescriptorSetCreateInfo descSetCreateInfo = {
        {0,       ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLER, 1, RHI_SHADER_STAGE_FRAGMENT_BIT},
        {1, ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, RHI_SHADER_STAGE_FRAGMENT_BIT}
    };

    m_DescriptorSet = m_RHI->CreateRHIDescriptorSet(descSetCreateInfo);
    if (!m_DescriptorSet)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHIDescriptorSet fail");
        return false;
    }

    m_DescriptorSets = {m_DescriptorSet};

    // Sampler
    RHISamplerCreateInfo samplerInfo;
    m_Sampler = m_RHI->CreateRHISampler(samplerInfo);
    if (!m_Sampler)
    {
        MEPIC_LOG_ERROR("RHI::CreateRHISampler fail");
        return false;
    }

    return true;
}

bool PicRenderer::CreateGraphicPass()
{
    // render pass desc
    RHIRenderPassCreateDesc renderPassDesc = {
        {ERHIPixelFormat::PF_R8G8B8A8_UNORM, ERHITextureUsage::ColorAttachment}
    };

    // Pipeline Stats
    RHIGraphicPipelineStats pipelineStats;
    pipelineStats.ShaderVS = m_VertexShader;
    pipelineStats.ShaderPS = m_PixelShader;
    pipelineStats.VertexInputLayout = {
        {"InPosition", ERHIShaderDataType::Float2, 0},
        {"InTexcoord", ERHIShaderDataType::Float2, 1}
    };
    pipelineStats.InputAssemblyInfo.PrimitiveTopology = RHIPrimitiveTopology::RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineStats.ColorBlendDesc = {
        {true, RHIBlendFactor::SrcAlpha, RHIBlendFactor::OneMinusSrcAlpha, RHIBlendOp::Add, RHIBlendFactor::SrcAlpha,
         RHIBlendFactor::DstAlpha, RHIBlendOp::Add}
    };
    pipelineStats.DescriptorSets = m_DescriptorSets;

    GraphicsPassBuildInfo buildInfo;
    buildInfo.Name = "PicRenderPass";
    buildInfo.RenderPassDesc = renderPassDesc;
    buildInfo.PipelineStats = pipelineStats;

    m_GraphicPass = CreateRef<GraphicsPass>(m_RHI);
    bool ret = m_GraphicPass->BuildGraphicsPass(buildInfo);
    if (!ret)
    {
        MEPIC_LOG_ERROR("GraphicsPass::BuildGraphicsPass fail");
        return false;
    }

    return true;
}

}  //namespace ME
