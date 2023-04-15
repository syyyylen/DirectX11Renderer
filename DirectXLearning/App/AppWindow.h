#pragma once
#include "../GameEngine/WindowingSystem/Window.h"
#include "../GameEngine/GraphicsEngine/GraphicsEngine.h"
#include "../GameEngine/GraphicsEngine/VertexBuffer/VertexBuffer.h"
#include "../GameEngine/GraphicsEngine/IndexBuffer/IndexBuffer.h"
#include "../GameEngine/GraphicsEngine/VertexShader/VertexShader.h"
#include "../GameEngine/GraphicsEngine/PixelShader/PixelShader.h"
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
    SwapChain* m_swapChain;
    VertexBuffer* m_vertexBuffer;
    IndexBuffer* m_indexBuffer;
    VertexShader* m_vs;
    PixelShader* m_ps;
    ConstantBuffer* m_cb;

    unsigned long m_oldDeltaTime = 0.0f;
    unsigned long m_newDeltaTime = 0.0f;
    float m_deltaTime = 0.0f;
    float m_angle = 0.0f;

    float m_totalTime = 0.0f;

    float m_rotationX = 0.0f;
    float m_rotationY = 0.0f;

    float m_scaleCube = 1.0;

    float m_camForward = 0.0f;
    float m_camRight = 0.0f;
    Matrix4x4 m_worldCam;

    bool m_EKeyLocked = false;
    bool m_isMouseLocked = true;
};
