#include "MechanicPic.h"
#include <memory>
#include <iostream>
#include <filesystem>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"

namespace ME
{
std::unique_ptr<Application> CreateApplication()
{
    return std::make_unique<MechanicPic>();
}

MechanicPic::MechanicPic()
    : Application({"MechanicPic"})
{
}

bool MechanicPic::OnEngineInit()
{
    return true;
}

}  //namespace ME
