#pragma once
#include "../GameEngine/WindowingSystem/Window.h"
#include "../GameEngine/GraphicsEngine/GraphicsEngine.h"
#include "../GameEngine/GraphicsEngine/VertexBuffer/VertexBuffer.h"
#include "../GameEngine/GraphicsEngine/VertexShader/VertexShader.h"
#include "../GameEngine/GraphicsEngine/PixelShader/PixelShader.h"

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
    VertexShader* m_vs;
    PixelShader* m_ps;
    ConstantBuffer* m_cb;

    unsigned long m_oldTime = 0;
    float m_deltaTime = 0.0f;
    float m_angle = 0.0f;
};
