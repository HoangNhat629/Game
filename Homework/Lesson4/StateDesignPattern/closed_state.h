#pragma once

#include "state_base.h"

class closed_state : public state_base
{
public:
    void apply_action(state_context* context, door_action action) override;
    inline const char* description() const override
    {
        return "Closed";
    }
};