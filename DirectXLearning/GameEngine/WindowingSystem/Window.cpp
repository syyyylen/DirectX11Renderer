#include "Window.h"

#include "../GraphicsEngine/DeviceContext/DeviceContext.h"

//Window* window = nullptr;

Window::Window()
{
}

Window::~Window()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
            window->SetHWND(hwnd);
            window->OnCreate();
            break;
        }

        case WM_DESTROY:
        {
            Window* window = (Window*)GetWindowLong(hwnd, GWLP_USERDATA);
            if(window)
                window->OnDestroy();
            ::PostQuitMessage(0);
            break;
        }

        default:
            return ::DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

bool Window::Init()
{
    // Window properties
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"MyWindowClass";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

   if(!::RegisterClassExW(&wc))
       return false;

    // if(!window)
    //     window = this;

    // Creation of the window 
    m_hwnd = ::CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectXLearning", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);

    if(!m_hwnd)
        return false;

    // Show up the window
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // Set this flag to true to indicate that the window is running
    m_isRunning = true;
    
    return true;
}

bool Window::Release()
{
    if(!::DestroyWindow(m_hwnd))
        return false;

    return true;
}

bool Window::BroadCast()
{
    MSG msg;

    OnUpdate();
    
    while(::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    Sleep(1);
    
    return true;
}

bool Window::IsRunning()
{
    return m_isRunning;
}

void Window::OnCreate()
{
    
}

void Window::OnUpdate()
{
    
}

void Window::OnDestroy()
{
    m_isRunning = false;
}

RECT Window::GetClientWindowRect()
{
    RECT rc;
    ::GetClientRect(m_hwnd, &rc);
    return rc;
}

void Window::SetHWND(HWND hwnd)
{
    m_hwnd = hwnd;
}
