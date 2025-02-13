#pragma once
#include <iostream>
#include <string>
#include "MechanicEngine/Include/Core/Core.h"
#include "MechanicEngine/Include/Application/EntryPoint.h"
#include "MechanicEngine/Include/Application/Application.h"

namespace ME
{
class MechanicPic : public Application
{
public:
    MechanicPic(const std::string& binPath, const std::string& resourcePath);

private:
    virtual bool OnEngineInit() override;
};

}  //namespace ME
