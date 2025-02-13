#pragma once
#include <array>
#include "MechanicEngine/Include/Render/GraphicsPass.h"

class TestRenderer
{
public:
    TestRenderer(ME::Ref<ME::RHI> rhi);

public:
    bool Init(uint32_t w, uint32_t h);
    bool Resize(uint32_t w, uint32_t h);
    void Draw(ME::Ref<ME::RHICommandBuffer> cmdBuffer);
    void* GetTargetImTextureID();
    void SetParamRHIColorBlendState(ME::RHIColorBlendState state);

private:
    bool ValidTargetColorTexture(uint32_t w, uint32_t h);
    bool CreateRenderResourece();
    bool CreateGraphicPass();

private:
    struct Vertex
    {
        std::array<float, 2> Position = {0, 0};
        std::array<float, 2> Texcoord = {0, 0};
    };

    struct Index
    {
        uint32_t Index1 = 0;
        uint32_t Index2 = 0;
        uint32_t Index3 = 0;
    };

    void* m_TargetImTextureID = nullptr;

    ME::Ref<ME::RHI> m_RHI;
    ME::Ref<ME::RHITexture2D> m_TargetColorTexture;
    ME::Ref<ME::GraphicsPass> m_GraphicPass;

    ME::Ref<ME::RHIShader> m_VertexShader;
    ME::Ref<ME::RHIShader> m_PixelShader;
    ME::Ref<ME::RHIBuffer> m_VertexBuffer;
    ME::Ref<ME::RHIBuffer> m_IndexBuffer;
    ME::Ref<ME::RHIDescriptorSet> m_DescriptorSet;
    std::vector<ME::Ref<ME::RHIDescriptorSet>> m_DescriptorSets;
    ME::Ref<ME::RHISampler> m_Sampler;
    ME::Ref<ME::RHIBuffer> m_ImageBuffer;
    ME::Ref<ME::RHITexture2D> m_Texture;

    bool m_UploadTexture = false;

    ME::RHIColorBlendState m_ColorBlendState;
};
