#include "pch.h"
#include "Vaccine.h"


const char* HackThreadName = "[HackThread] ";

const char* FileNameA = "123.txt";
const wchar_t* FileNameW = L"123.txt";
const char* FileNameFindFirstA = "123 - Copy (3).txt";
const wchar_t* FileNameFindFirstW = L"123 - Copy (3).txt";

const char* fullFileNameA = "123.txt";;
const wchar_t* fullFileNameW = L"123.txt";;


tFindNextFileA oFindNextFileA;
tFindNextFileW oFindNextFileW;

tFindFirstFileA oFindFirstFileA;
tFindFirstFileW oFindFirstFileW;

tCreateFileA oCreateFileA;
tCreateFileW oCreateFileW;


BOOL __stdcall HooktFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
	bool res = oFindNextFileA(hFindFile, lpFindFileData);

	if (strcmp(lpFindFileData->cFileName, FileNameA) == 0)
	{
		std::cout << HackThreadName << "file being hidden\n";
		if (res)
			return oFindNextFileA(hFindFile, lpFindFileData);
		else
			return false;
	}
	return true;
}

BOOL __stdcall HooktFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	bool res = oFindNextFileW(hFindFile, lpFindFileData);
	
	if (wcscmp(lpFindFileData->cFileName, FileNameW) == 0)
	{
		std::cout << HackThreadName << "file being hidden\n";
		if (res)
			return oFindNextFileW(hFindFile, lpFindFileData);
		else
			return false;
	}
	return true;
}


HANDLE __stdcall HookFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	std::cout << HackThreadName << "FindFirstFileA hooked\n";

	HANDLE res = oFindFirstFileA(lpFileName, lpFindFileData);

	if (strcmp(lpFindFileData->cFileName, FileNameFindFirstA) == 0)
		FindNextFileA(res, lpFindFileData);

	return res;
}

HANDLE __stdcall HookFindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
	std::cout << HackThreadName << "FindFirstFileW hooked\n";

	HANDLE res = oFindFirstFileW(lpFileName, lpFindFileData);

	if (wcscmp(lpFindFileData->cFileName, FileNameFindFirstW) == 0)
		FindNextFileW(res, lpFindFileData);

	return res;
}


HANDLE __stdcall HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, 
									DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
									DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	std::cout << HackThreadName << "CreateFileA hooked\n";
	
	if (strcmp(lpFileName, fullFileNameA) == 0)
	{
		if (dwCreationDisposition == OPEN_EXISTING ||
			dwCreationDisposition == TRUNCATE_EXISTING)
		{
			SetLastError(ERROR_FILE_NOT_FOUND);
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			DeleteFileA(lpFileName);
		}
	}

	return oCreateFileA(lpFileName, dwDesiredAccess,
						dwShareMode, lpSecurityAttributes, 
						dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


HANDLE __stdcall HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
								LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, 
								DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	std::cout << HackThreadName << "CreateFileW hooked\n";

	if (wcscmp(lpFileName, fullFileNameW) == 0)
	{
		if (dwCreationDisposition == OPEN_EXISTING || 
			dwCreationDisposition == TRUNCATE_EXISTING)
		{
			SetLastError(ERROR_FILE_NOT_FOUND);
			return INVALID_HANDLE_VALUE;
		}
		else
		{
			DeleteFileW(lpFileName);
		}
	}

	return oCreateFileW(lpFileName, dwDesiredAccess,
						dwShareMode, lpSecurityAttributes,
						dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
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


	std::cout << HackThreadName << "Hello chumba from hack thread!\n";

	Hookpatch32 hookFindNextFileA;
	oFindNextFileA = (tFindNextFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindNextFileA");
	oFindNextFileA = (tFindNextFileA)hookFindNextFileA.trampHook((BYTE*)oFindNextFileA, (BYTE*)HooktFindNextFileA, PATCH_SIZE_32);

	Hookpatch32 hookFindNextFileW;
	oFindNextFileW = (tFindNextFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindNextFileW");
	oFindNextFileW = (tFindNextFileW)hookFindNextFileW.trampHook((BYTE*)oFindNextFileW, (BYTE*)HooktFindNextFileW, PATCH_SIZE_32);

	Hookpatch32 hookFindFirstFileA;
	oFindFirstFileA = (tFindFirstFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindFirstFileA");
	oFindFirstFileA = (tFindFirstFileA)hookFindFirstFileA.trampHook((BYTE*)oFindFirstFileA, (BYTE*)HookFindFirstFileA, PATCH_SIZE_32);
	
	Hookpatch32 hookFindFirstFileW;
	oFindFirstFileW = (tFindFirstFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindFirstFileW");
	oFindFirstFileW = (tFindFirstFileW)hookFindFirstFileW.trampHook((BYTE*)oFindFirstFileW, (BYTE*)HookFindFirstFileW, PATCH_SIZE_32);

	Hookpatch32 hookCreateFileA;
	oCreateFileA = (tCreateFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "CreateFileA");
	oCreateFileA = (tCreateFileA)hookCreateFileA.trampHook((BYTE*)oCreateFileA, (BYTE*)HookCreateFileA, PATCH_SIZE_32);

	Hookpatch32 hookCreateFileW;
	oCreateFileW = (tCreateFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "CreateFileW");
	oCreateFileW = (tCreateFileW)hookCreateFileW.trampHook((BYTE*)oCreateFileW, (BYTE*)HookCreateFileW, PATCH_SIZE_32);


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