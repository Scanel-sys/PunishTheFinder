#include "pch.h"
#include "Hookpatch32.h"


Hookpatch32::Hookpatch32()
{
	_stolen_bytes = nullptr;
	_patch_bytes = nullptr;
	_src = nullptr;
	_dst = nullptr;
	_len = 0;
}

Hookpatch32::~Hookpatch32()
{
	if (_stolen_bytes != nullptr)
		delete[] _stolen_bytes;

	if (_patch_bytes != nullptr)
		delete[] _patch_bytes;
}

BYTE* Hookpatch32::initTrampHook(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < PATCH_SIZE_32)	return 0;
	
	_len = len;
	_dst = dst;
	_src = src;
	_stolen_bytes = new BYTE[len];
	_patch_bytes = new BYTE[len];
	
	memcpy_s(_stolen_bytes, len, src, len);

	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, src, len);		//write back stolen bytes

	//prepare gateway
	uintptr_t gatewayRelAddr = src - gateway - PATCH_SIZE_32;
	*(gateway + len) = RELATIVE_JMP;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelAddr;

	//perform the detour
	uintptr_t relAddr = dst - src - PATCH_SIZE_32;
	*_patch_bytes = RELATIVE_JMP;
	*(uintptr_t*)(_patch_bytes + 1) = relAddr;
	this->hook();

	return gateway;
}

void Hookpatch32::init_base_hook(BYTE* src, BYTE* dst, const uintptr_t len)
{
	_src = src;
	_dst = dst;
	_len = len;
	_stolen_bytes = new BYTE[len];
	_patch_bytes = new BYTE[len];

	memcpy_s(_stolen_bytes, len, src, len);

	uintptr_t relAddr = ((DWORD)dst - (DWORD)src - len);
	*_patch_bytes = RELATIVE_JMP;
	*(uintptr_t*)(_patch_bytes + 1) = relAddr;

	this->hook();
}

void Hookpatch32::hook()
{
	if (_len < PATCH_SIZE_32)	return;

	if (_src != nullptr && _patch_bytes != nullptr)
	{
		DWORD curProtection;
		VirtualProtect(_src, _len, PAGE_EXECUTE_READWRITE, &curProtection);
		memcpy_s(_src, _len, _patch_bytes, _len);
		VirtualProtect(_src, _len, curProtection, &curProtection);
	}
}

void Hookpatch32::unhook()
{
	if (_len < PATCH_SIZE_32)	return;

	if (_src != nullptr && _stolen_bytes != nullptr)
	{
		DWORD curProtection;
		VirtualProtect(_src, _len, PAGE_EXECUTE_READWRITE, &curProtection);
		memcpy_s(_src, _len, _stolen_bytes, _len);
		VirtualProtect(_src, _len, curProtection, &curProtection);
	}
}
