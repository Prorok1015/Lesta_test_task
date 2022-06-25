#pragma once
#include "Type.h"
#include "lib/pugixml.hpp"
struct Attribute
{
    std::wstring name_;
    std::wstring value_;
    Type type_;
    auto operator <=>(const Attribute&) const = default;
    bool operator== (std::wstring name) const { return name_ == name; }
    Attribute& load(const pugi::xml_node& node);
    void save(pugi::xml_node& node) const;
};