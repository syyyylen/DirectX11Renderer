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
    Vec3 Color;
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
        {-0.5f, -0.5f, 0.0f,    1,0,0},
        {-0.5f, 0.5f, 0.0f,     0,1,0},
        {0.5f, -0.5f, 0.0f,     0,0,1},
        {0.5f, 0.5f, 0.0f,            1,1,0}
    };

    m_vertexBuffer = GraphicsEngine::Get()->CreateVertexBuffer();
    UINT listSize = ARRAYSIZE(list);

    void* shaderByteCode = nullptr;
    size_t shaderSize = 0;

    // Vertex Shader compilation & creation
    GraphicsEngine::Get()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
    m_vs = GraphicsEngine::Get()->CreateVertexShader(shaderByteCode, shaderSize);
    m_vertexBuffer->Load(list, sizeof(Vertex), listSize, shaderByteCode, shaderSize);
    GraphicsEngine::Get()->ReleaseCompiledShader();

    // Pixel Shader compilation & creation
    GraphicsEngine::Get()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
    m_ps = GraphicsEngine::Get()->CreatePixelShader(shaderByteCode, shaderSize);
    GraphicsEngine::Get()->ReleaseCompiledShader();
}

void AppWindow::OnUpdate()
{
    Window::OnUpdate();
    
    GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swapChain, 1.0f, 1.0f, 1.0f, 1);

    // Set the viewport target in which we draw
    RECT rc = GetClientWindowRect();
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    // Set the shaders
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexShader(m_vs);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetPixelShader(m_ps);

    // Set the vertices
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_vertexBuffer);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawTriangleStrip(m_vertexBuffer->GetVertexListSize(), 0);

    m_swapChain->Present(true);
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    m_swapChain->Release();
    m_vertexBuffer->Release();
    m_vs->Release();
    m_ps->Release();
    GraphicsEngine::Get()->Release();
}
