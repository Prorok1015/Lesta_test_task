
#include "lib/pugixml.hpp"
#include "Object.h"

#include "Attribute.h"

void Object::load(const pugi::xml_node& node)
{
    for (const auto& attribute : node.attributes()) {
        if (wcscmp(attribute.name(), L"Name") == 0) name_ = attribute.value();
        else if (wcscmp(attribute.name(), L"Image") == 0) img_ = attribute.value();
    }

    for (const auto& child : node) {
        Attribute atr; atr.load(child);
        attrs_[atr.name_] = std::move(atr);
    }
}

void Object::save(pugi::xml_node& node) const
{
    auto child = node.append_child(L"Object");
    child.append_attribute(L"Name") = name_.c_str();
    child.append_attribute(L"Image") = img_.c_str();

    for (const auto& [key, at] : attrs_) {
        at.save(child);
    }
}