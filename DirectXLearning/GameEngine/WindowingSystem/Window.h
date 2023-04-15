#pragma once

#include <Windows.h>

class Window
{
public:
    Window();
    ~Window();
    
    bool Init();
    bool Release();

    bool BroadCast();
    bool IsRunning();

    virtual void OnCreate();
    virtual void OnUpdate();
    virtual void OnDestroy();

    virtual void OnFocus();
    virtual void OnLooseFocus();

    RECT GetClientWindowRect();
    void SetHWND(HWND hwnd);

protected:
    HWND m_hwnd;
    bool m_isRunning;
};
