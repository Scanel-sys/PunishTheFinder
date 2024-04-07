#include "pch.h"
#include "Hookpatch32.h"

BYTE* Hookpatch32::trampHook(BYTE* src, BYTE* dst, const uintptr_t len)
{
	if (len < PATCH_SIZE_32)	return 0;

	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, src, len);		//write back stolen bytes

	//get the gateway to dst addr
	uintptr_t gatewayRelAddr = src - gateway - PATCH_SIZE_32;

	//add JMP opcode to the end of gateway && addr of gateway
	*(gateway + len) = RELATIVE_JMP;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelAddr;

	//perform the detour
	detour(src, dst, len);
	
	return gateway;
}

void Hookpatch32::trampUnhook()
{

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
