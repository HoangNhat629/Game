#include "opened_state.h"

#include "closed_state.h"

void opened_state::apply_action(state_context* context, door_action action)
{
    if (action == door_action::Close)
    {
        context->set_state(std::make_unique<closed_state>());
    }
}
