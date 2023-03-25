#include "AppWindow.h"

#include "../GameEngine/GraphicsEngine/DeviceContext/DeviceContext.h"
#include "../GameEngine/GraphicsEngine/SwapChain/SwapChain.h"

struct Vec3
{
    float x, y, z;
};

struct Vertex
{
    Vec3 Position;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnCreate()
{
    Window::OnCreate();
    GraphicsEngine::Get()->Init();
    m_swapChain = GraphicsEngine::Get()->CreateSwapChain();
    
    RECT rc = GetClientWindowRect();
    m_swapChain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

    Vertex list[] =
    {
        {-0.5f, -0.5f, 0.0f},
        {0.0f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f}
    };

    m_vertexBuffer = GraphicsEngine::Get()->CreateVertexBuffer();
    UINT listSize = ARRAYSIZE(list);

    GraphicsEngine::Get()->CreateShaders();

    void* shaderByteCode = nullptr;
    UINT shaderSize = 0;
    GraphicsEngine::Get()->GetShaderBufferAndSize(&shaderByteCode, &shaderSize);
    
    m_vertexBuffer->Load(list, sizeof(Vertex), listSize, shaderByteCode, shaderSize);
}

void AppWindow::OnUpdate()
{
    Window::OnUpdate();
    
    GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swapChain, 0, 0.3f, 0.4f, 1);

    // Set the viewport target in which we draw
    RECT rc = GetClientWindowRect();
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    // Some default shader to be able to draw
    GraphicsEngine::Get()->SetShaders();

    // Set the vertices
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_vertexBuffer);

    GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawTriangleList(m_vertexBuffer->GetVertexListSize(), 0);

    m_swapChain->Present(false);
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    m_swapChain->Release();
    m_vertexBuffer->Release();
    GraphicsEngine::Get()->Release();
}
