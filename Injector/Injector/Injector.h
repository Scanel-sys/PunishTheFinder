#pragma once

#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <Windows.h>
#include <string>
#include <stdexcept>
#include <tlhelp32.h>

namespace fs = std::filesystem;

class Injector
{
private:
	
	DWORD _process_id;
	void* _load_lib_addr;

	DWORD getPid(std::wstring processName);

public:
	Injector();

	void findLoadProcess();

	void setProcessContext(std::wstring processName);
	void setProcessContext(DWORD dwProcessId);

	bool inject(std::filesystem::path dllPath);

	bool makeInject(std::wstring processName, std::filesystem::path dllPath);
	bool makeInject(DWORD dwProcessId, std::filesystem::path dllPath);
};