#pragma once
#include <iostream>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"

class EmptyApp : public ME::Application
{
public:
    EmptyApp();

private:
    virtual bool OnEngineInit() override;
};
