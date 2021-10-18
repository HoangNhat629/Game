#include "locked_state.h"

#include "closed_state.h"

void locked_state::apply_action(state_context* context, door_action action)
{
    if (action == door_action::Unlock)
    {
        context->set_state(std::make_unique<closed_state>());
    }
}
