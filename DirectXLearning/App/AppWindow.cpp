#include "AppWindow.h"

#include "../GameEngine/GraphicsEngine/DeviceContext/DeviceContext.h"
#include "../GameEngine/GraphicsEngine/SwapChain/SwapChain.h"
#include <Windows.h>
#include "../GameEngine/Math/Vector3.h"
#include "../GameEngine/Math/Matrix4x4.h"
#include "../imgui_implem/imgui_impl_dx11.h"
#include "../imgui_implem/imgui_impl_win32.h"


struct Vertex
{
    Vector3 Pos;
    Vector3 Pos1;
    Vector3 Color;
    Vector3 Color1;
};

// DX Nvidia memory chunks of 16 bytes, so if we exceed 16 bytes we need to allocate a multiple of 16 memory
// this line does it for us, god bless her 
__declspec(align(16))
struct Constant
{
    Matrix4x4 m_world;
    Matrix4x4 m_view;
    Matrix4x4 m_projection;
    float Angle;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::UpdateQuadPosition()
{
    m_angle += 1.57f*m_deltaTime;
    Constant cc;
    cc.Angle = m_angle;

    m_deltaPos += m_deltaTime / 15.0f;
    if(m_deltaPos > 1.0f)
        m_deltaPos = 0.0f;

    m_deltaScale += m_deltaTime * 5.0f;

    // transformation matrices
    Matrix4x4 temp;
    cc.m_world.SetScale(Vector3::Lerp(Vector3(0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), (sin(m_deltaScale)+1.0f)/2.0f));
    temp.SetTranslation(Vector3::Lerp(Vector3(-1.5f, -1.5f, 0), Vector3(1.5f, 1.5f, 0), m_deltaPos));
    cc.m_world *= temp;
    cc.m_view.SetIdentity();
    cc.m_projection.SetOrthoLH((GetClientWindowRect().right - GetClientWindowRect().left) / 400.0f,
                               (GetClientWindowRect().bottom - GetClientWindowRect().top) / 400.0f,
                               -4.0f, 4.0f);
    
    m_cb->Update(GraphicsEngine::Get()->GetImmediateDeviceContext(), &cc);
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
        {Vector3(-0.5f, -0.5f, 0.0f),    Vector3(-0.32f, -0.11f, 0.0f),      Vector3(0,0,0), Vector3(0,1,0)},
        {Vector3(-0.5f, 0.5f, 0.0f),     Vector3(-0.11f, 0.78f, 0.0f),      Vector3(0,0,0), Vector3(1,1,0)},
        {Vector3(0.5f, -0.5f, 0.0f),     Vector3(0.75f, -0.73f, 0.0f),      Vector3(0,0,0), Vector3(1,0,0)},
        {Vector3(0.5f, 0.5f, 0.0f),      Vector3(0.88f, 0.77f, 0.0f),      Vector3(0,0,0), Vector3(0,0,1)}
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

    Constant cc;
    cc.Angle = 0.0f;
    
    // Constant buffer allocation
    m_cb = GraphicsEngine::Get()->CreateConstantBuffer();
    m_cb->Load(&cc, sizeof(Constant));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hwnd); 
    ImGui_ImplDX11_Init(GraphicsEngine::Get()->GetDevice(), GraphicsEngine::Get()->GetDeviceContext());
}

void AppWindow::OnUpdate()
{
    Window::OnUpdate();
    
    GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swapChain, 1.0f, 1.0f, 1.0f, 1);

    // Set the viewport target in which we draw
    RECT rc = GetClientWindowRect();
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    // Shader constants 
    UpdateQuadPosition();
    
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetConstantBuffer(m_vs, m_cb);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetConstantBuffer(m_ps, m_cb);

    // Set the shaders
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexShader(m_vs);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetPixelShader(m_ps);

    // Set the vertices
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_vertexBuffer);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawTriangleStrip(m_vertexBuffer->GetVertexListSize(), 0);

    m_swapChain->Present(true);

    // time
    m_oldDeltaTime = m_newDeltaTime;
    m_newDeltaTime = GetTickCount();
    m_deltaTime = m_oldDeltaTime ? (m_newDeltaTime - m_oldDeltaTime) / 1000.0f : 0;
    m_totalTime += m_deltaTime;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    bool ShowDemoWindow = true;
    if (ShowDemoWindow)
        ImGui::ShowDemoWindow(&ShowDemoWindow);

    // Rendering
    ImGui::Render();
    GraphicsEngine::Get()->GetDeviceContext()->OMSetRenderTargets(1, m_swapChain->GetRTVLValue(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_swapChain->Present(0); // Present with vsync
    //g_pSwapChain->Present(0, 0); // Present without vsync
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    m_swapChain->Release();
    m_vertexBuffer->Release();
    m_vs->Release();
    m_ps->Release();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    GraphicsEngine::Get()->Release();
}
