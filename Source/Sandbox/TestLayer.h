﻿#pragma once
#include <iostream>
#include <string>
#include <array>
#include "MechanicEngine/Include/Application/Layer.h"
#include "MechanicEngine/Include/Event/MouseEvent.h"
#include "MechanicEngine/Include/Event/KeyEvent.h"
#include "MechanicEngine/Include/Event/ApplicationEvent.h"
#include "TestRenderer.h"

class TestLayer : public ME::Layer
{
public:
    TestLayer();

public:
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(ME::Timestep timestep) override;
    virtual void OnUIUpdate() override;
    virtual void OnEvent(ME::Event& event) override;

private:
    void BeginDockspace();
    void EndDockspace();

private:
    ME::Ref<ME::RHI> m_RHI;
    ME::Ref<TestRenderer> m_TestRenderer;
    std::array<uint32_t, 2> m_ViewportSize = {0, 0};
    std::array<uint32_t, 2> m_CacheViewportSize = {0, 0};
};
