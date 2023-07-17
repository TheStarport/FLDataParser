#include <fstream>

#include "Exceptions/BiniParsingException.hpp"
#include "Structures/INI/BiniStringBlock.hpp"
#include "Structures/INI/IniEntry.hpp"
#include "Structures/INI/IniValueType.hpp"

namespace Ini
{
    IniEntry::IniEntry(const std::string_view file, std::fstream& stream, BiniStringBlock& block)
    {
        short nameOffset;
        stream.read(reinterpret_cast<char*>(&nameOffset), sizeof(short));
        name = block.Get(nameOffset);

        std::byte count;
        stream.read(reinterpret_cast<char*>(&count), sizeof(std::byte));
        values.reserve(static_cast<size_t>(count));

        for (int i = 0; i < static_cast<int>(count); i++)
        {
            IniValueType type;
            stream.read(reinterpret_cast<char*>(&type), sizeof(std::byte));

            switch (type)
            {
                case IniValueType::Boolean:
                    char boolean;
                    stream.read(&boolean, sizeof(char));
                    values.emplace_back(boolean > 0);
                    break;
                case IniValueType::Int32:
                    int int32;
                    stream.read(reinterpret_cast<char*>(&int32), sizeof(int));
                    values.emplace_back(int32);
                    break;
                case IniValueType::Single:
                    float single;
                    stream.read(reinterpret_cast<char*>(&single), sizeof(float));
                    values.emplace_back(single);
                    break;
                case IniValueType::String:
                    int offset;
                    stream.read(reinterpret_cast<char*>(&offset), sizeof(int));
                    values.emplace_back(block.Get(offset));
                    break;
                default: throw BiniParsingException(file);
            }
        }
    }

    IniEntry::IniEntry(const std::string_view name, std::vector<PossibleValueType>& values) : values(std::move(values)), name(name) {}
} // namespace Ini
