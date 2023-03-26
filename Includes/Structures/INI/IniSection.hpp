#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "IniEntry.hpp"

namespace Ini
{
	class IniSection
	{
		std::string_view name;
		long line = -1;
		std::string_view fileName;
		std::vector<IniEntry> entries;

	public:
		explicit IniSection(const std::string_view& fileName, const std::string_view& name, long line);
		IniSection(const std::string_view& fileName, std::fstream& stream, class BiniStringBlock& block);
		void EmplaceIniEntry(IniEntry entry);

		std::optional<IniEntry&> operator[](int index);
		std::optional<IniEntry&> operator[](std::string_view name);
	};
}
