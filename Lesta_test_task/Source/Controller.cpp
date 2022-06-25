#include "Attribute.h"
#include "Object.h"
#include "Controller.h"
#include <string>

void Controller::edit(std::wstring name, Object& new_obj)
{
    static size_t i = 0;
    auto obj = model.find(name);
    model.pop(name);
    if (model.find(new_obj.name_))
        new_obj.name_ += std::to_wstring(i++);
    model.insert(new_obj);
}

void Controller::add_attr(std::wstring name, Attribute& attr)
{
    auto obj = model.find(name);
    if (!obj) return;
    obj->attrs_[attr.name_] = attr;
}

const std::unordered_map<std::wstring, Object>& Controller::get_all()
{
    return model.get_all();
}