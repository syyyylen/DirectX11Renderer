#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

InputSystem* InputSystem::Get()
{
    static InputSystem system;
    return &system;
}

void InputSystem::Update()
{
    if (::GetKeyboardState(m_keys_state))
    {
        for (unsigned int i = 0; i < 256; i++)
        {
            // KEY IS DOWN
            if (m_keys_state[i] & 0x80)
            {
                std::unordered_set<InputListener*>::iterator it = m_listenersSet.begin();

                while (it != m_listenersSet.end())
                {
                    (*it)->OnKeyDown(i);
                    ++it;
                }
            }
            else // KEY IS UP
                {
                if (m_keys_state[i] != m_old_keys_state[i])
                {
                    std::unordered_set<InputListener*>::iterator it = m_listenersSet.begin();

                    while (it != m_listenersSet.end())
                    {
                        (*it)->OnKeyUp(i);
                        ++it;
                    }
                }

                }

        }
        // store current keys state to old keys state buffer
        ::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
    }
}

void InputSystem::AddListener(InputListener* Listener)
{
    m_listenersSet.insert(Listener);
}

void InputSystem::RemoveListener(InputListener* Listener)
{
    m_listenersSet.erase(Listener);
}
