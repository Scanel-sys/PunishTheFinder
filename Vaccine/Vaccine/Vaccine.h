#pragma once
#include "Hookpatch32.h"


#ifdef VACCINE_EXPORTS
	#define VACCINE_API __declspec(dllexport)
#else
	#define VACCINE_API __declspec(dllimport)
#endif


//typedef BOOL(__stdcall* tFindNextFileA)(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
typedef decltype(&FindNextFileA) tFindNextFileA;
typedef decltype(&FindNextFileW) tFindNextFileW;


BOOL __stdcall hooktFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
BOOL __stdcall hooktFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);

DWORD WINAPI HackThread(HMODULE hModule);
