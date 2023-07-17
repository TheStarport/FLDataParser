#pragma once

#include <format>

class BiniParsingException : std::exception
{
        std::string_view fileName;

    public:
        BiniParsingException(const std::string_view& fileName) : fileName(fileName) {}

        [[nodiscard]]
        const char* what() const override
        {
            return std::format("Unable to parse BINI file: {}", fileName).c_str();
        }
};
