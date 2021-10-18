#include "base_object.h"
#include "tree.h"
#include "house.h"
#include "motorbike.h"
#include "car.h"

#include <vector>
#include <memory>

#include <stdio.h>

using obj_list_t = std::vector<std::unique_ptr<base_object>>;

void print_objects(const obj_list_t& objList)
{
    for (const auto& obj : objList)
    {
        printf("[%-10s (#%lld)]\tPosition: (%.3f, %.3f)\n", obj->get_name().c_str(), obj->get_id(), obj->get_position().x, obj->get_position().y);
    }
}

int main()
{
    obj_list_t objList;
    objList.reserve(4);

    objList.emplace_back(std::make_unique<tree>("Tree1"));
    objList.emplace_back(std::make_unique<house>("House1"));
    objList.emplace_back(std::make_unique<motorbike>("Motorbike1"));
    objList.emplace_back(std::make_unique<car>("Car1"));

    printf_s("List of objects before moving:\n");
    print_objects(objList);
    printf_s("\n");

    printf_s("Attempt to move objects...\n");
    for (auto& obj : objList)
    {
        obj->move();
    }
    printf_s("\n");

    printf_s("List of objects after moving:\n");
    print_objects(objList);

    return 0;
}