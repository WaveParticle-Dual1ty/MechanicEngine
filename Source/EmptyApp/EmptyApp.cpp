#include "EmptyApp.h"
#include <memory>
#include <iostream>
#include <filesystem>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"

std::unique_ptr<ME::Application> ME::CreateApplication()
{
    return std::make_unique<EmptyApp>();
}

EmptyApp::EmptyApp()
    : Application({"EmptyApp"})
{
}

bool EmptyApp::OnEngineInit()
{
    return true;
}
