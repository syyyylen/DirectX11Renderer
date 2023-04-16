#pragma once
#include "../GameEngine/WindowingSystem/Window.h"
#include "../GameEngine/GraphicsEngine/GraphicsEngine.h"
#include "../GameEngine/GraphicsEngine/Renderer/VertexBuffer/VertexBuffer.h"
#include "../GameEngine/GraphicsEngine/Renderer/IndexBuffer/IndexBuffer.h"
#include "../GameEngine/GraphicsEngine/Renderer/VertexShader/VertexShader.h"
#include "../GameEngine/GraphicsEngine/Renderer/PixelShader/PixelShader.h"
#include "../GameEngine/InputSystem/InputListener.h"
#include "../GameEngine/Math/Matrix4x4.h"


class AppWindow : public Window, public InputListener
{
public:
    AppWindow();
    ~AppWindow();

    void Update();

    // Window overrides
    virtual void OnCreate() override;
    virtual void OnUpdate() override;
    virtual void OnDestroy() override;
    virtual void OnFocus() override;
    virtual void OnLooseFocus() override;

    // InputListener interface implem
    void OnKeyDown(int key) override;
    void OnKeyUp(int key) override;
    void OnMouseMove(const Point& mousePosition) override;
    virtual void OnLeftMouseDown(const Point& mousePos) override;
    virtual void OnRightMouseDown(const Point& mousePos) override;
    virtual void OnLeftMouseUp(const Point& mousePos) override;
    virtual void OnRightMouseUp(const Point& mousePos) override;

private:
    std::shared_ptr<SwapChain> m_swapChain;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<VertexShader> m_vs;
    std::shared_ptr<PixelShader> m_ps;
    std::shared_ptr<ConstantBuffer> m_cb;

    // Time
    unsigned long m_oldDeltaTime = 0.0f;
    unsigned long m_newDeltaTime = 0.0f;
    float m_deltaTime = 0.0f;
    float m_angle = 0.0f;

    float m_totalTime = 0.0f;

    // Experiments
    float m_rotationX = 0.0f;
    float m_rotationY = 0.0f;
    
    // Camera 
    float m_camForward = 0.0f;
    float m_camRight = 0.0f;
    Matrix4x4 m_worldCam;

    // UI state when E key pressed
    bool m_EKeyLocked = false;
    bool m_isMouseLocked = true;

    // Tex
    std::shared_ptr<Texture> m_woodTexture;
};
