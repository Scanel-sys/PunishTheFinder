#include <iostream>
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <string>
#include <filesystem>

#pragma comment(lib, "User32.lib")

#define NO_MORE_FILES		(18)

namespace fs = std::filesystem;


void DisplayError(LPTSTR lpszFunction);


int main(int argc, char * argv[])
{
	//fs::path file_name = argv[1];
	//fs::path file_name_to_find = argv[2];
	fs::path file_name = "C:\\Users\\scanel\\Desktop\\src\\study\\3_course\\2_sem\\TRSPO\\2\\Patient\\Debug\\123.txt";
	fs::path file_name_to_find = "C:\\Users\\scanel\\Desktop\\src\\study\\3_course\\2_sem\\TRSPO\\2\\Patient\\Debug\\123*.txt";


	PVOID OldValue = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LARGE_INTEGER filesize;

	std::cout << "\n";
	int i = 0;
	while (i++ < 2)
	{
		std::cout << "| log | Starting finding file...\n";

		hFind = FindFirstFile(file_name_to_find.wstring().data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				_tprintf(TEXT("| log |   %s   <DIR>\n"), FindFileData.cFileName);
			}
			else
			{
				filesize.LowPart = FindFileData.nFileSizeLow;
				filesize.HighPart = FindFileData.nFileSizeHigh;
				wprintf(TEXT("| log |   %s   %ld bytes\n"), FindFileData.cFileName, filesize.QuadPart);
			}
		
			Sleep(1000);
		} while (FindNextFile(hFind, &FindFileData) != 0);
		if(GetLastError() == NO_MORE_FILES)
			std::cout << "| log | No more files" << '\n';
		FindClose(hFind);
	}

	system("PAUSE");
	//HANDLE hFile = CreateFile(file_name.c_str(),		// name of the write
	//							GENERIC_WRITE,          // open for writing
	//							0,                      // do not share
	//							NULL,                   // default security
	//							CREATE_NEW,             // create new file only
	//							FILE_ATTRIBUTE_NORMAL,  // normal file
	//							NULL);                  // no attr. template


	HANDLE hFile = CreateFile(file_name.c_str(),								// file to open
		GENERIC_READ,									// open for reading
		FILE_SHARE_READ,								// share for reading
		NULL,											// default security
		OPEN_EXISTING,									// existing file only
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// normal file
		NULL);											// no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile\n";
		std::cout << "Terminal failure: file doesnt exist " << file_name.string().c_str() << '\n';
	}

	CloseHandle(hFile);

	system("PAUSE");

	return 0;
}
