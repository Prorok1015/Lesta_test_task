#pragma once

#include <unordered_map>
#include "Object.h"

class Model
{
    std::unordered_map<std::wstring, Object> objects;
public:
    bool open(std::wstring path);
    bool save(std::wstring path );

    Object& insert(Object obj) {
        return objects[obj.name_] = std::move(obj);
    }

    void pop(std::wstring name) {
        objects.erase(name);
    }

    Object* find(std::wstring name);

    std::unordered_map<std::wstring, Object>& get_all()
    {
        return objects;
    }
};