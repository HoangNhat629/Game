#include "car.h"

void car::move()
{
    constexpr float speed = 5.0f;
    constexpr vector2 direction = vector2(1.0f, 1.0f);

    m_position = m_position + direction * speed;
}