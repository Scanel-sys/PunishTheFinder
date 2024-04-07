#include "pch.h"
#include "Hookpatch64.h"

BYTE* Hookpatch64::trampHook(BYTE* src, BYTE* dst, const uintptr_t len)
{
	return nullptr;
}

void Hookpatch64::trampUnhook()
{
}

bool Hookpatch64::detour(BYTE* src, BYTE* dst, const uintptr_t len)
{
	return false;
}
