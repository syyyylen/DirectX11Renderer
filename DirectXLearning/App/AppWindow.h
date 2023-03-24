#pragma once
#include "../Engine/Window.h"

class AppWindow : public Window
{
public:
    AppWindow();
    ~AppWindow();

    virtual void OnCreate() override;
    virtual void OnUpdate() override;
    virtual void OnDestroy() override;
};
