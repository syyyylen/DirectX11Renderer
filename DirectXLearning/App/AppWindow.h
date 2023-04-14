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

    void UpdateQuadPosition();

    virtual void OnCreate() override;
    virtual void OnUpdate() override;
    virtual void OnDestroy() override;

private:
    SwapChain* m_swapChain;
    VertexBuffer* m_vertexBuffer;
    VertexShader* m_vs;
    PixelShader* m_ps;
    ConstantBuffer* m_cb;

    unsigned long m_oldDeltaTime = 0.0f;
    unsigned long m_newDeltaTime = 0.0f;
    float m_deltaTime = 0.0f;
    float m_angle = 0.0f;

    float m_totalTime = 0.0f;

    float m_deltaPos = 0.0f;
    float m_deltaScale = 0.0f;
};
