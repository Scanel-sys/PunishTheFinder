#pragma once

#include <iostream>

#ifdef VACCINE_EXPORTS
	#define VACCINE_API __declspec(dllexport)
#else
	#define VACCINE_API __declspec(dllimport)
#endif