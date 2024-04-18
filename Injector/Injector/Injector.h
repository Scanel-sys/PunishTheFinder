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
	void setProcessContext(std::string processName);

	void setProcessContext(DWORD dwProcessId);

	bool inject(fs::path dllPath);

	bool makeInject(std::wstring processName, fs::path dllPath);
	bool makeInject(std::string processName, fs::path dllPath);
	bool makeInject(DWORD dwProcessId, fs::path dllPath);
	
	static std::wstring s2ws(const std::string& str);
};