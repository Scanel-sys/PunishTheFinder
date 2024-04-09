#include "pch.h"
#include "Hookpatch32.h"


Hookpatch32::Hookpatch32()
{
	_gateway = nullptr;
	_stolen_bytes = nullptr;
	_src = nullptr;
	_dst = nullptr;
	_len = 0;
}

Hookpatch32::~Hookpatch32()
{
	if (_stolen_bytes != nullptr)
		delete[] _stolen_bytes;
}

BYTE* Hookpatch32::trampHook(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < PATCH_SIZE_32)	return 0;
	
	_len = len;
	_dst = dst;
	_src = src;
	_stolen_bytes = new BYTE[len];
	memcpy_s(_stolen_bytes, len, src, len);

	_gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(_gateway, len, src, len);		//write back stolen bytes
	
	//get the gateway to dst addr
	uintptr_t gatewayRelAddr = src - _gateway - PATCH_SIZE_32;

	//add JMP opcode to the end of gateway && addr of gateway
	*(_gateway + len) = RELATIVE_JMP;
	*(uintptr_t*)((uintptr_t)_gateway + len + 1) = gatewayRelAddr;

	//perform the detour
	detour(src, dst, len);
	
	return _gateway;
}

void Hookpatch32::trampUnhook()
{
	if (_len < PATCH_SIZE_32)	return;

	if (_src != nullptr)
	{
		DWORD curProtection;
		VirtualProtect(_src, _len, PAGE_EXECUTE_READWRITE, &curProtection);
		memcpy_s(_src, _len, _stolen_bytes, _len);
		VirtualProtect(_src, _len, curProtection, &curProtection);
	}

	if(_gateway != nullptr)
		VirtualFree(_gateway, 0, MEM_RELEASE);
}

bool Hookpatch32::detour(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < PATCH_SIZE_32)	return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relAddr = dst - src - PATCH_SIZE_32;

	*src = RELATIVE_JMP;
	*(uintptr_t*)(src + 1) = relAddr;

	VirtualProtect(src, len, curProtection, &curProtection);

	return true;
}
