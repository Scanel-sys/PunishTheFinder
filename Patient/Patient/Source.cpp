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
	fs::path file_name = argv[1];
	fs::path file_name_to_create = argv[2];

	PVOID OldValue = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LARGE_INTEGER filesize;

	std::cout << "\n";
	int i = 0;
	while (i++ < 4)
	{
		std::cout << "| log | Starting finding file...\n";

		hFind = FindFirstFile(file_name.wstring().data(), &FindFileData);
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
		
			Sleep(500);
		} while (FindNextFile(hFind, &FindFileData) != 0);
		if(GetLastError() == NO_MORE_FILES)
			std::cout << "| log | No more files" << '\n';
		FindClose(hFind);
	}


	HANDLE hFile = CreateFile(file_name_to_create.c_str(),		// name of the write
								GENERIC_WRITE,          // open for writing
								0,                      // do not share
								NULL,                   // default security
								CREATE_NEW,             // create new file only
								FILE_ATTRIBUTE_NORMAL,  // normal file
								NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "| log |   CreateFile failure: creating new file failured" << file_name_to_create << '\n';
	}
	CloseHandle(hFile);
	HANDLE hFile2 = CreateFile(file_name_to_create.c_str(),								// file to open
								GENERIC_READ,									// open for reading
								FILE_SHARE_READ,								// share for reading
								NULL,											// default security
								OPEN_EXISTING,									// existing file only
								FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// normal file
								NULL);											// no attr. template

	if (hFile2 == INVALID_HANDLE_VALUE)
	{
		std::cout << "| log |  CreateFile failure: file doesnt exist " << file_name_to_create << '\n';
	}

	CloseHandle(hFile2);

	return 0;
}
