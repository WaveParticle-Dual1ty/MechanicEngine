#pragma once
#include <iostream>
#include <string>
#include <array>
#include "MechanicEngine/Include/Application/Layer.h"
#include "MechanicEngine/Include/Event/MouseEvent.h"
#include "MechanicEngine/Include/Event/KeyEvent.h"
#include "MechanicEngine/Include/Event/ApplicationEvent.h"
#include "MechanicEngine/Include/Render/RHI.h"

namespace ME
{
class EditorLayer : public Layer
{
public:
    EditorLayer();

public:
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Timestep timestep) override;
    virtual void OnUIUpdate() override;
    virtual void OnEvent(Event& event) override;

private:
    void BeginDockspace();
    void EndDockspace();

private:
    Ref<RHI> m_RHI;
    std::array<uint32_t, 2> m_ViewportSize = {0, 0};
    std::array<uint32_t, 2> m_CacheViewportSize = {0, 0};
};

}  //namespace ME
