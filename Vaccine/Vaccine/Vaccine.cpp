#include "pch.h"
#include "Vaccine.h"


tFindNextFileA oFindNextFileA;
tFindNextFileW oFindNextFileW;



BOOL __stdcall hooktFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
	std::cout << "[hook] Hooked FindNextFileA\n";

	return oFindNextFileA(hFindFile, lpFindFileData);
}

BOOL __stdcall hooktFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	std::cout << "[hook] Hooked FindNextFileW\n";

	return oFindNextFileW(hFindFile, lpFindFileData);
}


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


	std::cout << "[HackThread] Hello chumba from hack thread!\n";

	//
	Hookpatch32 hookFindNextFileA;
	oFindNextFileA = (tFindNextFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindNextFileA");
	oFindNextFileA = (tFindNextFileA)hookFindNextFileA.trampHook((BYTE*)oFindNextFileA, (BYTE*)hooktFindNextFileA, 5);

	Hookpatch32 hookFindNextFileW;
	oFindNextFileW = (tFindNextFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindNextFileW");
	oFindNextFileW = (tFindNextFileW)hookFindNextFileW.trampHook((BYTE*)oFindNextFileW, (BYTE*)hooktFindNextFileW, 5);


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