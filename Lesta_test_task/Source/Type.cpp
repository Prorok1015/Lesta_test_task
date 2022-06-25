
#include "Type.h"
#include <string>

std::wstring from_type(Type type)
{
    std::wstring res;
    switch (type)
    {
    case Type::Bool: res = L"Bool";
        break;
    case Type::Float: res = L"Float";
        break;
    case Type::Int: res = L"Int";
        break;
    }
    return res;
}

Type to_type(std::wstring_view name)
{
    Type type = Type::Int;
    if (name == L"Bool") type = Type::Bool;
    else if (name == L"Float") type = Type::Float;
    else if (name == L"Int") type = Type::Int;
    return type;
}