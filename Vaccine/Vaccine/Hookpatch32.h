#pragma once
#include "Hookpatch.h"


#define PATCH_SIZE_32			(5)
#define RELATIVE_JMP			(0xE9)


class Hookpatch32 : Hookpatch
{

public:
	BYTE* trampHook(BYTE* src, BYTE* dst, const uintptr_t len) override;
	void trampUnhook() override;
	bool detour(BYTE* src, BYTE* dst, const uintptr_t len) override;

};