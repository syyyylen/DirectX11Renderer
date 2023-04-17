#pragma once

#include <Windows.h>

class Window
{
public:
    Window();
    ~Window();
    
    bool IsRunning();

    virtual void OnCreate();
    virtual void OnUpdate();
    virtual void OnDestroy();

    virtual void OnFocus();
    virtual void OnLooseFocus();

    virtual void OnResize();

    RECT GetClientWindowRect();
    RECT GetScreenSize();

private:
    bool BroadCast();

protected:
    HWND m_hwnd;
    bool m_isRunning;
    bool m_isInit;
};
