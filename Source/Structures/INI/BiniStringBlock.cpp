#include "Structures/INI/BiniStringBlock.hpp"

namespace Ini
{
    BiniStringBlock::BiniStringBlock(std::string& block) : block(std::move(block)) {}

    std::string_view BiniStringBlock::Get(const int offset)
    {
        const auto str = strings.find(offset);
        if (str == strings.end())
        {
            const std::string_view blockView = block;
            const std::string_view view = blockView.substr(offset, blockView.find('\0', offset) - offset);
            strings[offset] = view;
            return view;
        }

        return str->second;
    }
} // namespace Ini
