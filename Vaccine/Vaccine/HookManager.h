#ifndef HOOKMANAGER_H
#define HOOKMANAGER_H

#include "Hookpatch32.h"
#include <filesystem>
#include <atlstr.h>
#include <string>

#define SIZE				6
#define BUFFER_SIZE			300


namespace fs = std::filesystem;


class HookManager
{
private:
	HANDLE _pipe;

	fs::path _filePathToHide;
	std::string _someKernelFunctionName;

	std::string _thread_name;
	std::vector <Hookpatch32 *> _hooks{};

	std::wstring s2ws(const std::string& str);

	void __stdcall initPipe();
	void __stdcall readPipe();

	bool _isWorking;

public:
	HookManager();
	~HookManager();
	
	std::string getHackThreadName();

	void initHideHooks(fs::path filePath);
	void initCommonHook(std::string dllName, std::string funcName);
	void push_back_hook(Hookpatch32 * hook);

	static std::string __stdcall getHookLog(std::string hookName, std::string action);

	void unhookAll();
	void hookAll();

	bool stillWork();
};
#endif