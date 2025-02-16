#include "EditorLayer.h"
#include <array>
#include "ThirdParty/imgui/imgui.h"
#include "MechanicEngine/Include/Core/Assert.h"
#include "MechanicEngine/Include/Application/Application.h"
#include "MechanicEngine/Include/Event/MouseEvent.h"
#include "MechanicEngine/Include/Event/Input.h"
#include "MechanicEngine/Include/Event/EventUtils.h"
#include "MechanicEngine/Include/Media/MediaEnumToStr.h"
#include "MechanicPicLog.h"

namespace ME
{
EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
}

void EditorLayer::OnAttach()
{
    m_RHI = Application::Get().GetRHI();

    m_ViewportSize = {300, 200};
    m_CacheViewportSize = m_ViewportSize;

    m_PicRenderer = CreateRef<PicRenderer>();
    bool ret = m_PicRenderer->Init(300, 200);
    if (!ret)
    {
        ME_ASSERT(false, "PicRenderer::Init fail");
        return;
    }
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(ME::Timestep timestep)
{
    static_cast<void>(timestep);

    std::array<uint32_t, 2> size = m_CacheViewportSize;
    if (size[0] != m_ViewportSize[0] || size[1] != m_ViewportSize[1])
    {
        m_ViewportSize = size;
        bool ret = m_PicRenderer->Resize(size[0], size[1]);
        if (!ret)
        {
            ME_ASSERT(false, "PicRenderer::Resize fail");
            return;
        }
    }

    Ref<RHICommandBuffer> cmdBuffer = m_RHI->GetCurrentCommandBuffer();
    m_PicRenderer->Draw(cmdBuffer);
}

void EditorLayer::OnUIUpdate()
{
    BeginDockspace();

    ImVec2 viewportSize = ImVec2(0, 0);

    {
        ImGui::Begin("Viewport");

        viewportSize = ImGui::GetContentRegionAvail();
        m_CacheViewportSize = {(uint32_t)viewportSize.x, (uint32_t)viewportSize.y};

        void* texID = m_PicRenderer->GetTargetImTextureID();
        ImGui::Image(texID, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 0), ImVec2(1, 1));

        ImGui::End();
    }

    {
        ImGui::Begin("Information");

        if (m_CurrentImage.Avaliable)
        {
            Ref<FileReader> file = m_CurrentImage.FileReader;
            if (ImGui::CollapsingHeader("File", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("File Name: %s", file->GetFileName().c_str());
                ImGui::Text("File path: %s", file->GetFullPath().c_str());
                ImGui::Text("File size: %s", file->GetFileSizeInStr().c_str());
            }

            Ref<ImageLoader> imageLoader = m_CurrentImage.ImageLoader;
            if (ImGui::CollapsingHeader("Image", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const ImageInfo& imageInfo = imageLoader->GetImageInfo();
                std::string format = Utils::EMPixelFormatToStr(imageInfo.Format);

                ImGui::Text("Type:   \t%s", imageInfo.TypeInStr.c_str());
                ImGui::Text("Width:  \t%d", imageInfo.Width);
                ImGui::Text("Height: \t%d", imageInfo.Height);
                ImGui::Text("Format: \t%s", format.c_str());
            }
        }

        ImGui::End();
    }

    {
        ImGui::Begin("Debug");

        ImGui::Text("Viewport size: (%.2f, %.2f)", viewportSize.x, viewportSize.y);
        ImGui::End();
    }

    bool showDemo = false;
    if (showDemo)
        ImGui::ShowDemoWindow(&showDemo);

    EndDockspace();
}

void EditorLayer::OnEvent(ME::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<FileDropEvent>(
        [this](FileDropEvent& event) -> bool
        {
            return OnFileDrop(event);
        });
}

void EditorLayer::BeginDockspace()
{
    static bool dockspaceOpen = true;
    static bool optFullscreen = true;
    static bool optPadding = false;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!optPadding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
    if (!optPadding)
        ImGui::PopStyleVar();

    if (optFullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    //float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }
}

void EditorLayer::EndDockspace()
{
    ImGui::End();
}

bool EditorLayer::OnFileDrop(FileDropEvent& event)
{
    std::string imagePath = event.GetDropFiles()[0];

    Image image;
    Ref<FileReader>& file = image.FileReader;
    file = CreateRef<FileReader>(imagePath);
    file->Detect();
    if (!file->IsExist())
    {
        MEPIC_LOG_WARN("File not exist: {}", imagePath);
        return false;
    }

    Ref<ImageLoader>& imageLoader = image.ImageLoader;
    imageLoader = ImageLoader::CreateInstance(imagePath);
    imageLoader->Detect();
    bool res = imageLoader->Avaliable();
    if (!res)
    {
        MEPIC_LOG_WARN("File not Avaliable: {}", imagePath);
        return false;
    }

    res = imageLoader->Load();
    if (!res)
    {
        MEPIC_LOG_WARN("Image fail to oad: {}", imagePath);
        return false;
    }

    image.Avaliable = true;

    if (image.Avaliable == true)
        m_CurrentImage = image;

    ImageFrame frame = imageLoader->GetImageFrame();
    ImageInfo imageInfo = imageLoader->GetImageInfo();
    m_PicRenderer->UpdateImageFrame(imageInfo, frame);

    return false;
}

}  //namespace ME
