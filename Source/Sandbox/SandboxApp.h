#pragma once
#include <iostream>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"

class SandboxApp : public ME::Application
{
public:
    SandboxApp(const std::string& binPath, const std::string& resourcePath);

private:
    virtual bool OnEngineInit() override;
};
