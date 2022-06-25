#pragma once
#include "Model.h"
#include <vector>

class Controller
{
    Model model;
public:
    const std::unordered_map<std::wstring, Object>& get_all();

    Object* find(std::wstring name) { return model.find(name); };

    void save(std::wstring path = L"file.xml") {
        model.save(path);
    }

    void open(std::wstring path = L"file.xml")
    {
        model.open(path);
    }

    void push(Object obj) {
        model.insert(std::move(obj));
    }

    void pop(std::wstring name) {
        model.pop(name);
    }

    void edit(std::wstring name, Object& new_obj);

    void add_attr(std::wstring name, Attribute& attr);

    void edit_attr(std::wstring object_name, std::wstring name, Attribute& attr)
    {
        static size_t i = 0;
        auto obj = model.find(object_name);
        if (!obj) return;
        obj->attrs_.erase(name);
        if (obj->attrs_.find(attr.name_) != obj->attrs_.end())
        {
            attr.name_ += std::to_wstring(i++);
        }
        obj->attrs_[attr.name_] = attr;
        
    }
};