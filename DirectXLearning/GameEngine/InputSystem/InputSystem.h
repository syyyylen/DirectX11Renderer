#pragma once

#include "InputListener.h"
#include <map>
#include <unordered_set>

class InputSystem
{
public:
    InputSystem();
    ~InputSystem();

    static InputSystem* Get();

    void Update();
    void AddListener(InputListener* Listener);
    void RemoveListener(InputListener* Listener);

private:
    std::unordered_set<InputListener*> m_listenersSet;
    unsigned char m_keys_state[256] = {};
    unsigned char m_old_keys_state[256] = {};
};
