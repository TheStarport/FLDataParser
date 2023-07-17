#pragma once

#include <map>
#include <string>

namespace Ini
{
    class BiniStringBlock
    {
            std::string block;
            std::map<int, std::string_view> strings;

        public:
            explicit BiniStringBlock(std::string& block);
            std::string_view Get(int offset);
    };
} // namespace Ini
