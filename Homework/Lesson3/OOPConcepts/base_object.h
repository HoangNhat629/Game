#pragma once

#include "vector2.h"

#include <stdint.h>
#include <string>

class base_object
{
public:
    explicit base_object(std::string name);
    virtual ~base_object() = default;

    const std::string& get_name() const;
    uint64_t get_id() const;

    void set_position(vector2 position);

    const vector2& get_position() const;

    virtual void move() = 0;

protected:
    vector2 m_position;

private:
    uint64_t m_id;
    std::string m_name;

    static uint64_t s_globalObjectIDCounter;
};
