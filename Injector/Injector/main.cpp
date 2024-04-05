#include "Injector.h"


int main(int argc, int * argv[])
{
	fs::path dll_path;
	std::wstring proc_name = L"cmd.exe";
	dll_path.append("C:\\Users\\scanel\\Desktop\\src\\study\\3_course\\2_sem\\TRSPO\\2\\Injector\\Injector\\Vaccine.dll");

	Injector injector;
	injector.makeInject(proc_name, dll_path);
	system("PAUSE");
}