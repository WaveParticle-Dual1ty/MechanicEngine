#include "MechanicPic.h"
#include <memory>
#include <iostream>
#include <filesystem>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"
#include "EditorLayer.h"

namespace ME
{
std::unique_ptr<Application> CreateApplication()
{
    std::filesystem::path binPath = std::filesystem::current_path();
    std::filesystem::path resourcePath = binPath / "Resources/MechanicPic";
    resourcePath = std::filesystem::absolute(resourcePath);
    return std::make_unique<MechanicPic>(binPath.string(), resourcePath.string());
}

MechanicPic::MechanicPic(const std::string& binPath, const std::string& resourcePath)
    : Application({"MechanicPic", binPath, resourcePath})
{
}

bool MechanicPic::OnEngineInit()
{
    std::shared_ptr<EditorLayer> editorLayer = std::make_shared<EditorLayer>();
    PushLayer(editorLayer);

    return true;
}

}  //namespace ME
