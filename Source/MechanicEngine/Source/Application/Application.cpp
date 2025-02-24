﻿#include "MechanicEngine/Include/Application/Application.h"
#include <thread>
#include <chrono>
#include "MechanicEngine/Include/Core/Time.h"
#include "MechanicEngine/Include/Core/Assert.h"
#include "MechanicEngine/Source/ImGui/ImGuiLayer.h"
#include "MechanicEngine/Source/ImGui/ImGuiRenderer.h"
#include "AppLog.h"
#include "LayerStack.h"

namespace ME
{
Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& spec)
    : m_AppSpec(spec)
{
    ME_ASSERT(s_Instance == nullptr, "Application can not create once");
    s_Instance = this;
}

Application& Application::Get()
{
    return *s_Instance;
}

void Application::Run()
{
    bool ret = InitApp();
    if (!ret)
    {
        APP_LOG_ERROR("InitApp fail");
        return;
    }

    while (m_Running)
    {
        Timestep timestep = GetTimestep();

        m_Window->PollEvents();

        if (m_WndMinimized)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (m_WndResized)
        {
            m_WndResized = false;

            uint32_t w = m_Window->GetWidth();
            uint32_t h = m_Window->GetHeight();
            ret = m_RHI->Resize(w, h);
            if (!ret)
            {
                APP_LOG_ERROR("RHI::Resize fail");
                break;
            }

            RHISwapchainInfo swapchainInfo = m_RHI->GetSwapchainInfo();
            RHIExtend2D swapchainSize = swapchainInfo.Extend;
            ret = m_ImGuiRenderer->Resize(swapchainSize.Width, swapchainSize.Height);
            if (!ret)
            {
                APP_LOG_ERROR("ImGuiRenderer::Resize fail");
                break;
            }
        }

        m_RHI->PrepareForNextFrame();

        Ref<RHICommandBuffer> cmdBuffer = m_RHI->GetCurrentCommandBuffer();
        m_RHI->BeginCommandBuffer(cmdBuffer);

        for (auto& layer : m_LayerStack->GetLayers())
        {
            layer->OnUpdate(timestep);
        }

        {
            m_ImGuiLayer->Begin();

            for (auto& layer : m_LayerStack->GetLayers())
            {
                layer->OnUIUpdate();
            }

            m_ImGuiLayer->End();
        }

        Ref<RHITexture2D> backTexture = m_RHI->GetCurrentBackTexture();

        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_TOP_OF_PIPE_BIT, RHI_PIPELINE_STAGE_TRANSFER_BIT, backTexture,
                           ERHITextureUsage::None, ERHITextureUsage::TransferDst));

        m_RHI->CmdClearColor(cmdBuffer, backTexture, RHIColor(0, 0, 0, 1));

        m_ImGuiRenderer->Draw(cmdBuffer);

        Ref<RHITexture2D> uiTexture = m_ImGuiRenderer->GetTargetColorTexture();

        m_RHI->CmdTransition(
            cmdBuffer, RHITransition(
                           RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, RHI_PIPELINE_STAGE_TRANSFER_BIT, uiTexture,
                           ERHITextureUsage::ColorAttachment, ERHITextureUsage::TransferSrc));

        m_RHI->CmdCopyTexture(cmdBuffer, uiTexture, backTexture);

        m_RHI->CmdPipelineBarrier(
            cmdBuffer,
            RHIPipelineBarrierInfo(
                backTexture, RHI_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, RHI_ACCESS_MEMORY_READ_BIT,
                ERHIImageLayout::RHI_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                ERHIImageLayout::RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR, RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                RHI_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, RHI_IMAGE_ASPECT_COLOR_BIT));

        m_RHI->EndCommandBuffer(cmdBuffer);

        m_RHI->SubmmitRenderCommands();
        m_RHI->Present();
    }

    m_Window->Destroy();

    APP_LOG_INFO("Application stop");
}

Window& Application::GetWindow()
{
    return *m_Window.get();
}

Ref<RHI> Application::GetRHI()
{
    return m_RHI;
}

const std::string& Application::GetBinPath()
{
    return m_AppSpec.BinPath;
}

const std::string& Application::GetResourcePath()
{
    return m_AppSpec.ResourcePath;
}

void Application::PushLayer(std::shared_ptr<Layer> layer)
{
    m_LayerStack->PushLayer(layer);
}

void Application::PushOverlay(std::shared_ptr<Layer> layer)
{
    m_LayerStack->PushOverlay(layer);
}

bool Application::InitApp()
{
    WindowProps props;
    props.Title = m_AppSpec.Name;
    props.Width = 1500;
    props.Height = 800;

    m_Window = Window::Create();
    bool ret = m_Window->InitWindow(props);
    if (!ret)
    {
        APP_LOG_ERROR("Window::InitWindow fail");
        return false;
    }

    m_Window->SetEventCallback(
        [this](Event& event)
        {
            OnEvent(event);
        });

    RHI::API renderAPI = RHI::API::Vulkan;
    m_RHI = RHI::CreateRHI(renderAPI);
    ret = m_RHI->Initialize(m_Window);
    if (!ret)
    {
        APP_LOG_ERROR("RHI::Initialize fail");
        return false;
    }

    m_LayerStack = std::make_shared<LayerStack>();
    {
        RHISwapchainInfo swapchainInfo = m_RHI->GetSwapchainInfo();
        RHIExtend2D swapchainSize = swapchainInfo.Extend;

        m_ImGuiRenderer = CreateRef<ImGuiRenderer>(m_RHI);
        ret = m_ImGuiRenderer->Init(swapchainSize.Width, swapchainSize.Height, swapchainInfo.PixelFormat);
        if (!ret)
        {
            APP_LOG_ERROR("ImGuiRenderer::Init fail");
            return false;
        }

        m_ImGuiLayer = ImGuiLayer::Create(renderAPI);
        if (m_ImGuiLayer == nullptr)
        {
            APP_LOG_ERROR("ImGuiLayer::Create fail");
            return false;
        }

        ret = m_ImGuiLayer->Init(m_AppSpec.Name, m_Window, m_RHI, m_ImGuiRenderer->GetRenderPass());
        if (!ret)
        {
            APP_LOG_ERROR("ImGuiLayer::Init fail");
            return false;
        }

        PushOverlay(m_ImGuiLayer);
    }

    ret = OnEngineInit();
    if (!ret)
    {
        APP_LOG_ERROR("OnEngineInit fail");
        return false;
    }

    return true;
}

Timestep Application::GetTimestep()
{
    double time = Time::GetCurrentTimeInSecond();
    Timestep timestep = time - m_LastFrameTimeInSec;
    m_LastFrameTimeInSec = time;
    return timestep;
}

void Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent& event) -> bool
        {
            return OnWindowClosed(event);
        });

    dispatcher.Dispatch<WindowResizeEvent>(
        [this](WindowResizeEvent& event) -> bool
        {
            return OnWindowResize(event);
        });

    const LayerStack::LayerList& layers = m_LayerStack->GetLayers();
    for (auto it = layers.rbegin(); it != layers.rend(); ++it)
    {
        const std::shared_ptr<Layer>& layer = *it;
        layer->OnEvent(event);
        if (event.Handled)
            break;
    }
}

bool Application::OnWindowClosed(WindowCloseEvent& event)
{
    APP_LOG_INFO("Application receive event: {}", event.ToString());
    m_Running = false;
    return false;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
    if (event.GetWidth() == 0 || event.GetHeight() == 0)
    {
        APP_LOG_INFO("Application receive minimized event");
        m_WndMinimized = true;
        return false;
    }
    else
    {
        m_WndMinimized = false;
        m_WndResized = true;
        return false;
    }
}

void Application::UpdateLayers(Timestep timestep)
{
    for (const auto& layer : m_LayerStack->GetLayers())
    {
        layer->OnUpdate(timestep);
    }
}

void Application::UpdateLayersUI(Timestep timestep)
{
    static_cast<void>(timestep);
}

}  //namespace ME
