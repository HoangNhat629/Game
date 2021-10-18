#pragma once

#include "door_action.h"

#include <memory>

class state_base;

class state_context
{
public:
    state_context(std::unique_ptr<state_base> initialState);

    void apply_action(door_action action);

    void set_state(std::unique_ptr<state_base> newState);

private:
    std::unique_ptr<state_base> m_state;
};
