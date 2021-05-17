#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <iostream>

class Object
{
public:
    const std::vector<std::string> tags;
    const std::string description;
    const std::string details;

    int weight, capacity, count = 0;

    Object* parent = nullptr;
    Object* inventoryHead = nullptr;
    Object* next = nullptr;

    Object* destination = nullptr;

    Object();
    Object(std::vector<std::string> tags, std::string description, std::string details, int weight, int capacity, int count);

    ~Object();

    void AppendInventory(Object& obj);
};

#endif