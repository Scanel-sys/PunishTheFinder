#include "Injector.h"


int main(int argc, char * argv[])
{
	std::wstring proc_name = Injector::s2ws(argv[1]);
	fs::path dll_path = argv[2];

	Injector injector;
	injector.makeInject(proc_name, dll_path);
}