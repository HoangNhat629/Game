#include "static_object.h"

#include <stdio.h>

void static_object::move()
{
    printf_s("Object #%llu cannot change position\n", get_id());
}