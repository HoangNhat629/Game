#pragma once

#include "static_object.h"

class house : public static_object
{
public:
    explicit house(std::string name) : static_object(std::move(name))
    {
    }
};
