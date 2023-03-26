#include <stdexcept>
#include <fstream>

#include "Structures/INI/IniSection.hpp"
#include "Structures/INI/BiniStringBlock.hpp"

namespace Ini
{
	IniSection::IniSection(const std::string_view& fileName, const std::string_view& name, const long line)
	{
		if (name.empty())
		{
			throw std::invalid_argument("IniSection was created with empty name");
		}

		this->fileName = fileName;
		this->name = name;
		this->line = line;
	}

	IniSection::IniSection(const std::string_view& fileName, std::fstream& stream, BiniStringBlock& block)
	{
		this->fileName = fileName;

		short offset;
		stream.read(reinterpret_cast<char*>(&offset), sizeof(short));
		name = block.Get(offset);

		short count;
		stream.read(reinterpret_cast<char*>(&count), sizeof(short));
		entries.reserve(count);

		for (int i = 0; i < count; i++)
		{
			entries.emplace_back(fileName, stream, block);
		}
	}

	void IniSection::EmplaceIniEntry(IniEntry entry)
	{
		entries.emplace_back(std::move(entry));
	}
}
