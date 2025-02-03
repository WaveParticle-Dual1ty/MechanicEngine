#pragma once
#include <string>
#include "MechanicEngine/Include/Core/Timestep.h"
#include "MechanicEngine/Include/Event/Event.h"

namespace ME
{
class Layer
{
public:
    Layer(const std::string& name);
    virtual ~Layer() = default;

public:
    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(Timestep timestep);
    virtual void OnUIUpdate();
    virtual void OnEvent(Event& event);

public:
    const std::string& GetName();

private:
    std::string m_DebugName;
};

}  //namespace ME