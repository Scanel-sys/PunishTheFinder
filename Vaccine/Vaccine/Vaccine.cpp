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
	hook_manager.Init();

	std::cout << hook_manager.getHackThreadName() << "Hello chumba from hack thread!\n";

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"Patient.exe");

	while (1)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
			break;
	}

	fclose(fDummy);
	if(ifAllocatedConsole) FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}


const std::string currentDateTime() 
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
