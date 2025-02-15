#pragma once
#include <vector>
#include "RHIStruct.h"
#include "RHIDescriptorSet.h"
#include "RHIShaders.h"
#include "RHIRenderPass.h"

namespace ME
{
struct RHIPipeline
{
};

enum class ERHIShaderDataType
{
    None,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

uint32_t GetRHIShaderDataTypeSize(ERHIShaderDataType type);

struct RHIVertexElement
{
    std::string Name;
    ERHIShaderDataType Type;
    uint32_t Location = 0;

    uint32_t Size = 0;
    uint32_t Offset = 0;

    RHIVertexElement(const std::string& name, ERHIShaderDataType type, uint32_t location)
        : Name(name)
        , Type(type)
        , Location(location)
        , Size(GetRHIShaderDataTypeSize(type))
    {
    }
};

class RHIVertexInputLayout
{
public:
    RHIVertexInputLayout() = default;

    RHIVertexInputLayout(const std::initializer_list<RHIVertexElement>& elements)
        : m_Elements(elements)
    {
        CalculateOffsetAndStride();
    }

public:
    const std::vector<RHIVertexElement>& GetElements();
    uint32_t GetStride() const;

private:
    void CalculateOffsetAndStride();

private:
    std::vector<RHIVertexElement> m_Elements;
    uint32_t m_Stride = 0;
};

enum class RHIPrimitiveTopology
{
    RHI_PRIMITIVE_TOPOLOGY_POINT_LIST,
    RHI_PRIMITIVE_TOPOLOGY_LINE_LIST,
    RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    RHI_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,
    RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,
    RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,
    RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,
    RHI_PRIMITIVE_TOPOLOGY_PATCH_LIST,
    RHI_PRIMITIVE_TOPOLOGY_MAX_ENUM
};

struct RHIInputAssemblyInfo
{
    RHIPrimitiveTopology PrimitiveTopology = RHIPrimitiveTopology::RHI_PRIMITIVE_TOPOLOGY_POINT_LIST;
};

enum class RHIBlendFactor : uint32_t
{
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
};

enum class RHIBlendOp : uint32_t
{
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

struct RHIColorBlendEquation
{
    RHIBlendFactor SrcColorBlendFactor = RHIBlendFactor::Zero;
    RHIBlendFactor DstColorBlendFactor = RHIBlendFactor::Zero;
    RHIBlendOp ColorBlendOp = RHIBlendOp::Add;
    RHIBlendFactor SrcAlphaBlendFactor = RHIBlendFactor::Zero;
    RHIBlendFactor DstAlphaBlendFactor = RHIBlendFactor::Zero;
    RHIBlendOp AlphaBlendOp = RHIBlendOp::Add;

    RHIColorBlendEquation() = default;

    RHIColorBlendEquation(
        RHIBlendFactor srcColorBlendFactor,
        RHIBlendFactor dstColorBlendFactor,
        RHIBlendOp colorBlendOp,
        RHIBlendFactor srcAlphaBlendFactor,
        RHIBlendFactor dstAlphaBlendFactor,
        RHIBlendOp alphaBlendOp)
        : SrcColorBlendFactor(srcColorBlendFactor)
        , DstColorBlendFactor(dstColorBlendFactor)
        , ColorBlendOp(colorBlendOp)
        , SrcAlphaBlendFactor(srcAlphaBlendFactor)
        , DstAlphaBlendFactor(dstAlphaBlendFactor)
        , AlphaBlendOp(alphaBlendOp)
    {
    }
};

struct RHIColorBlendState
{
    bool EnableBlend = false;
    RHIColorBlendEquation ColorBlendEquation;

    RHIColorBlendState() = default;

    RHIColorBlendState(
        bool enableBlend,
        RHIBlendFactor srcColorBlendFactor,
        RHIBlendFactor dstColorBlendFactor,
        RHIBlendOp colorBlendOp,
        RHIBlendFactor srcAlphaBlendFactor,
        RHIBlendFactor dstAlphaBlendFactor,
        RHIBlendOp alphaBlendOp)
        : EnableBlend(enableBlend)
        , ColorBlendEquation(
              srcColorBlendFactor,
              dstColorBlendFactor,
              colorBlendOp,
              srcAlphaBlendFactor,
              dstAlphaBlendFactor,
              alphaBlendOp)
    {
    }
};

struct RHIColorBlendDesc
{
    std::vector<RHIColorBlendState> AttachmentColorBlendStates;
    RHIColor Constants = {0, 0, 0, 0};

    RHIColorBlendDesc() = default;

    RHIColorBlendDesc(const std::initializer_list<RHIColorBlendState>& attachmentColorBlendStates)
        : AttachmentColorBlendStates(attachmentColorBlendStates)
    {
    }
};

enum class RHIDynamicState : uint32_t
{
    CullMode,
    FrontMode,
    PrimitiveTopology,
};

struct RHIDynamicStatesDesc
{
    std::vector<RHIDynamicState> DynamicStates;

    RHIDynamicStatesDesc() = default;

    RHIDynamicStatesDesc(const std::initializer_list<RHIDynamicState>& dynamicStates)
        : DynamicStates(dynamicStates)
    {
    }
};

enum class ERHIShaderStage : uint32_t
{
    RHI_SHADER_STAGE_VERTEX_BIT,
    RHI_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    RHI_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    RHI_SHADER_STAGE_GEOMETRY_BIT,
    RHI_SHADER_STAGE_FRAGMENT_BIT,
    RHI_SHADER_STAGE_COMPUTE_BIT,
    RHI_SHADER_STAGE_ALL_GRAPHICS,
};

struct RHIConstantRange
{
    ERHIShaderStage ShaderStage = ERHIShaderStage::RHI_SHADER_STAGE_VERTEX_BIT;
    uint32_t Size = 0;
};

struct RHIGraphicPipelineStats
{
    Ref<RHIShader> ShaderVS;
    Ref<RHIShader> ShaderPS;
    RHIVertexInputLayout VertexInputLayout;
    RHIInputAssemblyInfo InputAssemblyInfo;
    RHIColorBlendDesc ColorBlendDesc;
    RHIDynamicStatesDesc DynamicStatesDesc;
    std::vector<RHIConstantRange> ConstantRanges;
    std::vector<Ref<RHIDescriptorSet>> DescriptorSets;
};

struct RHIGraphicPipelineCreateInfo
{
    RHIGraphicPipelineStats PipelineStats;
    Ref<RHIRenderPass> RenderPass;
};

struct RHIGraphicPipeline
{
    virtual ~RHIGraphicPipeline() = default;
};

}  //namespace ME
