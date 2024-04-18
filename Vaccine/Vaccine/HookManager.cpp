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

	bool checkIfStop();
	void __stdcall universal_hook();
	HANDLE pipe;
	char func_name[50] = { 0 };
	const char* KernelLibName = "kernel32.dll";
	std::string someKernelFuncName;
	Hookpatch32 someCommonHook;
	FARPROC someCommonFunc;
	std::string commonFuncMSG;

	fs::path filePathToHide;

	tFindNextFileA oFindNextFileA;
	tFindNextFileW oFindNextFileW;

	tFindFirstFileA oFindFirstFileA;
	tFindFirstFileW oFindFirstFileW;

	tCreateFileA oCreateFileA;
	tCreateFileW oCreateFileW;

	std::string injectedProcName;

	bool isWorking = true;
	char buffer[BUFFER_SIZE];
}



BOOL __stdcall hookManagerNS::HookFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData)
{
	bool lIsWorking = checkIfStop();

	std::string hookLog = HookManager::getHookLog("FindNextFileA", "hooked");
	WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);


	bool res = oFindNextFileA(hFindFile, lpFindFileData);
	
	if (strcmp(lpFindFileData->cFileName, filePathToHide.filename().string().c_str()) == 0)
	{
		hookLog = HookManager::getHookLog("FindNextFileA", ": file being hidden");
		WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);
		
		isWorking = lIsWorking;
		if (res)
			return oFindNextFileA(hFindFile, lpFindFileData);
		else
			return false;
	}
	isWorking = lIsWorking;
	return true;
}

BOOL __stdcall hookManagerNS::HookFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
{
	bool lIsWorking = checkIfStop();

	std::string hookLog = HookManager::getHookLog("FindNextFileW", "hooked");
	WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

	bool res = hookManagerNS::oFindNextFileW(hFindFile, lpFindFileData);

	if (wcscmp(lpFindFileData->cFileName, filePathToHide.filename().wstring().c_str()) == 0)
	{
		hookLog = HookManager::getHookLog("FindNextFileW", ": file being hidden");
		WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

		isWorking = lIsWorking;
		if (res)
			return hookManagerNS::oFindNextFileW(hFindFile, lpFindFileData);
		else
			return false;
	}
	isWorking = lIsWorking;
	return true;
}


HANDLE __stdcall hookManagerNS::HookFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	bool lIsWorking = checkIfStop();

	std::string hookLog = HookManager::getHookLog("FindFirstFileA", "hooked");
	WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

	HANDLE res = oFindFirstFileA(lpFileName, lpFindFileData);

	if (strcmp(lpFindFileData->cFileName, filePathToHide.filename().string().c_str()) == 0)
	{
		hookLog = HookManager::getHookLog("FindFirstFileA", ": file being hidden");
		WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

		FindNextFileA(res, lpFindFileData);
	}
	isWorking = lIsWorking;
	return res;
}

HANDLE __stdcall hookManagerNS::HookFindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
{
	bool lIsWorking = checkIfStop();

	std::string hookLog = HookManager::getHookLog("FindFirstFileW", "hooked");
	WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

	HANDLE res = oFindFirstFileW(lpFileName, lpFindFileData);

	if (wcscmp(lpFindFileData->cFileName, filePathToHide.filename().wstring().c_str()) == 0)
	{
		hookLog = HookManager::getHookLog("FindFirstFileW", ": file being hidden");
		WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

		FindNextFileW(res, lpFindFileData);
	}
	isWorking = lIsWorking;

	return res;
}


HANDLE __stdcall hookManagerNS::HookCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess,
												DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
												DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	bool lIsWorking = checkIfStop();

	std::string hookLog = HookManager::getHookLog("CreateFileA", "hooked");
	WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

	if (strcmp(lpFileName, filePathToHide.string().c_str()) == 0)
	{
		hookLog = HookManager::getHookLog("CreateFileA", ": file being hidden");
		WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

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
	isWorking = lIsWorking;

	return oCreateFileA(lpFileName, dwDesiredAccess,
						dwShareMode, lpSecurityAttributes,
						dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


HANDLE __stdcall hookManagerNS::HookCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
												LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
												DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	bool lIsWorking = checkIfStop();

	std::string hookLog = HookManager::getHookLog("CreateFileW", "hooked");
	BOOL result = WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

	if (wcscmp(lpFileName, filePathToHide.wstring().c_str()) == 0)
	{
		hookLog = HookManager::getHookLog("CreateFileW", ": file being hidden");
		WriteFile(pipe, hookLog.c_str(), hookLog.size() + 1, NULL, NULL);

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
	isWorking = lIsWorking;

	return oCreateFileW(lpFileName, dwDesiredAccess,
						dwShareMode, lpSecurityAttributes,
						dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}



void __stdcall hookManagerNS::universal_hook()
 {
	__asm {
		push eax
		push ebx
		push ecx
		push edx
	}

	commonFuncMSG = HookManager::getHookLog(func_name, "hooked");
	WriteFile(pipe, commonFuncMSG.c_str(), commonFuncMSG.size() + 1, NULL, NULL);

	someCommonHook.unhook();
	__asm {
		pop edx
		pop ecx
		pop ebx
		pop eax

		pop     ecx
		pop edi
		pop esi
		pop edx

		add esp, 0x114
		cmp     ebp, esp
		mov     esp, ebp
		pop ebp
		pop ebx //eip

		call someCommonFunc
		push eax
	}
	someCommonHook.hook();
	isWorking = checkIfStop();
	__asm {
		pop eax
		jmp ebx
	}
}

bool hookManagerNS::checkIfStop()
{
	if (ReadFile(pipe, buffer, BUFFER_SIZE, NULL, NULL) &&
		strcmp("-stop", buffer) == 0)
		return false;
	
	return true;
}

std::wstring HookManager::s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

void __stdcall HookManager::initPipe()
{
	WaitNamedPipeA("\\\\.\\pipe\\pipe_of_insight", 20000);
	hookManagerNS::pipe = _pipe = CreateFileA("\\\\.\\pipe\\pipe_of_insight", 
						GENERIC_READ | GENERIC_WRITE, 
						0, 
						NULL, 
						OPEN_EXISTING, 
						0, 
						NULL);

	DWORD Mode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState(_pipe,
							&Mode,
							NULL,
							NULL);
}

void __stdcall HookManager::readPipe()
{
	char buffer[BUFFER_SIZE];
	while (!ReadFile(_pipe, buffer, BUFFER_SIZE, NULL, NULL)) {}
	
	int msgs_count = std::stoi(buffer);
	for (int i = 0; i < msgs_count; i++)
	{
		ReadFile(_pipe, buffer, BUFFER_SIZE, NULL, NULL);
		if (strncmp("-hide", buffer, strlen("-hide")) == 0)
		{
			ReadFile(_pipe, buffer, BUFFER_SIZE, NULL, NULL);
			hookManagerNS::filePathToHide.append(buffer);
			_filePathToHide.append(buffer);
		}
		else if (strncmp("-func", buffer, strlen("-func")) == 0)
		{
			ReadFile(_pipe, buffer, BUFFER_SIZE, NULL, NULL);
			hookManagerNS::someKernelFuncName.append(buffer);
			_someKernelFunctionName.append(buffer);
		}
	}
}

HookManager::HookManager()
{
	_isWorking = true;
	_filePathToHide.clear();
	_someKernelFunctionName.clear();
	this->_thread_name = hookManagerNS::HackThreadName;
	initPipe();
	readPipe();
	DWORD Mode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
	SetNamedPipeHandleState(_pipe,
							&Mode,
							NULL,
							NULL);

	if(!_filePathToHide.empty())
		initHideHooks(_filePathToHide);
	if(!_someKernelFunctionName.empty())
		initCommonHook(hookManagerNS::KernelLibName, _someKernelFunctionName);
}

HookManager::~HookManager()
{
	for (auto tHook : _hooks)
		delete tHook;

	VirtualFree(hookManagerNS::oFindNextFileA, 0, MEM_RELEASE);
	VirtualFree(hookManagerNS::oFindNextFileW, 0, MEM_RELEASE);
	VirtualFree(hookManagerNS::oFindFirstFileA, 0, MEM_RELEASE);
	VirtualFree(hookManagerNS::oFindFirstFileW, 0, MEM_RELEASE);
	VirtualFree(hookManagerNS::oCreateFileA, 0, MEM_RELEASE);
	VirtualFree(hookManagerNS::oCreateFileW, 0, MEM_RELEASE);
}

std::string HookManager::getHackThreadName()
{
	return this->_thread_name;
}

void HookManager::initHideHooks(fs::path filePath)
{
	hookManagerNS::filePathToHide = filePath;

	Hookpatch32* hookFindNextFileA = new Hookpatch32;
	hookManagerNS::oFindNextFileA = (hookManagerNS::tFindNextFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindNextFileA");
	hookManagerNS::oFindNextFileA = (hookManagerNS::tFindNextFileA)hookFindNextFileA->initTrampHook((BYTE*)hookManagerNS::oFindNextFileA,
																									(BYTE*)hookManagerNS::HookFindNextFileA, 
																									PATCH_SIZE_32);
	push_back_hook(hookFindNextFileA);


	Hookpatch32* hookFindNextFileW = new Hookpatch32;
	hookManagerNS::oFindNextFileW = (hookManagerNS::tFindNextFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindNextFileW");
	hookManagerNS::oFindNextFileW = (hookManagerNS::tFindNextFileW)hookFindNextFileW->initTrampHook((BYTE*)hookManagerNS::oFindNextFileW, 
																									(BYTE*)hookManagerNS::HookFindNextFileW, 
																									PATCH_SIZE_32);
	push_back_hook(hookFindNextFileW);

	Hookpatch32* hookFindFirstFileA = new Hookpatch32;
	hookManagerNS::oFindFirstFileA = (hookManagerNS::tFindFirstFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "FindFirstFileA");
	hookManagerNS::oFindFirstFileA = (hookManagerNS::tFindFirstFileA)hookFindFirstFileA->initTrampHook((BYTE*)hookManagerNS::oFindFirstFileA,
																										(BYTE*)hookManagerNS::HookFindFirstFileA, 
																										PATCH_SIZE_32);
	push_back_hook(hookFindFirstFileA);


	Hookpatch32* hookFindFirstFileW = new Hookpatch32;
	hookManagerNS::oFindFirstFileW = (hookManagerNS::tFindFirstFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "FindFirstFileW");
	hookManagerNS::oFindFirstFileW = (hookManagerNS::tFindFirstFileW)hookFindFirstFileW->initTrampHook((BYTE*)hookManagerNS::oFindFirstFileW, 
																										(BYTE*)hookManagerNS::HookFindFirstFileW, 
																										PATCH_SIZE_32);
	push_back_hook(hookFindFirstFileW);


	Hookpatch32* hookCreateFileA = new Hookpatch32;
	hookManagerNS::oCreateFileA = (hookManagerNS::tCreateFileA)GetProcAddress(GetModuleHandleA("KernelBase.dll"), "CreateFileA");
	hookManagerNS::oCreateFileA = (hookManagerNS::tCreateFileA)hookCreateFileA->initTrampHook((BYTE*)hookManagerNS::oCreateFileA, 
																								(BYTE*)hookManagerNS::HookCreateFileA, 
																								PATCH_SIZE_32);
	push_back_hook(hookCreateFileA);


	Hookpatch32* hookCreateFileW = new Hookpatch32;
	hookManagerNS::oCreateFileW = (hookManagerNS::tCreateFileW)GetProcAddress(GetModuleHandleW(L"KernelBase.dll"), "CreateFileW");
	hookManagerNS::oCreateFileW = (hookManagerNS::tCreateFileW)hookCreateFileW->initTrampHook((BYTE*)hookManagerNS::oCreateFileW, 
																								(BYTE*)hookManagerNS::HookCreateFileW, 
																								PATCH_SIZE_32);
	push_back_hook(hookCreateFileW);
}

void HookManager::initCommonHook(std::string dllName, std::string funcName)
{
	if (funcName.back() == 'A')
		hookManagerNS::someCommonFunc = GetProcAddress(GetModuleHandleA(dllName.c_str()), funcName.c_str());
	else if (funcName.back() == 'W')
		hookManagerNS::someCommonFunc = GetProcAddress(GetModuleHandleW(s2ws(dllName).c_str()), funcName.c_str());
	else
		return;

	std::string someKernelFuncName = funcName;
	strcpy_s(hookManagerNS::func_name, funcName.c_str());

	hookManagerNS::someCommonHook.init_base_hook((BYTE*)hookManagerNS::someCommonFunc,
												 (BYTE *)hookManagerNS::universal_hook,
												 PATCH_SIZE_32);

	this->push_back_hook(&hookManagerNS::someCommonHook);
}

void HookManager::push_back_hook(Hookpatch32 * hook)
{
	_hooks.push_back(hook);
}

std::string __stdcall HookManager::getHookLog(std::string hookName, std::string action)
{
	SYSTEMTIME lTime;  GetLocalTime(&lTime);
	CString cstrMessage;
	std::string to_write = hookManagerNS::HackThreadName;
	to_write += hookName + " " + action + " ";

	cstrMessage.Format(L"%d-%02d-%02d %02d:%02d:%02d.%03d",
						lTime.wYear,
						lTime.wMonth,
						lTime.wDay,
						lTime.wHour,
						lTime.wMinute,
						lTime.wSecond,
						lTime.wMilliseconds);

	to_write += CT2A(cstrMessage.GetString());

	return to_write;
}

void HookManager::unhookAll()
{
	for (auto tHook : _hooks)
		tHook->unhook();
}

void HookManager::hookAll()
{
	for (auto tHook : _hooks)
		tHook->hook();
}

bool HookManager::stillWork()
{
	_isWorking = hookManagerNS::isWorking;
	return _isWorking;
}