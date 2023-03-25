#include "AppWindow.h"

#include "../GameEngine/GraphicsEngine/SwapChain/SwapChain.h"

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
}

void AppWindow::OnUpdate()
{
    
}

void AppWindow::OnDestroy()
{
    Window::OnDestroy();
    m_swapChain->Release();
    GraphicsEngine::Get()->Release();
}
