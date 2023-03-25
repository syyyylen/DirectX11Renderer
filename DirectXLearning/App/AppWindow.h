#pragma once
#include "../GameEngine/WindowingSystem/Window.h"
#include "../GameEngine/GraphicsEngine/GraphicsEngine.h"
#include "../GameEngine/GraphicsEngine/VertexBuffer/VertexBuffer.h"

class AppWindow : public Window
{
public:
    AppWindow();
    ~AppWindow();

    virtual void OnCreate() override;
    virtual void OnUpdate() override;
    virtual void OnDestroy() override;

private:
    SwapChain* m_swapChain;
    VertexBuffer* m_vertexBuffer;
};
