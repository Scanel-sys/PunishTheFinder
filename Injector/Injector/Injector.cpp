#include "Injector.h"


std::wstring Injector::s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

DWORD Injector::getPid(std::wstring processName)
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe;
	DWORD pid = 0;
	BOOL hResult;
	
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);	// snapshot of all processes in the system
	if (INVALID_HANDLE_VALUE == hSnapshot) return 0;

	pe.dwSize = sizeof(PROCESSENTRY32);			// initializing size: needed for using Process32First
	hResult = Process32First(hSnapshot, &pe);	// info about first process encountered in a system snapshot
	while (hResult) 
	{
		if (wcscmp(processName.c_str(), pe.szExeFile) == 0) {
			pid = pe.th32ProcessID;
			hResult = false;
		}
		else
			hResult = Process32Next(hSnapshot, &pe);
	}
	CloseHandle(hSnapshot);
	return pid;
}


Injector::Injector()
{
	_load_lib_addr = nullptr;
	_process_id = 0;
}

void Injector::findLoadProcess()
{
	HMODULE hKernel32 = GetModuleHandle(L"kernel32");
	if(hKernel32 == 0)
		throw std::runtime_error("Failed to find example.dll");

	_load_lib_addr = GetProcAddress(hKernel32, "LoadLibraryA");
}

void Injector::setProcessContext(std::wstring processName)
{
	_process_id = getPid(processName);
}

void Injector::setProcessContext(std::string processName)
{
	setProcessContext(s2ws(processName));
}

void Injector::setProcessContext(DWORD dwProcessId)
{
	_process_id = dwProcessId;
}

bool Injector::inject(fs::path dllPath)
{
	HANDLE pHandler;
	HANDLE remoteThread;
	LPVOID remoteBuffer;
	uint64_t path_byte_size = strlen(dllPath.string().c_str()) + 1;

	if (_process_id == 0)
	{
		std::cout << "[*] PID is zero\n";
		return false;
	}
	else
	{
		std::cout << "[*] Injecting process : " << _process_id << '\n';
	}

	pHandler = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _process_id);

	remoteBuffer = VirtualAllocEx(pHandler, 
									NULL, 
									path_byte_size, 
									(MEM_RESERVE | MEM_COMMIT), 
									PAGE_EXECUTE_READWRITE);
	
	if (remoteBuffer == NULL)
	{
		std::cout << "[*] Allocating memory trouble. Exiting...";
		return false;
	}

	WriteProcessMemory(pHandler, 
						remoteBuffer, 
						dllPath.string().c_str(), 
						path_byte_size, NULL);

	remoteThread = CreateRemoteThread(pHandler, 
										NULL, 0, 
										(LPTHREAD_START_ROUTINE)_load_lib_addr, 
										remoteBuffer, 
										0, NULL);
	CloseHandle(pHandler);
	return true;
}

bool Injector::makeInject(std::wstring processName, fs::path dllPath)
{
	findLoadProcess();
	setProcessContext(processName);
	return inject(dllPath);
}

bool Injector::makeInject(std::string processName, fs::path dllPath)
{
	return makeInject(s2ws(processName), dllPath);
}

bool Injector::makeInject(DWORD dwProcessId, fs::path dllPath)
{
	findLoadProcess();
	setProcessContext(dwProcessId);
	return inject(dllPath);
}
