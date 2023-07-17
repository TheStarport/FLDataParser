#pragma once

#include <array>
#include <format>
#include <fstream>

#include "Structures/INI/BiniStringBlock.hpp"
#include "Structures/INI/IniSection.hpp"

#include "IniSerializer.hpp"

namespace Ini
{
    class Reader final
    {
            static constexpr char BINI[] = { 'B', 'I', 'N', 'I' };

            std::string fileName;
            bool throwOnIniWarning = false;
            std::optional<BiniStringBlock> block;
            std::optional<std::string> iniData;
            size_t length = 0;

            static int ParseEquals(std::string_view line, std::array<std::string_view, 3>& parts, bool allowMaps);
            std::vector<IniSection> ParseBini(std::fstream& stream);
            [[nodiscard]]
            std::vector<IniSection> ParseIni(const std::string_view& ini, bool allowMaps) const;
            void Reset();

        public:
            explicit Reader();

            template <typename T>
            std::shared_ptr<T> Parse(const std::string& file, const bool allowMaps = false)
            {
                fileName = file;
                std::fstream stream;
                stream.open(file, std::ios::binary | std::ios::in);
                if (!stream.is_open())
                {
                    throw std::ios::failure(std::format("Unable to open stream to file: {}", file));
                }

                stream.ignore(std::numeric_limits<std::streamsize>::max());
                length = stream.gcount();
                stream.clear();
                stream.seekg(0, std::ios::beg);

                std::array<char, 4> first4Bytes;
                stream.read(first4Bytes.data(), 4);

                std::vector<IniSection> sections;

                if (std::equal(first4Bytes.begin(), first4Bytes.end(), BINI))
                {
                    sections = ParseBini(stream);
                }
                else
                {
                    iniData = std::string(length, '\0');
                    stream.read(iniData.value().data(), length);
                    sections = ParseIni(iniData.value(), allowMaps);
                }

                return std::make_shared<T>(IniSerializer::Deserialize<T>(sections));
            }

            template <typename T>
            std::shared_ptr<T> ParseString(const std::string_view& data, const bool allowMaps = false)
            {
                const auto sections = ParseIni(data, allowMaps);
                return IniSerializer::Deserialize<T>(sections);
            }

            void ThrowOnIniWarning(bool doThrow);
    };
} // namespace Ini
