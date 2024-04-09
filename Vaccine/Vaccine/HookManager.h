#ifndef HOOKMANAGER_H
#define HOOKMANAGER_H

#include "Hookpatch32.h"




class HookManager
{
private:
	std::string _thread_name;
	std::vector <Hookpatch32 *> _hooks{};

public:
	HookManager();
	HookManager(std::string thread_name);
	~HookManager();
	
	std::string getHackThreadName();

	void Init();
	void push_back_hook(Hookpatch32 * hook);
};

#endif