#include "Reader.hpp"

#include "Exceptions/BiniParsingException.hpp"
#include "Exceptions/IniParsingException.hpp"
#include "Utils/Strings.hpp"

namespace Ini
{
	int Reader::ParseEquals(std::string_view line, std::array<std::string_view, 3>& parts, const bool allowMaps)
	{
		auto idx0 = line.find('=');
		if (idx0 == std::string::npos)
		{
			parts[0] = line;
			return 1;
		}

		parts[0] = line.substr(0, idx0);

		if (idx0 + 1 >= line.length())
		{
			return 1;
		}

		if (const auto idx1 = line.find('=', idx0 + 1); idx1 != std::string::npos && !allowMaps)
		{
			//Skip duplicate equals
			for (int i = idx0; i < line.length(); i++)
			{
				if (line[i] != '=' && line[i] != ' ' && line[i] != '\t')
				{
					idx0 = i - 1;
					break;
				}
			}
		}
		else if (idx1 != std::string::npos)
		{
			parts[1] = line.substr(idx0 + 1, idx1 - (idx0 + 1));
			parts[2] = line.substr(idx1 + 1);
			return 3;
		}

		parts[1] = line.substr(idx0 + 1);
		return 2;
	}

	// BINI files should always use version 1 of the format, as there was only ever a single version of the spec
	constexpr int FormatVersion = 1;
	std::vector<IniSection> Reader::ParseBini(std::fstream& stream)
	{
		int formatVersion;
		stream.read(reinterpret_cast<char*>(&formatVersion), sizeof(int));

		if (formatVersion != FormatVersion)
		{
			throw BiniParsingException(fileName);
		}

		size_t stringBlockOffset;
		stream.read(reinterpret_cast<char*>(&stringBlockOffset), sizeof(int));

		if (stringBlockOffset > length)
		{
			throw std::out_of_range(std::format("BINI string block was out of range: {}", fileName));
		}

		const auto sectionBlockOffset = stream.tellg();

		stream.seekg(stringBlockOffset, std::ios::beg);
		std::string data(length - stringBlockOffset, ' ');
		stream.read(data.data(), data.length());

		block = BiniStringBlock(data);

		std::vector<IniSection> sections;
		stream.seekg(sectionBlockOffset, std::ios::beg);
		while (stream.tellg() < stringBlockOffset)
		{
			sections.emplace_back(fileName, stream, block.value());
		}

		return sections;
	}

	std::vector<IniSection> Reader::ParseIni(const std::string_view& ini, bool allowMaps) const
	{
		int currentLine = 0;
        bool inSection = false;
        std::array<std::string_view, 3> parts;

        std::vector<IniSection> sections;
        std::optional<IniSection> currentSection;

		for (const auto lines = Strings::Split(ini, '\n'); std::string_view line : lines)
        {

            currentLine++;
			if (line.empty() || line[0] == ';' || line[0] == '@')
                continue;

            if (line[0] == '[')
            {
                if (currentSection.has_value())
                {
                    sections.emplace_back(std::move(currentSection.value()));
                    currentSection = std::nullopt;
                }

                const unsigned indexComment = line.find(';');
                const unsigned indexClose = line.find(']');
                if (indexComment != std::string::npos && indexComment < indexClose)
                {
                    inSection = false;
                    currentSection = std::nullopt;
                    continue;
                }

                if (indexClose == std::string::npos)
                {
                    if (throwOnIniWarning)
                    {
                        throw IniParsingException(fileName, "INI section was opened but not closed", currentLine);
                    }

                    inSection = false;
                    currentSection = std::nullopt;
                }

                auto name = line.substr(1, indexClose - 1);
				Strings::TrimView(name);

                currentSection = IniSection(fileName, name, currentLine);

                inSection = true;
                continue;
            }

			if (!inSection)
			{
				continue;
			}

			if (unsigned indexComment = line.find(';'); indexComment != std::string::npos)
			{
				line = line.substr(0, indexComment);
			}

            int partCount;
            if (!std::isalpha(line[0]) && !std::isdigit(line[0]) && line[0] != '_')
            {
	            if (throwOnIniWarning)
	            {
					throw IniParsingException(fileName, "INI key was malformed", currentLine);
	            }

				continue;
            }

            partCount = ParseEquals(line, parts, allowMaps);
            if (partCount == 2)
            {
	            const auto val = parts[1];
				std::vector<IniEntry::PossibleValueType> values;
				for (std::string_view part : Strings::Split(val, ','))
				{
					Strings::TrimView(part);

					if (part.empty())
					{
						values.emplace_back("");
						continue;
					}

					// Detect is the entire thing is a number (or dashes for negative numbers, technically allows all dashes but w/e)
					if (std::ranges::all_of(part, [](const char c) { return isdigit(c) || c == '-'; }))
					{
						const std::string toConvert = std::string(part);
						errno = 0;
						char* dummy;
						if (part.find('.'))
						{
							float ret = std::strtof(toConvert.c_str(), &dummy);
							if (errno == ERANGE)
							{
								if (throwOnIniWarning)
								{
									throw IniParsingException(fileName, "Numerical value had a decimal point while not being a valid floating point value.", currentLine);
								}

								ret = 0;
								errno = 0;
							}

							values.emplace_back(ret);
						}
						else
						{
							int ret = std::strtol(toConvert.c_str(), &dummy, 10);
							if (errno == ERANGE)
							{
								if (throwOnIniWarning)
								{
									throw IniParsingException(fileName, "Numerical value was too large and not a floating point value", currentLine);
								}

								ret = 0;
							}

							values.emplace_back(ret);
						}
						continue;
					}

					if (Strings::CompareInsensitive(part, "true"))
					{
						values.emplace_back(true);
						continue;
					}

					if (Strings::CompareInsensitive(part, "false"))
					{
						values.emplace_back(false);
						continue;
					}
					
					values.emplace_back(part);
				}
				
				Strings::TrimView(parts[0]);
	            currentSection.value().EmplaceIniEntry(IniEntry(parts[0], values));
            }
            else if (partCount == 3 && allowMaps)
            {
				Strings::TrimView(parts[0]);
				Strings::TrimView(parts[1]);
				Strings::TrimView(parts[2]);

				std::vector<IniEntry::PossibleValueType> values;
				values.emplace_back(std::pair{ parts[1], parts[2] });
				currentSection.value().EmplaceIniEntry(IniEntry(parts[0], values));
            }
            else if (partCount == 1)
            {
				Strings::TrimView(parts[0]);
				std::vector<IniEntry::PossibleValueType> values;
	            currentSection->EmplaceIniEntry(IniEntry(parts[0], values));
            }
        }

		if (inSection)
		{
			sections.emplace_back(currentSection.value());
		}

		return sections;
	}

	void Reader::Reset()
	{
		length = 0;
		iniData = std::nullopt;
		block = std::nullopt;
		fileName.clear();
		throwOnIniWarning = false;
	}


	Reader::Reader() = default;

	void Reader::ThrowOnIniWarning(const bool doThrow)
	{
		this->throwOnIniWarning = doThrow;
	}
}
