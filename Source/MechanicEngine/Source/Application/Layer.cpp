﻿#include "MechanicEngine/Include/Application/Layer.h"

namespace ME
{
Layer::Layer(const std::string& name)
    : m_DebugName(name)
{
}

void Layer::OnAttach()
{
}

void Layer::OnDetach()
{
}

void Layer::OnUpdate(Timestep timestep)
{
    static_cast<void>(timestep);
}

void Layer::OnUIUpdate()
{
}

void Layer::OnEvent(Event& event)
{
    static_cast<void>(event);
}

const std::string& Layer::GetName()
{
    return m_DebugName;
}

}  //namespace ME