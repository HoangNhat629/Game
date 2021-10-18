#pragma once

#include "KeyCode.h"

#include "GameConfig.h"

#include <stdint.h>
#include <stdio.h>

class InputManager
{
public:
    InputManager() : m_keyHoldingBitSet{}, m_keyDownBitSet{}, m_keyUpBitSet{}, m_mouseButtonPressed(false)
    {
    }

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    void OnTouchEvent(bool pressed)
    {
        m_mouseButtonPressed = pressed;
    }

    void OnKeyEvent(unsigned char key, bool pressed)
    {
        constexpr uint32_t one = (uint32_t)1;
        const int keyShiftAmount = (int)CharToKeyCode(key);
        const uint32_t mask = one << keyShiftAmount;

        if (pressed)
        {
            // Set down state
            //
            //	    ____ 1 ____         m_keyDownBitSet | mask
            // ^    0000 k 0000         m_keyHoldingBitSet & mask
            // =    ____~k ____         m_keyHoldingBitSet with pressed key bit set to the opposite of
            //                          whatever state it was last frame
            //
            m_keyDownBitSet = ((m_keyDownBitSet | mask) ^ (m_keyHoldingBitSet & mask));

            // Set holding state
            m_keyHoldingBitSet |= mask;

            // Clear up state
            m_keyUpBitSet &= ~mask;
        }
        else
        {
            // Clear holding state
            m_keyHoldingBitSet &= ~mask;

            // Clear down state
            m_keyDownBitSet &= ~mask;

            // Set up state
            m_keyUpBitSet |= mask;
        }

    }

    void OnUpdate()
    {
        m_keyDownBitSet = 0;
        m_keyUpBitSet = 0;
    }

    bool Key(KeyCode keyCode) const
    {
        constexpr uint32_t one = (uint32_t)1;
        const int keyShiftAmount = (int)keyCode;
        const uint32_t mask = one << keyShiftAmount;

        return (m_keyHoldingBitSet & mask) != 0;
    }

    bool KeyDown(KeyCode keyCode) const
    {
        constexpr uint32_t one = (uint32_t)1;
        const int keyShiftAmount = (int)keyCode;
        const uint32_t mask = one << keyShiftAmount;

        return (m_keyDownBitSet & mask) != 0;
    }

    bool KeyUp(KeyCode keyCode) const
    {
        constexpr uint32_t one = (uint32_t)1;
        const int keyShiftAmount = (int)keyCode;
        const uint32_t mask = one << keyShiftAmount;

        return (m_keyUpBitSet & mask) != 0;
    }

    bool Touch() const
    {
        return m_mouseButtonPressed;
    }

private:
    constexpr KeyCode CharToKeyCode(unsigned char key)
    {
        switch (key)
        {
        case VK_UP:
            return KeyCode::UP;
        case VK_DOWN:
            return KeyCode::DOWN;
        case VK_LEFT:
            return KeyCode::LEFT;
        case VK_RIGHT:
            return KeyCode::RIGHT;
        case 'W':
            return KeyCode::W;
        case 'A':
            return KeyCode::A;
        case 'S':
            return KeyCode::S;
        case 'D':
            return KeyCode::D;
        case 'E':
            return KeyCode::E;
        case 'R':
            return KeyCode::R;
        case VK_SPACE:
            return KeyCode::SPACE;
        default:
            return KeyCode::UNKNOWN;
        }
    }

private:
    uint32_t m_keyHoldingBitSet;
    uint32_t m_keyDownBitSet;
    uint32_t m_keyUpBitSet;

    bool m_mouseButtonPressed;
};
