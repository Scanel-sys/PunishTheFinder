#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <iostream>


class Hookpatch
{
protected:
	BYTE* _gateway;
	BYTE* src;
	BYTE* dst;
	uintptr_t len;

public:
	virtual bool detour(BYTE* src, BYTE* dst, const uintptr_t len) = 0;
	virtual BYTE * trampHook(BYTE* src, BYTE* dst, const uintptr_t len) = 0;
	virtual void trampUnhook() = 0;

};