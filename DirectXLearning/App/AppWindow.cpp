#include "AppWindow.h"

#include "../GameEngine/GraphicsEngine/DeviceContext/DeviceContext.h"
#include "../GameEngine/GraphicsEngine/SwapChain/SwapChain.h"
#include <Windows.h>

#include "../GameEngine/InputSystem/InputSystem.h"
#include "../GameEngine/Math/Vector3.h"
#include "../GameEngine/Math/Matrix4x4.h"
#include "../imgui_implem/imgui_impl_dx11.h"
#include "../imgui_implem/imgui_impl_win32.h"

struct Vertex
{
    Vector3 Pos;
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

static float ColorScalar = 2.0f;

static float MouseSensivity = 2.0f;
static float MoveSpeed = 2.0f;
static float StrafeMoveSpeed = 1.0f;

static float InputsDownScalar = 0.03f;


void AppWindow::Update()
{
    // Color changing 
    m_angle += ColorScalar*m_deltaTime;
    Constant cc;
    cc.Angle = m_angle;

    // Transformation matrices
    
    cc.m_world.SetIdentity();

    Matrix4x4 worldCam;
    Matrix4x4 temp;

    worldCam.SetIdentity();
    temp.SetIdentity();
    temp.SetRotationX(m_rotationX);
    worldCam *= temp;
    temp.SetIdentity();
    temp.SetRotationY(m_rotationY);
    worldCam *= temp;

    Vector3 newPos = m_worldCam.GetTranslation() + worldCam.GetZDirection() * (m_camForward * (MoveSpeed * InputsDownScalar));
    newPos = newPos + worldCam.GetXDirection() * (m_camRight * (StrafeMoveSpeed * InputsDownScalar));
    worldCam.SetTranslation(newPos);
    m_worldCam = worldCam;
    worldCam.Inverse();
    cc.m_view = worldCam;

    int width = GetClientWindowRect().right - GetClientWindowRect().left;
    int height = GetClientWindowRect().bottom - GetClientWindowRect().top;
    
    // Orthographic cam 
    // cc.m_projection.SetOrthoLH(width/400.0f, height/400.0f, -4.0f, 4.0f);

    cc.m_projection.SetPerspectiveFovLH(1.57f, ((float)width/(float)height), 0.1f, 100.0f);
    
    m_cb->Update(GraphicsEngine::Get()->GetImmediateDeviceContext(), &cc);
}

void AppWindow::OnCreate()
{
    Window::OnCreate();
    InputSystem::Get()->AddListener(this);
    InputSystem::Get()->ShowCursor(false);
    GraphicsEngine::Get()->Init();
    m_swapChain = GraphicsEngine::Get()->CreateSwapChain();

    RECT rc = GetClientWindowRect();
    m_swapChain->Init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

    m_worldCam.SetTranslation(Vector3(0.0f, 0.0f, -2.0f));

    Vertex vertexList[] =
    {
        //X - Y - Z
        //FRONT FACE
        {Vector3(-0.5f,-0.5f,-0.5f),    Vector3(1,0,0),  Vector3(0.2f,0,0) },
        {Vector3(-0.5f,0.5f,-0.5f),    Vector3(1,1,0), Vector3(0.2f,0.2f,0) },
        { Vector3(0.5f,0.5f,-0.5f),   Vector3(1,1,0),  Vector3(0.2f,0.2f,0) },
        { Vector3(0.5f,-0.5f,-0.5f),     Vector3(1,0,0), Vector3(0.2f,0,0) },

        //BACK FACE
        { Vector3(0.5f,-0.5f,0.5f),    Vector3(0,1,0), Vector3(0,0.2f,0) },
        { Vector3(0.5f,0.5f,0.5f),    Vector3(0,1,1), Vector3(0,0.2f,0.2f) },
        { Vector3(-0.5f,0.5f,0.5f),   Vector3(0,1,1),  Vector3(0,0.2f,0.2f) },
        { Vector3(-0.5f,-0.5f,0.5f),     Vector3(0,1,0), Vector3(0,0.2f,0) }
    };

    m_vertexBuffer = GraphicsEngine::Get()->CreateVertexBuffer();
    UINT listSize = ARRAYSIZE(vertexList);

    unsigned int index_list[]=
    {
        //FRONT SIDE
        0,1,2,  //FIRST TRIANGLE
        2,3,0,  //SECOND TRIANGLE
        //BACK SIDE
        4,5,6,
        6,7,4,
        //TOP SIDE
        1,6,5,
        5,2,1,
        //BOTTOM SIDE
        7,0,3,
        3,4,7,
        //RIGHT SIDE
        3,2,5,
        5,4,3,
        //LEFT SIDE
        7,6,1,
        1,0,7
    };

    m_indexBuffer = GraphicsEngine::Get()->CreateIndexBuffer();
    UINT indexListSize = ARRAYSIZE(index_list);
    m_indexBuffer->Load(index_list, indexListSize);

    void* shaderByteCode = nullptr;
    size_t shaderSize = 0;

    // Vertex Shader compilation & creation
    GraphicsEngine::Get()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
    m_vs = GraphicsEngine::Get()->CreateVertexShader(shaderByteCode, shaderSize);
    m_vertexBuffer->Load(vertexList, sizeof(Vertex), listSize, shaderByteCode, shaderSize);
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
    InputSystem::Get()->Update();
    
    GraphicsEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swapChain, 0.0f, 0.3f,0.4f, 1);

    // Set the viewport target in which we draw
    RECT rc = GetClientWindowRect();
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    // Shader constants 
    Update();
    
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetConstantBuffer(m_vs, m_cb);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetConstantBuffer(m_ps, m_cb);

    // Set the shaders
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexShader(m_vs);
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetPixelShader(m_ps);

    // Set the vertices of the triangle to draw
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(m_vertexBuffer);
    // Set the indices of the triangle to draw
    GraphicsEngine::Get()->GetImmediateDeviceContext()->SetIndexBuffer(m_indexBuffer);

    // finally draw
    GraphicsEngine::Get()->GetImmediateDeviceContext()->DrawIndexedTriangleList(m_indexBuffer->GetIndexListSize(), 0, 0);

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

    { // My imgui test window
        ImGui::Begin("ImGui test window");                  
        ImGui::SliderFloat("ColorSpeed", &ColorScalar, 0.0f, 5.0f);
        ImGui::SliderFloat("Sensivity", &MouseSensivity, 0.1f, 10.0f);            
        ImGui::SliderFloat("MoveSpeed", &MoveSpeed, 0.1f, 10.0f);            
        ImGui::SliderFloat("StrafeMoveSpeed", &StrafeMoveSpeed, 0.1f, 10.0f);            
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    GraphicsEngine::Get()->GetDeviceContext()->OMSetRenderTargets(1, m_swapChain->GetRTVLValue(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_swapChain->Present(0); // Present with vsync
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    m_swapChain->Release();
    m_vertexBuffer->Release();
    m_indexBuffer->Release();
    m_cb->Release();
    m_vs->Release();
    m_ps->Release();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    GraphicsEngine::Get()->Release();
}

void AppWindow::OnFocus()
{
    InputSystem::Get()->AddListener(this);
}

void AppWindow::OnLooseFocus()
{
    InputSystem::Get()->RemoveListener(this);
}

void AppWindow::OnKeyDown(int key)
{
    if(key == 'Z')
    {
        m_camForward = 1.0f;
    }
    else if(key == 'S')
    {
        m_camForward = -1.0f;
    }
    else if(key == 'Q')
    {
        m_camRight = -1.0f;
    }
    else if(key == 'D')
    {
        m_camRight = 1.0f;
    }
    else if(key == 'E' && m_EKeyLocked == false)
    {
        m_isMouseLocked = !m_isMouseLocked;
        m_isMouseLocked ? LOG("Mouse Locked") : LOG("Mouse Unlocked");
        m_EKeyLocked = true;
    }
}

void AppWindow::OnKeyUp(int key)
{
    m_camForward = 0.0f;
    m_camRight = 0.0f;

    if(key == 'E')
        m_EKeyLocked = false;
}

void AppWindow::OnMouseMove(const Point& mousePosition)
{
    if(m_isMouseLocked) // Mouse Locked & Hidden while not in UI and moving inside the scene
    {
        int width = GetClientWindowRect().right - GetClientWindowRect().left;
        int height = GetClientWindowRect().bottom - GetClientWindowRect().top;
    
        m_rotationX += (mousePosition.Y - (height / 2.0f)) * m_deltaTime * (MouseSensivity * InputsDownScalar);
        m_rotationY += (mousePosition.X - (width / 2.0f)) * m_deltaTime * (MouseSensivity * InputsDownScalar);

        InputSystem::Get()->SetCursorPosition(Point(width/2.0f, height/2.0f));
        if(InputSystem::Get()->IsCursorVisible())
            InputSystem::Get()->ShowCursor(false);
    }
    else // Mouse Visible when E key pressed to move in UI  TODO find a better keymapping
    {
        if(!InputSystem::Get()->IsCursorVisible())
            InputSystem::Get()->ShowCursor(true);
    }
}

void AppWindow::OnLeftMouseDown(const Point& mousePos)
{
    
}

void AppWindow::OnRightMouseDown(const Point& mousePos)
{
    
}

void AppWindow::OnLeftMouseUp(const Point& mousePos)
{
    
}

void AppWindow::OnRightMouseUp(const Point& mousePos)
{
    
}
