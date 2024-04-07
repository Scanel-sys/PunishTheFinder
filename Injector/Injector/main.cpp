#include "Injector.h"

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


int main(int argc, char * argv[])
{
	std::wstring proc_name = s2ws(argv[1]);
	fs::path dll_path = argv[2];

	Injector injector;
	injector.makeInject(proc_name, dll_path);
}