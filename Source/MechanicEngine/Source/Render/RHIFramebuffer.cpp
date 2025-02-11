#include "MechanicEngine/Include/Render/RHIFramebuffer.h"
#include "MechanicEngine/Include/Core/Assert.h"

namespace ME
{
Ref<RHITexture2D> RHIFramebuffer::GetTargetTexture(uint32_t index)
{
    ME_ASSERT(index < m_Textures.size(), "Index out of range")

    return m_Textures[index];
}

const std::vector<Ref<RHITexture2D>>& RHIFramebuffer::GetTargetTextures() const
{
    return m_Textures;
}

uint32_t RHIFramebuffer::GetWidth() const
{
    return m_Width;
}

uint32_t RHIFramebuffer::GetHeight() const
{
    return m_Height;
}
}  //namespace ME