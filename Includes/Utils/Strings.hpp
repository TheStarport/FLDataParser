#pragma once

#include <algorithm>
#include <ranges>
#include <string>

class Strings
{
    public:
        Strings() = delete;

        static std::string Trim(std::string str)
        {
            const auto typeOfWhitespaces = " \t\n\r\f\v";
            str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
            str.erase(0, str.find_first_not_of(typeOfWhitespaces));
            return str;
        }

        static void TrimView(std::string_view& str)
        {
            constexpr std::array<char, 6> characters = { ' ', '\t', '\r', '\n', '\f', '\v' };
            while (std::ranges::any_of(characters, [str](const char character) { return str.starts_with(character); }))
            {
                str.remove_prefix(1);
            }

            while (std::ranges::any_of(characters, [str](const char character) { return str.ends_with(character); }))
            {
                str.remove_suffix(1);
            }
        }

        static bool CompareInsensitive(const std::string_view& str1, const std::string_view& str2)
        {
            return std::ranges::equal(str1, str2, [](const char& a, const char& b) { return (std::tolower(a) == std::tolower(b)); });
        }

        static std::vector<std::string_view> Split(const std::string_view str, const char delimiter = ',')
        {
            std::vector<std::string_view> result;

            int indexCommaToRightOfColumn = -1;

            for (int i = 0; i < static_cast<int>(str.size()); i++)
            {
                if (str[i] == delimiter)
                {
                    const int indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
                    indexCommaToRightOfColumn = i;
                    const int index = indexCommaToLeftOfColumn + 1;
                    const int length = indexCommaToRightOfColumn - index;

                    std::string_view column(str.data() + index, length);
                    result.push_back(column);
                }
            }
            const std::string_view finalColumn(str.data() + indexCommaToRightOfColumn + 1, str.size() - indexCommaToRightOfColumn - 1);
            result.push_back(finalColumn);
            return result;
        }
};
