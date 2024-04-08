#ifndef VACCINE_H
#define VACCINE_H

#include "Hookpatch32.h"


#ifdef VACCINE_EXPORTS
	#define VACCINE_API __declspec(dllexport)
#else
	#define VACCINE_API __declspec(dllimport)
#endif


typedef decltype(&FindNextFileA) tFindNextFileA;
typedef decltype(&FindNextFileW) tFindNextFileW;

typedef decltype(&FindFirstFileA) tFindFirstFileA;
typedef decltype(&FindFirstFileW) tFindFirstFileW;

typedef decltype(&CreateFileA) tCreateFileA;
typedef decltype(&CreateFileW) tCreateFileW;


BOOL __stdcall HooktFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData);
BOOL __stdcall HooktFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData);

HANDLE __stdcall HookFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
HANDLE __stdcall HookFindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData);

HANDLE __stdcall HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, 
									DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
									DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

HANDLE __stdcall HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, 
								DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
								DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);


DWORD WINAPI HackThread(HMODULE hModule);

#endif