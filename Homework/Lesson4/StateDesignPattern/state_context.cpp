#include "state_context.h"

#include "state_base.h"

#include <stdio.h>

state_context::state_context(std::unique_ptr<state_base> initialState) : m_state(std::move(initialState))
{
}

void state_context::apply_action(door_action action)
{
    m_state->apply_action(this, action);
}

void state_context::set_state(std::unique_ptr<state_base> newState)
{
    m_state = std::move(newState);

    printf_s("Transition to new state: %s\n", m_state->description());
}
