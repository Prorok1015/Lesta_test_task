#pragma once
#include <string>
#include <unordered_map>
#include "Attribute.h"
#include "lib/pugixml.hpp"

using MyImage = std::wstring;
struct Object
{
    std::wstring name_;
    MyImage img_;
    std::unordered_map<std::wstring, Attribute> attrs_;
    auto operator <=>(const Object&) const = default;
    bool operator== (const std::wstring& str) const { return name_ == str; }
    void load(const pugi::xml_node& node);
    void save(pugi::xml_node& node) const;
};