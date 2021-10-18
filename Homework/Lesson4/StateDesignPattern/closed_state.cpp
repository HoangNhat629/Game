#include "closed_state.h"

#include "locked_state.h"
#include "opened_state.h"

void closed_state::apply_action(state_context* context, door_action action)
{
    if (action == door_action::Lock)
    {
        context->set_state(std::make_unique<locked_state>());
    }
    else if (action == door_action::Open)
    {
        context->set_state(std::make_unique<opened_state>());
    }
}
