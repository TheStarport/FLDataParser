#pragma once

#include <string_view>
#include <variant>
#include <vector>

namespace Ini
{
	class IniEntry
	{
	public:
		using PossibleValueType = std::variant<bool, int, float, std::string_view, std::pair<std::string_view, std::string_view>>;
	private:
		std::vector<PossibleValueType> values;
		std::string name;

	public:
		IniEntry(std::string_view file, std::fstream& stream, class BiniStringBlock&);
		explicit IniEntry(std::string_view name, std::vector<PossibleValueType>& values);
	};
}
