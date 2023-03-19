#include "PCH.hpp"

bool CompareInsensitive(const std::string& str1, const std::string& str2)
{
	auto toLower{ std::ranges::views::transform(std::tolower) };
	return std::ranges::equal(str1 | toLower, str2 | toLower);
}

int __stdcall DllMain(HMODULE dll, const DWORD reason, LPVOID _)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		// do thing	
	}

	return TRUE;
}

