#include "AppWindow.h"

#include "../GameEngine/GraphicsEngine/Renderer/DeviceContext/DeviceContext.h"
#include "../GameEngine/GraphicsEngine/Renderer/SwapChain/SwapChain.h"
#include <Windows.h>

#include "../GameEngine/GraphicsEngine/ResourceManager/MeshManager/MeshManager.h"
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
    Vector4 m_lightDirection;
    Vector4 m_cameraPosition;

    float m_ambiantReflection; // default 0.1f
    Vector3 m_ambiantLightColor; // default 1.0f 1.0f 1.0f
    float m_diffuseLight; // default 0.7f
    float m_specularLight; // default 1.0f
    float m_shininess; // default 30.0f
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

// Inputs ImGui exposed vars
static float MouseSensivity = 3.0f;
static float MoveSpeed = 0.5f;
static float StrafeMoveSpeed = 0.5f;
static float InputsDownScalar = 0.03f;

// Lighting ImGui exposed vars
static float LightRotation = 0.0f;
static float AmbiantReflection = 0.1f;
static ImVec4 ImAmbiantLightColor = ImVec4(1.0, 1.0, 1.0, 1.0);
static float DiffuseLight = 0.7f;
static float SpecularLight = 1.0f;
static float Shininess = 30.0f;

void AppWindow::UpdateShaderConstants()
{
    Constant cc;
    cc.m_ambiantReflection = AmbiantReflection;
    cc.m_ambiantLightColor = Vector3(ImAmbiantLightColor.x, ImAmbiantLightColor.y, ImAmbiantLightColor.z);
    cc.m_diffuseLight = DiffuseLight;
    cc.m_specularLight = SpecularLight;
    cc.m_shininess = Shininess;
    
    // Transformation matrices
    cc.m_world.SetIdentity();

    Matrix4x4 worldCam;
    Matrix4x4 temp;

    Matrix4x4 lightRotationMatrix;
    lightRotationMatrix.SetIdentity();
    lightRotationMatrix.SetRotationY(LightRotation);
    cc.m_lightDirection = lightRotationMatrix.GetZDirection();

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
    cc.m_cameraPosition = newPos;
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

void AppWindow::Render()
{
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->ClearRenderTargetColor(m_swapChain, 0.0f, 0.3f,0.4f, 1);

    // Set the viewport target in which we draw
    RECT rc = GetClientWindowRect();
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    // Shader constants 
    UpdateShaderConstants();
    
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetConstantBuffer(m_vs, m_cb);
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetConstantBuffer(m_ps, m_cb);

    // Set the shaders
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetVertexShader(m_vs);
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetPixelShader(m_ps);

    // Temp wood texture
    if(m_woodTexture)
        GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetTexture(m_ps, m_woodTexture);

    // Set the vertices of the triangle to draw
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetVertexBuffer(m_mesh->GetVertexBuffer());
    // Set the indices of the triangle to draw
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->SetIndexBuffer(m_mesh->GetIndexBuffer());

    // finally draw
    GraphicsEngine::Get()->GetRenderer()->GetImmediateDeviceContext()->DrawIndexedTriangleList(m_mesh->GetIndexBuffer()->GetIndexListSize(), 0, 0);

    RenderUI();
    
    m_swapChain->Present(true);
}

void AppWindow::RenderUI()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    { // My imgui test window
        ImGui::Begin("Movements");                  
        ImGui::SliderFloat("Sensivity", &MouseSensivity, 0.1f, 10.0f);            
        ImGui::SliderFloat("MoveSpeed", &MoveSpeed, 0.1f, 10.0f);            
        ImGui::SliderFloat("StrafeMoveSpeed", &StrafeMoveSpeed, 0.1f, 10.0f);            
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    { // ImGui Lighting settings window
        ImGui::Begin("Lighting");
        ImGui::SliderFloat("Rotation", &LightRotation, 0.0f, 6.0f);
        ImGui::SliderFloat("Ambiant", &AmbiantReflection, 0.0f, 1.0f);
        ImGui::ColorEdit3("AmbiantColor", (float*)&ImAmbiantLightColor);
        ImGui::SliderFloat("Diffuse", &DiffuseLight, 0.0f, 5.0f);
        ImGui::SliderFloat("Specular", &SpecularLight, 0.0f, 5.0f);
        ImGui::SliderFloat("Shininess", &Shininess, 0.0f, 100.0f);
        if(ImGui::Button("Reset Settings"))
        {
            LightRotation = 0.0f;
            AmbiantReflection = 0.1f;
            ImAmbiantLightColor = ImVec4(1.0, 1.0, 1.0, 1.0);
            DiffuseLight = 0.7f;
            SpecularLight = 1.0f;
            Shininess = 30.0f;
        }
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    GraphicsEngine::Get()->GetRenderer()->GetDeviceContext()->OMSetRenderTargets(1, m_swapChain->GetRTVLValue(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    //m_swapChain->Present(0); // Present with vsync
}

void AppWindow::OnCreate()
{
    Window::OnCreate();
    
    InputSystem::Get()->AddListener(this);
    InputSystem::Get()->ShowCursor(false);

    m_woodTexture = GraphicsEngine::Get()->GetTextureMgr()->CreateTextureFromFile(L"Assets\\Textures\\brick.png");
    m_mesh = GraphicsEngine::Get()->GetMeshMgr()->CreateMeshFromFile(L"Assets\\Meshes\\statue.obj");
    
    RECT rc = GetClientWindowRect();
    m_swapChain = GraphicsEngine::Get()->GetRenderer()->CreateSwapChain(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

    // Cam spawnloc 
    m_worldCam.SetTranslation(Vector3(0.0f, 0.0f, -1.0f));

    void* shaderByteCode = nullptr;
    size_t shaderSize = 0;

    // Vertex Shader/Buffer compilation & creation
    GraphicsEngine::Get()->GetRenderer()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
    m_vs = GraphicsEngine::Get()->GetRenderer()->CreateVertexShader(shaderByteCode, shaderSize);
    GraphicsEngine::Get()->GetRenderer()->ReleaseCompiledShader();

    // Pixel Shader compilation & creation
    GraphicsEngine::Get()->GetRenderer()->CompilePixelShader(L"PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
    m_ps = GraphicsEngine::Get()->GetRenderer()->CreatePixelShader(shaderByteCode, shaderSize);
    GraphicsEngine::Get()->GetRenderer()->ReleaseCompiledShader();

    Constant cc;
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
    Render();
    
    // time
    m_oldDeltaTime = m_newDeltaTime;
    m_newDeltaTime = GetTickCount();
    m_deltaTime = m_oldDeltaTime ? (m_newDeltaTime - m_oldDeltaTime) / 1000.0f : 0;
    m_totalTime += m_deltaTime;
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    m_swapChain->SetFullscreen(false, 1, 1);
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

void AppWindow::OnResize()
{
    RECT rc = GetClientWindowRect();
    m_swapChain->Resize(rc.right, rc.bottom);
    Render();
    RenderUI();
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
}

void AppWindow::OnKeyUp(int key)
{
    m_camForward = 0.0f;
    m_camRight = 0.0f;

    if(key == 'E')
    {
        m_isMouseLocked = m_isMouseLocked ? false : true;
        InputSystem::Get()->ShowCursor(!m_isMouseLocked);
    }
    else if(key == 'F')
    {
        m_isFullscreen = m_isFullscreen ? false : true;
        RECT screenSize = GetScreenSize();
        m_swapChain->SetFullscreen(m_isFullscreen, screenSize.right, screenSize.bottom);
    }
}

void AppWindow::OnMouseMove(const Point& mousePosition)
{
    if(!m_isMouseLocked)
        return;
    
    int width = GetClientWindowRect().right - GetClientWindowRect().left;
    int height = GetClientWindowRect().bottom - GetClientWindowRect().top;

    m_rotationX += (mousePosition.Y - (height / 2.0f)) * m_deltaTime * (MouseSensivity * InputsDownScalar);
    m_rotationY += (mousePosition.X - (width / 2.0f)) * m_deltaTime * (MouseSensivity * InputsDownScalar);

    InputSystem::Get()->SetCursorPosition(Point(width/2.0f, height/2.0f));
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
