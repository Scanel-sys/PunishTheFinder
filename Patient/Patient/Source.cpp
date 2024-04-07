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


int main(int argc, char * argv[])
{
	fs::path file_name = argv[1];
	
	PVOID OldValue = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LARGE_INTEGER filesize;
	//Wow64DisableWow64FsRedirection(&OldValue);
	
	std::cout << "\n";
	while (1)
	{
		std::cout << "| log | Finding first file...\n";

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
		
			Sleep(2000);
			std::cout << "| log | Finding next file...\n";
		} while (FindNextFile(hFind, &FindFileData) != 0);
		if(GetLastError() == NO_MORE_FILES)
			std::cout << "| log | No more files" << '\n';
	}
	FindClose(hFind);
	//Wow64RevertWow64FsRedirection(OldValue);
}