#include "Model.h"
#include "lib/pugixml.hpp"
#include "Object.h"

using namespace pugi;
bool Model::open(std::wstring path)
{
    using namespace pugi;
    xml_document doc;
    bool res = false;
    objects.clear();
    if (auto result = doc.load_file(path.c_str());
        result.status == xml_parse_status::status_ok)
    {
        for (const auto& node : doc) {
            if (wcscmp(node.name(), L"Document") == 0) {
                for (const auto& child : node) {
                    Object obj; obj.load(child);
                    insert(std::move(obj));
                }
            }
        }

        res = true;
    }

    return res;
}

bool Model::save(std::wstring path)
{
    using namespace pugi;
    xml_document xml;
    pugi::xml_node decl = xml.append_child(xml_node_type::node_declaration);
    decl.append_attribute(L"version") = L"1.0";
    decl.append_attribute(L"encoding") = L"utf-8";
    auto doc = xml.append_child(L"Document");

    for (const auto& [key, obj] : objects) {
        obj.save(doc);
    }

    xml.save_file(path.c_str());

    return !objects.empty();
}

Object* Model::find(std::wstring name) {
    auto result = objects.find(name);

    if (result == objects.end())
        return nullptr;
    return &(result->second);
}