#pragma once
#include "MechanicEngine/Include/Render/RHI.h"
#include "MechanicEngine/Include/Render/GraphicsPass.h"
#include "MechanicEngine/Include/Media/ImageStruct.h"
#include "MechanicEngine/Include/Math/Math.h"

namespace ME
{
class PicRenderer
{
public:
    PicRenderer();

public:
    bool Init(uint32_t w, uint32_t h);
    bool Resize(uint32_t w, uint32_t h);
    void Draw(Ref<RHICommandBuffer> cmdBuffer);
    void* GetTargetImTextureID();
    void UpdateImageFrame(const ImageInfo& imageInfo, const ImageFrame& frame);

private:
    bool ValidTargetColorTexture(uint32_t w, uint32_t h);
    bool CreatePicRenderResourece();
    bool CreatePicRenderGraphicPass();
    glm::mat4 GetProjectMat(Ref<RHITexture2D> srcTex, Ref<RHITexture2D> viewportTex);

private:
    struct ConstantData
    {
        glm::mat4x4 ProjectMat;
    };

    Ref<RHI> m_RHI;
    Ref<RHITexture2D> m_TargetColorTexture;
    void* m_TargetImTextureID = nullptr;

    Ref<RHIShader> m_PicRenderVS;
    Ref<RHIShader> m_PicRenderPS;
    Ref<RHIBuffer> m_PicRenderVertexBuffer;
    Ref<RHIBuffer> m_PicRenderIndexBuffer;
    Ref<RHIDescriptorSet> m_PicRenderDescriptorSet;
    std::vector<Ref<RHIDescriptorSet>> m_PicRenderDescriptorSets;
    Ref<RHISampler> m_PicRenderSampler;
    Ref<GraphicsPass> m_PicRenderGraphicPass;

    bool m_UploadTexture = false;
    ImageInfo m_ImageInfo;
    Ref<RHIBuffer> m_ImageBuffer;
    Ref<RHITexture2D> m_ImageTexture;
};

}  //namespace ME
