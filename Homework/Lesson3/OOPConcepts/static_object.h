#pragma once

#include "base_object.h"

class static_object : public base_object
{
public:
    explicit static_object(std::string name) : base_object(std::move(name))
    {
    }

    void move() override;
};