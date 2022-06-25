#pragma once
#include <string>
enum class Type { Bool = 1, Float, Int };

extern std::wstring from_type(Type type);
Type to_type(std::wstring_view name);