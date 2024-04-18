#include "pch.h"
#include "Vaccine.h"


DWORD WINAPI HackThread(HMODULE hModule)
{
	bool ifAllocatedConsole = AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	HookManager hook_manager;

	std::cout << hook_manager.getHackThreadName() << "Hello chumba from hack thread!\n";

	while (hook_manager.stillWork()){}

	hook_manager.unhookAll();
	fclose(fDummy);
	if(ifAllocatedConsole) FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}