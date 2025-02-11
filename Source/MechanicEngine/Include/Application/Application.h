﻿#pragma once
#include <memory>
#include <string>
#include "MechanicEngine/Include/Core/Timestep.h"
#include "MechanicEngine/Include/Core/Ptr.h"
#include "MechanicEngine/Include/Window/Window.h"
#include "MechanicEngine/Include/Event/ApplicationEvent.h"
#include "MechanicEngine/Include/Render/RHI.h"
#include "Layer.h"

namespace ME
{
struct ApplicationSpecification
{
    std::string Name = "Application";
    std::string BinPath;
    std::string ResourcePath;
};

class Application
{
public:
    Application(const ApplicationSpecification& spec);
    virtual ~Application() = default;
    static Application& Get();

public:
    void Run();
    Window& GetWindow();
    Ref<RHI> GetRHI();
    const std::string& GetBinPath();
    const std::string& GetResourcePath();

protected:
    void PushLayer(std::shared_ptr<Layer> layer);
    void PushOverlay(std::shared_ptr<Layer> layer);

private:
    virtual bool OnEngineInit() = 0;

private:
    bool InitApp();
    Timestep GetTimestep();
    void OnEvent(Event& event);
    bool OnWindowClosed(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
    void UpdateLayers(Timestep timestep);
    void UpdateLayersUI(Timestep timestep);

private:
    static Application* s_Instance;
    ApplicationSpecification m_AppSpec;
    bool m_Running = true;
    bool m_WndMinimized = false;
    bool m_WndResized = false;
    double m_LastFrameTimeInSec = 0.f;

    Ref<Window> m_Window;
    std::shared_ptr<class LayerStack> m_LayerStack;
    Ref<class ImGuiLayer> m_ImGuiLayer;
    Ref<class ImGuiRenderPass> m_ImGuiRenderPass;
    std::shared_ptr<RHI> m_RHI;
};

std::unique_ptr<Application> CreateApplication();

}  //namespace ME