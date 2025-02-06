#include "MechanicEngine/Include/Render/RHIShaders.h"
#include "vulkan/vulkan.h"

namespace ME
{
struct VulkanRHIShader : RHIShader
{
    VkShaderModule ShaderModule = VK_NULL_HANDLE;
};

}  //namespace ME
