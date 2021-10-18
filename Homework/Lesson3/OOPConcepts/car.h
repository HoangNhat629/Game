#pragma once

#include "dynamic_object.h"

class car : public dynamic_object
{
public:
    explicit car(std::string name) : dynamic_object(std::move(name))
    {
    }

    void move() override;
};
