#include "TestRenderer.h"
#include "ThirdParty/stb/stb_image.h"
#include "MechanicEngine/Include/Core/Assert.h"
#include "MechanicEngine/Include/Application/Application.h"
#include "SandboxLog.h"

using namespace ME;

TestRenderer::TestRenderer(Ref<RHI> rhi)
    : m_RHI(rhi)
{
}

bool TestRenderer::Init(uint32_t w, uint32_t h)
{
    bool ret = ValidTargetColorTexture(w, h);
    if (!ret)
    {
        SANDBOX_LOG_ERROR("ValidTargetColorTexture fail, w = {}, h = {}", w, h);
        return false;
    }

    ret = CreateRenderResourece();
    if (!ret)
    {
        SANDBOX_LOG_ERROR("CreateRenderResourece fail");
        return false;
    }

    ret = CreateGraphicPass();
    if (!ret)
    {
        SANDBOX_LOG_ERROR("CreateGraphicPass fail");
        return false;
    }

    return true;
}

bool TestRenderer::Resize(uint32_t w, uint32_t h)
{
    bool ret = ValidTargetColorTexture(w, h);
    if (!ret)
    {
        SANDBOX_LOG_ERROR("ValidTargetColorTexture fail");
        return false;
    }

    return true;
}

void TestRenderer::Draw(Ref<RHICommandBuffer> cmdBuffer)
{
    Ref<RHITexture2D> texture = m_TargetColorTexture;

#if 0
    m_RHI->CmdTransition(
        cmdBuffer, RHITransition(
                       RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT, RHI_PIPELINE_STAGE_TRANSFER_BIT, texture,
                       ERHITextureUsage::None, ERHITextureUsage::TransferDst));

    m_RHI->CmdClearColor(cmdBuffer, texture, RHIColor(0.8f, 0.9f, 0.3f, 1.f));

    m_RHI->CmdTransition(
        cmdBuffer, RHITransition(
                       RHI_PIPELINE_STAGE_TRANSFER_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, texture,
                       ERHITextureUsage::TransferDst, ERHITextureUsage::Sampled));

#else
    m_RHI->CmdTransition(
        cmdBuffer, RHITransition(
                       RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT, RHI_PIPELINE_STAGE_TRANSFER_BIT, texture,
                       ERHITextureUsage::None, ERHITextureUsage::TransferDst));

    m_RHI->CmdClearColor(cmdBuffer, texture, RHIColor(0.5f, 0.2f, 0.7f, 1.f));

    if (!m_UploadTexture)
    {
        m_RHI->CmdCopyBufferToImage(cmdBuffer, m_ImageBuffer, m_Texture);

        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_TRANSFER_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, m_Texture,
                           ERHITextureUsage::TransferDst, ERHITextureUsage::Sampled));

        m_UploadTexture = true;
    }

    m_GraphicPass->BeginPass(cmdBuffer, m_TargetColorTexture);

    m_RHI->CmdBindVertexBuffer(cmdBuffer, m_VertexBuffer);
    m_RHI->CmdBindIndexBuffer(cmdBuffer, m_IndexBuffer);
    m_RHI->CmdBindDescriptorSets(cmdBuffer, m_GraphicPass->GetPipeline(), m_DescriptorSets);
    m_RHI->CmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);

    m_GraphicPass->EndPass(cmdBuffer);

    m_RHI->CmdTransition(
        cmdBuffer, RHITransition(
                       RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, RHI_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, texture,
                       ERHITextureUsage::ColorAttachment, ERHITextureUsage::Sampled));
#endif
}

void* TestRenderer::GetTargetImTextureID()
{
    return m_TargetImTextureID;
}

bool TestRenderer::ValidTargetColorTexture(uint32_t w, uint32_t h)
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
        SANDBOX_LOG_ERROR("RHI::CreateRHITexture2D fail");
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

bool TestRenderer::CreateRenderResourece()
{
    // shaders
    const std::string resPath = Application::Get().GetResourcePath();
    RHIShaderCreateInfo shaderCreateInfo;
    shaderCreateInfo.Type = ERHIShaderType::Vertex;
    shaderCreateInfo.ShaderFile = resPath + "/Shaders/TestRendererShader.vert";
    shaderCreateInfo.EntryName = "main";
    m_VertexShader = m_RHI->CreateRHIShader(shaderCreateInfo);

    shaderCreateInfo.Type = ERHIShaderType::Pixel;
    shaderCreateInfo.ShaderFile = resPath + "/Shaders/TestRendererShader.frag";
    shaderCreateInfo.EntryName = "main";
    m_PixelShader = m_RHI->CreateRHIShader(shaderCreateInfo);

    // Vertex/Index Buffer
    Vertex vertexDatas[4] = {
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
        SANDBOX_LOG_ERROR("RHI::CreateRHIBuffer fail");
        return false;
    }

    Index indexData[2] = {
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
        SANDBOX_LOG_ERROR("RHI::CreateRHIBuffer fail");
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
        SANDBOX_LOG_ERROR("RHI::CreateRHIDescriptorSet fail");
        return false;
    }

    m_DescriptorSets = {m_DescriptorSet};

    // Sampler
    RHISamplerCreateInfo samplerInfo;
    m_Sampler = m_RHI->CreateRHISampler(samplerInfo);
    if (!m_Sampler)
    {
        SANDBOX_LOG_ERROR("RHI::CreateRHISampler fail");
        return false;
    }

    // Load Image
    std::string imagePath = resPath + "/Images/awesomeface.png";

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    ME_ASSERT(data != nullptr, "stbi_load fail");

    RHIBufferCreateDesc imageBufferDesc;
    imageBufferDesc.Usage = RHI_BUFFER_USAGE_TRANSFER_SRC_BIT;
    imageBufferDesc.MemoryProperty = RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    imageBufferDesc.BufferSize = width * height * channels;
    imageBufferDesc.Data = data;
    m_ImageBuffer = m_RHI->CreateRHIBuffer(imageBufferDesc);
    if (!m_ImageBuffer)
    {
        SANDBOX_LOG_ERROR("RHI::CreateRHIBuffer fail");
        return false;
    }

    stbi_image_free(data);
    data = nullptr;

    // Create texture for image
    RHITexture2DCreateDesc imageTexCreateDesc;
    imageTexCreateDesc.PixelFormat = ERHIPixelFormat::PF_R8G8B8A8_UNORM;
    imageTexCreateDesc.Width = width;
    imageTexCreateDesc.Height = height;
    imageTexCreateDesc.NumMips = 1;
    imageTexCreateDesc.NumSamples = 1;
    imageTexCreateDesc.Usage = RHI_TEXTURE_USAGE_TRANSFER_DST_BIT | RHI_TEXTURE_USAGE_SAMPLED_BIT;
    imageTexCreateDesc.MemoryProperty = 0;
    m_Texture = m_RHI->CreateRHITexture2D(imageTexCreateDesc);
    if (!m_Texture)
    {
        SANDBOX_LOG_ERROR("RHI::CreateRHITexture2D fail");
        return false;
    }

    std::vector<RHIWriteDescriptorSet> writeDescSets = {
        RHIWriteDescriptorSet(ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLER, 0, 0, m_Sampler),
        RHIWriteDescriptorSet(ERHIDescriptorType::RHI_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, 0, m_Texture)};

    m_RHI->UpdateDescriptorSets(m_DescriptorSet, writeDescSets);

    return true;
}

bool TestRenderer::CreateGraphicPass()
{
    // render pass desc
    RHIRenderPassCreateDesc renderPassDesc = {
        {ERHIPixelFormat::PF_R8G8B8A8_UNORM, ERHITextureUsage::ColorAttachment}
    };

    // Pipeline Stats
    GraphicsPassPipelineStats pipelineStats;
    pipelineStats.ShaderVS = m_VertexShader;
    pipelineStats.ShaderPS = m_PixelShader;
    pipelineStats.VertexInputLayout = {
        {"InPosition", ERHIShaderDataType::Float2, 0},
        {"InTexcoord", ERHIShaderDataType::Float2, 1}
    };
    pipelineStats.InputAssemblyInfo.PrimitiveTopology = RHIPrimitiveTopology::RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineStats.DescriptorSets = m_DescriptorSets;

    GraphicsPassBuildInfo buildInfo;
    buildInfo.Name = "TestRendererPass";
    buildInfo.RenderPassDesc = renderPassDesc;
    buildInfo.PipelineStats = pipelineStats;

    m_GraphicPass = CreateRef<GraphicsPass>(m_RHI);
    bool ret = m_GraphicPass->BuildGraphicsPass(buildInfo);
    if (!ret)
    {
        SANDBOX_LOG_ERROR("GraphicsPass::BuildGraphicsPass fail");
        return false;
    }

    return true;
}
