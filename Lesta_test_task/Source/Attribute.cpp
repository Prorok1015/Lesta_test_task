#include "lib/pugixml.hpp"
#include "Attribute.h"

Attribute& Attribute::load(const pugi::xml_node& node)
{
    for (const auto& attribute : node.attributes()) {
        if (wcscmp(attribute.name(), L"Name") == 0) name_ = attribute.value();
        else if (wcscmp(attribute.name(), L"Value") == 0) value_ = attribute.value();
        else if (wcscmp(attribute.name(), L"Type") == 0) type_ = to_type(attribute.value());
    }
    return *this;
}

void Attribute::save(pugi::xml_node& node) const
{
    auto child = node.append_child(L"Attribute");
    child.append_attribute(L"Name") = name_.c_str();
    child.append_attribute(L"Value") = value_.c_str();
    child.append_attribute(L"Type") = from_type(type_).c_str();
}