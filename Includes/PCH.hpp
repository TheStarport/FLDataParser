#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <ranges>
#include <algorithm>
#include <functional>

#define INI_CALL_HANDLER_ON_NEW_SECTION 1
#define INI_ALLOW_NO_VALUE 1
#define INI_HANDLER_LINENO 1
#define INI_MAX_LINE 1024
#define INI_USE_STACK 0
#define INI_ALLOW_REALLOC 1
#include "Inih.hpp"

bool CompareInsensitive(const std::string& str1, const std::string& str2);