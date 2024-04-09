#ifndef VACCINE_H
#define VACCINE_H

#include "HookManager.h"



#ifdef VACCINE_EXPORTS
	#define VACCINE_API __declspec(dllexport)
#else
	#define VACCINE_API __declspec(dllimport)
#endif


DWORD WINAPI HackThread(HMODULE hModule);

const std::string currentDateTime();

#endif