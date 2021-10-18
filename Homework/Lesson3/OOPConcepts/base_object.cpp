#include "base_object.h"

#include <stdio.h>

uint64_t base_object::s_globalObjectIDCounter = 0;

base_object::base_object(std::string name) : m_position{}, m_id(s_globalObjectIDCounter++), m_name(std::move(name))
{
}

const std::string& base_object::get_name() const
{
    return m_name;
}

uint64_t base_object::get_id() const
{
    return m_id;
}

void base_object::set_position(vector2 position)
{
    m_position = position;
}

const vector2& base_object::get_position() const
{
    return m_position;
}