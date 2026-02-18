#include <Windows.h>
#include "UAGame.h"

BOOL DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

		UAGame::bootstrap();
	}

	return TRUE;
}

