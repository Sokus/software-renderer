#include "object.hpp"

Object::Object() { }

Object::Object(std::vector<std::string> tags, std::string description, std::string details)
    : tags(tags), description(description), details(details)
    {
    }

Object::~Object()
{
    delete inventoryHead;
    delete next;
}

void Object::AppendInventory(Object& obj)
{
    if(this->inventoryHead == nullptr)
    {
        this->inventoryHead = &obj;
    }
    else
    {
        Object* p_inv = this->inventoryHead;
        while(p_inv->next != nullptr) p_inv = p_inv->next;
        p_inv->next = &obj;
    }

    obj.parent = this;
}