#include "pch.h"
#include "HookManager.h"

namespace hookManagerNS
{
	std::string HackThreadName = "[HackThread] ";

	typedef decltype(&FindNextFileA) tFindNextFileA;
	typedef decltype(&FindNextFileW) tFindNextFileW;

	typedef decltype(&FindFirstFileA) tFindFirstFileA;
	typedef decltype(&FindFirstFileW) tFindFirstFileW;

	typedef decltype(&CreateFileA) tCreateFileA;
	typedef decltype(&CreateFileW) tCreateFileW;

	BOOL __stdcall HookFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
	BOOL __stdcall HookFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);
	HANDLE __stdcall HookFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
	HANDLE __stdcall HookFindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);
	HANDLE __stdcall HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess,
		DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	HANDLE __stdcall HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

	tFindNextFileA oFindNextFileA;
	tFindNextFileW oFindNextFileW;

	tFindFirstFileA oFindFirstFileA;
	tFindFirstFileW oFindFirstFileW;

	tCreateFileA oCreateFileA;
	tCreateFileW oCreateFileW;

	std::string process_name;
	HANDLE		 PipeHandle;

	void status();
}

const char* FileNameA = "123.txt";
const wchar_t* FileNameW = L"123.txt";
const char* FileNameFindFirstA = "123 - Copy (3).txt";
const wchar_t* FileNameFindFirstW = L"123 - Copy (3).txt";

const char* fullFileNameA = "123.txt";
const wchar_t* fullFileNameW = L"123.txt";

BOOL __stdcall hookManagerNS::HookFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
	SYSTEMTIME lTime; GetSystemTime(&lTime);

	std::cout << hookManagerNS::HackThreadName << "FindNextFileA hooked : " << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

	bool res = oFindNextFileA(hFindFile, lpFindFileData);

	if (strcmp(lpFindFileData->cFileName, FileNameA) == 0)
	{
		std::cout << hookManagerNS::HackThreadName << "file being hidden\n";
		if (res)
			return oFindNextFileA(hFindFile, lpFindFileData);
		else
			return false;
	}
	return true;
}

BOOL __stdcall hookManagerNS::HookFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);

	std::cout << hookManagerNS::HackThreadName << "FindNextFileW hooked : " << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

	bool res = oFindNextFileW(hFindFile, lpFindFileData);

	if (wcscmp(lpFindFileData->cFileName, FileNameW) == 0)
	{
		std::cout << hookManagerNS::HackThreadName << "file being hidden\n";
		if (res)
			return oFindNextFileW(hFindFile, lpFindFileData);
		else
			return false;
	}
	return true;
}


HANDLE __stdcall hookManagerNS::HookFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);
	std::cout << hookManagerNS::HackThreadName << "FindFirstFileA hooked : " << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

	HANDLE res = oFindFirstFileA(lpFileName, lpFindFileData);

	if (strcmp(lpFindFileData->cFileName, FileNameFindFirstA) == 0)
		FindNextFileA(res, lpFindFileData);

	return res;
}

HANDLE __stdcall hookManagerNS::HookFindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);
	std::cout << hookManagerNS::HackThreadName << "FindFirstFileW hooked : " << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

	HANDLE res = oFindFirstFileW(lpFileName, lpFindFileData);

	if (wcscmp(lpFindFileData->cFileName, FileNameFindFirstW) == 0)
		FindNextFileW(res, lpFindFileData);

	return res;
}


HANDLE __stdcall hookManagerNS::HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess,
	DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);
	std::cout << hookManagerNS::HackThreadName << "CreateFileA hooked : " << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

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


HANDLE __stdcall hookManagerNS::HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);
	std::cout << hookManagerNS::HackThreadName << "CreateFileW hooked : " << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

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

void status()
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);
	//std::string to_write = hookManagerNS::HackThreadName + hookManagerNS::process_name;
	//to_write.push_back(lTime.wHour); 
	//to_write += ':';
	//to_write.push_back(lTime.wMinute);
	//to_write += ':';
	//to_write.push_back(lTime.wSecond);

	std::cout << hookManagerNS::HackThreadName << hookManagerNS::process_name << lTime.wHour << ':' << lTime.wMinute << ':' << lTime.wSecond << '\n';

	//if (Timer == NULL || strcmp(DataDate, Timer))
	//{
	//	if (Timer == NULL)
	//		Timer = (char*)calloc(10000, sizeof(char));

	//	DWORD NumberWriteBytes = (lstrlenA(to_write.c_str() + 1) * sizeof(CHAR);
	//	SuccessFlag = WriteFile(hookManagerNS::PipeHandle, MessageToSend, NumberWriteBytes, &NumberWrittenBytes, NULL);
	//}

}


HookManager::HookManager()
{
	this->_thread_name = hookManagerNS::HackThreadName;
}

HookManager::HookManager(std::string thread_name)
{
	this->_thread_name = thread_name;
}

HookManager::~HookManager()
{
	for (auto tHook : _hooks)
		tHook->trampUnhook();
}

std::string HookManager::getHackThreadName()
{
	return this->_thread_name;
}

void HookManager::Init()
{
	Hookpatch32 hookFindNextFileA;
	hookManagerNS::oFindNextFileA = (hookManagerNS::tFindNextFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindNextFileA");
	hookManagerNS::oFindNextFileA = (hookManagerNS::tFindNextFileA)hookFindNextFileA.trampHook((BYTE*)hookManagerNS::oFindNextFileA,
																								(BYTE*)hookManagerNS::HookFindNextFileA, 
																								PATCH_SIZE_32);
	push_back_hook(&hookFindNextFileA);


	Hookpatch32 hookFindNextFileW;
	hookManagerNS::oFindNextFileW = (hookManagerNS::tFindNextFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindNextFileW");
	hookManagerNS::oFindNextFileW = (hookManagerNS::tFindNextFileW)hookFindNextFileW.trampHook((BYTE*)hookManagerNS::oFindNextFileW, 
																								(BYTE*)hookManagerNS::HookFindNextFileW, 
																								PATCH_SIZE_32);
	push_back_hook(&hookFindNextFileW);

	Hookpatch32 hookFindFirstFileA;
	hookManagerNS::oFindFirstFileA = (hookManagerNS::tFindFirstFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindFirstFileA");
	hookManagerNS::oFindFirstFileA = (hookManagerNS::tFindFirstFileA)hookFindFirstFileA.trampHook((BYTE*)hookManagerNS::oFindFirstFileA,
																									(BYTE*)hookManagerNS::HookFindFirstFileA, 
																									PATCH_SIZE_32);
	push_back_hook(&hookFindFirstFileA);


	Hookpatch32 hookFindFirstFileW;
	hookManagerNS::oFindFirstFileW = (hookManagerNS::tFindFirstFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindFirstFileW");
	hookManagerNS::oFindFirstFileW = (hookManagerNS::tFindFirstFileW)hookFindFirstFileW.trampHook((BYTE*)hookManagerNS::oFindFirstFileW, 
																									(BYTE*)hookManagerNS::HookFindFirstFileW, 
																									PATCH_SIZE_32);
	push_back_hook(&hookFindFirstFileW);


	Hookpatch32 hookCreateFileA;
	hookManagerNS::oCreateFileA = (hookManagerNS::tCreateFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "CreateFileA");
	hookManagerNS::oCreateFileA = (hookManagerNS::tCreateFileA)hookCreateFileA.trampHook((BYTE*)hookManagerNS::oCreateFileA, 
																							(BYTE*)hookManagerNS::HookCreateFileA, 
																							PATCH_SIZE_32);
	push_back_hook(&hookCreateFileA);


	Hookpatch32 hookCreateFileW;
	hookManagerNS::oCreateFileW = (hookManagerNS::tCreateFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "CreateFileW");
	hookManagerNS::oCreateFileW = (hookManagerNS::tCreateFileW)hookCreateFileW.trampHook((BYTE*)hookManagerNS::oCreateFileW, 
																							(BYTE*)hookManagerNS::HookCreateFileW, 
																							PATCH_SIZE_32);
	push_back_hook(&hookCreateFileW);
}

void HookManager::push_back_hook(Hookpatch32 * hook)
{
	_hooks.push_back(hook);
}
