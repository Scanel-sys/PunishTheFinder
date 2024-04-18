#pragma once

#include "Injector.h"
#include <Windows.h>
#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>

namespace fs = std::filesystem;


class Monitor
{
	Injector _injector;

	DWORD _pid;
	fs::path _dllPath;
	std::wstring _processName;
	std::string _funcName;
	fs::path _fileToHide;
	HANDLE _pipe;

	bool _pid_passed;
	bool _proc_name_passed;
	bool _func_name_passed;
	bool _file_to_hide_passed;

	void initArgsWithZeroes();

	std::wstring s2ws(const std::string& str);

	std::vector <std::string> split(int argc, char * argv[]);
	bool ifServiceWord(char* word);
	void setArgs(std::vector <std::string>);
	
	bool findVaccine();
	bool inject();
	void init_and_launch_pipe();

	int DisplayResourceNAMessageBox(std::string msg);

	void monitor();

public:

	Monitor(int argc, char * argv[]);
	~Monitor();
	void launch();
};