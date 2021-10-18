#pragma once

#include "state_context.h"
#include "door_action.h"

class state_base
{
public:
    virtual ~state_base() = default;

    virtual void apply_action(state_context* context, door_action action) = 0;
    virtual const char* description() const = 0;
};
