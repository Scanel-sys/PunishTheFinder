#include "Monitor.h"

void Monitor::initArgsWithZeroes()
{
	_pid_passed = false;
	_proc_name_passed = false;
	_func_name_passed = false;
	_file_to_hide_passed = false;
}

std::wstring Monitor::s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::vector<std::string> Monitor::split(int argc, char* argv[])
{
	std::vector <std::string> output;
	int counter = 1;
	std::string temp_string;
	
	while (counter < argc)
	{

		if (ifServiceWord(argv[counter]))
		{
			if (temp_string.size() != 0)
			{
				output.push_back(temp_string);
				temp_string.clear();
			}
			temp_string += argv[counter];
			output.push_back(temp_string);
			temp_string.clear();
		}
		else if (argv[counter][0] ==  '-')
		{
			DisplayResourceNAMessageBox("Wrong argument passed. '-' symbol found unexpectedly");
			exit(1);
		}
		else
		{
			if (temp_string.size() != 0)
				temp_string += " ";
			
			temp_string += argv[counter];
		}
		counter++;
	}
	if (temp_string.size() != 0)
	{
		output.push_back(temp_string);
		temp_string.clear();
	}

	return output;
}

bool Monitor::ifServiceWord(char* word)
{
	int wordlen = strlen(word);
	return (wordlen == 4 && strcmp(word, "-pid") == 0 ||
			wordlen == 5 && (strcmp(word, "-func") == 0 ||
							 strcmp(word, "-name") == 0 ||
							 strcmp(word, "-hide") == 0));
}

void Monitor::setArgs(std::vector<std::string> parsed_args)
{
	if (parsed_args.size() > 1)
	{
		if (parsed_args[0] == "-pid")
		{
			this->_pid = stoi(parsed_args[1]);
			_pid_passed = true;

		}
		else if (parsed_args[0] == "-name")
		{
			this->_processName = s2ws(parsed_args[1]);
			_proc_name_passed = true;
		}
	}

	if (parsed_args.size() > 3)
	{
		if (parsed_args[2] == "-func")
		{
			this->_funcName.append(parsed_args[3]);
			_func_name_passed = true;
		}
	}


	int argv_hide_number = -1;
	if (parsed_args.size() > 3 && parsed_args[2] == "-hide")
	{
		argv_hide_number = 2;
	}
	else if (parsed_args.size() > 5 && parsed_args[4] == "-hide")
	{
		argv_hide_number = 4;
	}

	if (argv_hide_number != -1)
	{
		_file_to_hide_passed = true;
		this->_fileToHide = s2ws(parsed_args[argv_hide_number + 1]);
	}
}

Monitor::Monitor(int argc, char* argv[])
{
	initArgsWithZeroes();
	if (!findVaccine())
	{
		DisplayResourceNAMessageBox("Vaccine.dll wasnt found");
		exit(1);
	}

	std::vector <std::string> splittedArgs = split(argc, argv);
	setArgs(splittedArgs);
}

Monitor::~Monitor()
{
	CloseHandle(_pipe); 
}

void Monitor::launch()
{
	if (!inject())
	{
		DisplayResourceNAMessageBox("Inject error");
		exit(1);
	}
	init_and_launch_pipe();
	monitor();
}

bool Monitor::findVaccine()
{
	fs::path vaccine_path;
	vaccine_path = fs::current_path();
	vaccine_path.append("Vaccine.dll");
	
	if (fs::exists(vaccine_path))
	{
		_dllPath = vaccine_path;
		return true;
	}

	vaccine_path = "..\\..\\Vaccine\\Debug\\Vaccine.dll";
	if (fs::exists(vaccine_path))
	{
		_dllPath = vaccine_path;
		return true;
	}

	return false;
}

bool Monitor::inject()
{
	bool result = false;

	if (_pid_passed)
		result = _injector.makeInject(_pid, _dllPath);
	else
		result = _injector.makeInject(_processName, _dllPath);

	return result;
}

void Monitor::init_and_launch_pipe()
{
	_pipe = CreateNamedPipeA("\\\\.\\pipe\\pipe_of_insight", 
							PIPE_ACCESS_DUPLEX, 
							PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
							PIPE_UNLIMITED_INSTANCES, 
							300, 300, 0, NULL);

	ConnectNamedPipe(_pipe, NULL);

	int howMuchToSent = 0;

	if (_func_name_passed)	howMuchToSent++;
	if (_file_to_hide_passed)	howMuchToSent++;

	WriteFile(_pipe, std::to_string(howMuchToSent).c_str(), 
					 std::to_string(howMuchToSent).size() + 1, NULL, NULL);

	if (_func_name_passed)
	{
		WriteFile(_pipe, "-func", strlen("-func") + 1, NULL, NULL);
		WriteFile(_pipe, _funcName.c_str(), _funcName.size() + 1, NULL, NULL);
	}

	if (_file_to_hide_passed)
	{
		WriteFile(_pipe, "-hide", strlen("-hide") + 1, NULL, NULL);
		WriteFile(_pipe, _fileToHide.string().c_str(), _fileToHide.string().size() + 1, NULL, NULL);
	}
}


int Monitor::DisplayResourceNAMessageBox(std::string msg)
{
	int msgboxID = MessageBox(
								NULL,
								(LPCWSTR)s2ws(msg).c_str(),
								(LPCWSTR)L"Monitor",
								MB_OK
							 );

	switch (msgboxID)
	{
	case IDOK:
		break;
	}

	return msgboxID;
}

void Monitor::monitor()
{

	char msg[300] = { 0 };
	while (1) 
	{
		if(ReadFile(_pipe, msg, 300, NULL, NULL))
			std::cout << msg << std::endl;

		if (GetAsyncKeyState(VK_ESCAPE) & 1)
		{
			WriteFile(_pipe, "-stop", strlen("-stop") + 1, NULL, NULL);
			break;
		}
	}
}
