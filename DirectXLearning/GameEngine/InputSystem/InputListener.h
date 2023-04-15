﻿#pragma once

class InputListener
{
public:
    InputListener()
    {
        
    }

    ~InputListener()
    {
        
    }

    // Keyboard pure virtual callbacks
    virtual void OnKeyDown(int key) = 0;
    virtual void OnKeyUp(int key) = 0;
};