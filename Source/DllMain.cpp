#include "PCH.hpp"

int __stdcall DllMain(HMODULE dll, const DWORD reason, LPVOID _)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		// do thing	
	}

	return TRUE;
}

