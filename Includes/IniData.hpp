#pragma once

#include <type_traits>

#include "Structures/INI/IniSection.hpp"

namespace Ini
{
	class IniData
	{
	protected:
		~IniData() = default;

	public:
		virtual void ParseIniData(const std::vector<IniSection>& sections) = 0;
	};

	template<class T>
	concept IsIniData = std::is_base_of_v<IniData, T>;
}
