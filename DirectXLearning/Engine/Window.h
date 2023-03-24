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

    virtual void OnCreate()=0;
    virtual void OnUpdate()=0;
    virtual void OnDestroy();

protected:
    HWND m_hwnd;
    bool m_isRunning;
};
