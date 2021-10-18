#pragma once

#include "dynamic_object.h"

class motorbike : public dynamic_object
{
public:
    explicit motorbike(std::string name) : dynamic_object(std::move(name))
    {
    }

    void move() override;
};
