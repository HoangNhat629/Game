#pragma once

#include "base_object.h"

class dynamic_object : public base_object
{
public:
    explicit dynamic_object(std::string name) : base_object(std::move(name))
    {
    }
};