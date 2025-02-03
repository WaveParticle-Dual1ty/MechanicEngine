#pragma once
#include <iostream>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"

class MechanicEngineEditor : public ME::Application
{
public:
    MechanicEngineEditor();

private:
    virtual bool OnEngineInit() override;
};
