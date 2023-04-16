﻿#include "AppWindow.h"

#include "../GameEngine/GraphicsEngine/Renderer/DeviceContext/DeviceContext.h"
#include "../GameEngine/GraphicsEngine/Renderer/SwapChain/SwapChain.h"
#include <Windows.h>

#include "../GameEngine/InputSystem/InputSystem.h"
#include "../GameEngine/Math/Vector3.h"
#include "../GameEngine/Math/Vector2.h"
#include "../GameEngine/Math/Matrix4x4.h"
#include "../imgui_implem/imgui_impl_dx11.h"
#include "../imgui_implem/imgui_impl_win32.h"

struct Vertex
{
    Vector3 Pos;
    Vector2 texcoord;
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

static float MouseSensivity = 3.0f;
static float MoveSpeed = 2.0f;
static float StrafeMoveSpeed = 2.0f;

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
    
    m_cb->Update(GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext(), &cc);
}

void AppWindow::OnCreate()
{
    Window::OnCreate();
    
    InputSystem::Get()->AddListener(this);
    InputSystem::Get()->ShowCursor(false);

    m_woodTexture = GraphicsEngine::Get()->GetTextureMgr()->CreateTextureFromFile(L"Assets\\Textures\\wood.jpg");
    
    RECT rc = GetClientWindowRect();
    m_swapChain = GraphicsEngine::Get()->GetRenderer()->CreateSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

    // Cam spawnloc 
    m_worldCam.SetTranslation(Vector3(0.0f, 0.0f, -2.0f));

    Vector3 position_list[] =
    {
        { Vector3(-0.5f,-0.5f,-0.5f)},
        { Vector3(-0.5f,0.5f,-0.5f) },
        { Vector3(0.5f,0.5f,-0.5f) },
        { Vector3(0.5f,-0.5f,-0.5f)},

        //BACK FACE
        { Vector3(0.5f,-0.5f,0.5f) },
        { Vector3(0.5f,0.5f,0.5f) },
        { Vector3(-0.5f,0.5f,0.5f)},
        { Vector3(-0.5f,-0.5f,0.5f) }
    };

    Vector2 texcoord_list[] =
    {
        { Vector2(0.0f,0.0f) },
        { Vector2(0.0f,1.0f) },
        { Vector2(1.0f,0.0f) },
        { Vector2(1.0f,1.0f) }
    };

    Vertex vertex_list[] = 
    {
        //X - Y - Z
        //FRONT FACE
        { position_list[0],texcoord_list[1] },
        { position_list[1],texcoord_list[0] },
        { position_list[2],texcoord_list[2] },
        { position_list[3],texcoord_list[3] },


        { position_list[4],texcoord_list[1] },
        { position_list[5],texcoord_list[0] },
        { position_list[6],texcoord_list[2] },
        { position_list[7],texcoord_list[3] },


        { position_list[1],texcoord_list[1] },
        { position_list[6],texcoord_list[0] },
        { position_list[5],texcoord_list[2] },
        { position_list[2],texcoord_list[3] },

        { position_list[7],texcoord_list[1] },
        { position_list[0],texcoord_list[0] },
        { position_list[3],texcoord_list[2] },
        { position_list[4],texcoord_list[3] },

        { position_list[3],texcoord_list[1] },
        { position_list[2],texcoord_list[0] },
        { position_list[5],texcoord_list[2] },
        { position_list[4],texcoord_list[3] },

        { position_list[7],texcoord_list[1] },
        { position_list[6],texcoord_list[0] },
        { position_list[1],texcoord_list[2] },
        { position_list[0],texcoord_list[3] }
    };

    UINT listSize = ARRAYSIZE(vertex_list);

    unsigned int index_list[]=
    {
        //FRONT SIDE
        0,1,2,  //FIRST TRIANGLE
        2,3,0,  //SECOND TRIANGLE
        //BACK SIDE
        4,5,6,
        6,7,4,
        //TOP SIDE
        8,9,10,
        10,11,8,
        //BOTTOM SIDE
        12,13,14,
        14,15,12,
        //RIGHT SIDE
        16,17,18,
        18,19,16,
        //LEFT SIDE
        20,21,22,
        22,23,20
    };

    UINT indexListSize = ARRAYSIZE(index_list);
    m_indexBuffer = GraphicsEngine::Get()->GetRenderer()->CreateIndexBuffer(index_list, indexListSize);

    void* shaderByteCode = nullptr;
    size_t shaderSize = 0;

    // Vertex Shader/Buffer compilation & creation
    GraphicsEngine::Get()->GetRenderer()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
    m_vs = GraphicsEngine::Get()->GetRenderer()->CreateVertexShader(shaderByteCode, shaderSize);
    m_vertexBuffer = GraphicsEngine::Get()->GetRenderer()->CreateVertexBuffer(vertex_list, sizeof(Vertex), listSize, shaderByteCode, shaderSize);
    GraphicsEngine::Get()->GetRenderer()->ReleaseCompiledShader();

    // Pixel Shader compilation & creation
    GraphicsEngine::Get()->GetRenderer()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
    m_ps = GraphicsEngine::Get()->GetRenderer()->CreatePixelShader(shaderByteCode, shaderSize);
    GraphicsEngine::Get()->GetRenderer()->ReleaseCompiledShader();

    Constant cc;
    cc.Angle = 0.0f;
    
    // Constant buffer allocation
    m_cb = GraphicsEngine::Get()->GetRenderer()->CreateConstantBuffer(&cc, sizeof(Constant));

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
    ImGui_ImplDX11_Init(GraphicsEngine::Get()->GetRenderer()->GetDevice(), GraphicsEngine::Get()->GetRenderer()->GetDeviceContext());
}

void AppWindow::OnUpdate()
{
    Window::OnUpdate();
    InputSystem::Get()->Update();
    
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swapChain, 0.0f, 0.3f,0.4f, 1);

    // Set the viewport target in which we draw
    RECT rc = GetClientWindowRect();
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    // Shader constants 
    Update();
    
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetConstantBuffer(m_vs, m_cb);
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetConstantBuffer(m_ps, m_cb);

    // Set the shaders
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetVertexShader(m_vs);
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetPixelShader(m_ps);

    // Temp wood texture
    if(m_woodTexture)
        GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetTexture(m_ps, m_woodTexture);

    // Set the vertices of the triangle to draw
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetVertexBuffer(m_vertexBuffer);
    // Set the indices of the triangle to draw
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetIndexBuffer(m_indexBuffer);

    // finally draw
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->DrawIndexedTriangleList(m_indexBuffer->GetIndexListSize(), 0, 0);

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
    GraphicsEngine::Get()->GetRenderer()->GetDeviceContext()->OMSetRenderTargets(1, m_swapChain->GetRTVLValue(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_swapChain->Present(0); // Present with vsync
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
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
