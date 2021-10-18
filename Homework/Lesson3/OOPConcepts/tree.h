#pragma once

#include "static_object.h"

class tree : public static_object
{
public:
    explicit tree(std::string name) : static_object(std::move(name))
    {
    }
};