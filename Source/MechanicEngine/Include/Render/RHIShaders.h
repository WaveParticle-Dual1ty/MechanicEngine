#pragma once
#include <cstdint>
#include <string>

namespace ME
{
enum class ERHIShaderType
{
    Vertex,
    Pixel,
};

struct RHIShaderCreateInfo
{
    ERHIShaderType Type = ERHIShaderType::Vertex;
    std::string ShaderFile;
    std::string EntryName;
};

struct RHIShader
{
    virtual ~RHIShader() = default;

    ERHIShaderType Type = ERHIShaderType::Vertex;
    std::string EntryName;
};

}  //namespace ME
