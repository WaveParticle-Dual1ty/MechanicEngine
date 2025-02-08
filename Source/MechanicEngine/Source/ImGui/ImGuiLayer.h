#pragma once
#include <string>
#include "MechanicEngine/Include/Core/Ptr.h"
#include "MechanicEngine/Include/Application/Layer.h"
#include "MechanicEngine/Include/Render/RHI.h"
#include "MechanicEngine/Include/Window/Window.h"

namespace ME
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::string& name);
    virtual ~ImGuiLayer() = default;
    static Ref<ImGuiLayer> Create(RHI::API api);

public:
    virtual bool Init(const std::string& iniFilename, Ref<Window> wnd, Ref<RHI> rhi, Ref<RHIRenderPass> renderPass) = 0;
    virtual void Begin() = 0;
    virtual void End() = 0;
};

}  //namespace ME